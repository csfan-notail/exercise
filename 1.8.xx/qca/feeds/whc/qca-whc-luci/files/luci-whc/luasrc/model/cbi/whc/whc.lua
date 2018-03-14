--[[
LuCI - Lua Configuration Interface

Copyright (c) 2014 Qualcomm Atheros, Inc.

All Rights Reserved.
Qualcomm Atheros Confidential and Proprietary.

]]--

local enabled_prev = nil
local uci = require "luci.model.uci"

function has_lbd()
	if not nixio.fs.access("/etc/init.d/lbd") then
		return false
	else
		return true
	end
end

function fork_exec(command)
        local pid = nixio.fork()
        if pid > 0 then
                return
        elseif pid == 0 then
                -- change to root dir
                nixio.chdir("/")

                -- patch stdin, out, err to /dev/null
                local null = nixio.open("/dev/null", "w+")
                if null then
                        nixio.dup(null, nixio.stderr)
                        nixio.dup(null, nixio.stdout)
                        nixio.dup(null, nixio.stdin)
                        if null:fileno() > 2 then
                                null:close()
                        end
                end

                -- replace with target command
                nixio.exec("/bin/sh", "-c", command)
        end
end

m = Map("lbd", translate("Whole Home Coverage Settings"),
	translate("Configuration of Whole Home Coverage "))

m.on_before_save = function()
	local uci_r = uci.cursor()
	enabled_prev = uci_r:get("lbd", "config", "Enable")
end

m.on_after_commit = function()
	local enabled_new = nil
	enabled_new = m.uci:get("lbd", "config", "Enable")
	if (enabled_new == "0") then
		fork_exec("/etc/init.d/lbd stop")
	elseif (enabled_prev == "0") then
		fork_exec("/etc/init.d/lbd start")
	else
		fork_exec("/etc/init.d/lbd restart")
	end
end

------------------------------------------------------------------------------------------------
--Basic Settings
------------------------------------------------------------------------------------------------
s = m:section(NamedSection, "config", "basic", translate("Basic Settings"))
s.anonymous = true

e = s:option(Flag, "Enable", translate("Band Steering Enable"))
e.rmempty = false
vl = s:option(Value, "MatchingSSID", translate("SSID to match"))
vl.datatype = "string"

------------------------------------------------------------------------------------------------
--Station Database Setting
------------------------------------------------------------------------------------------------
s = m:section(NamedSection, "StaDB", "StaDB", translate("Station Database"))
s.anonymous = true

e = s:option(Flag, "IncludeOutOfNetwork", translate("Include out-of-network devices"))
e.rmempty = false

function validate_inact_interval(interval, idle_threshold, overload_threshold)
	if (tonumber(interval) <= tonumber(overload_threshold)) and
           (tonumber(interval) <= tonumber(idle_threshold)) then
		return interval
	else
		return nil, "Inactivity check interval cannot be longer than the timer value"
	end
end

------------------------------------------------------------------------------------------------
--2.4 GHz Wlan Interface Setting
------------------------------------------------------------------------------------------------
s = m:section(NamedSection, "WlanIF2G", "WlanIF2G", translate("2.4 GHz Wlan Interface Settings"))
s.anonymous = true

vl_inact_idle = s:option(Value, "InactIdleThreshold", translate("Normal Inactive timer (s)"))
vl_inact_idle.datatype = "uinteger"
vl_inact_overload = s:option(Value, "InactOverloadThreshold", translate("Overload Inactive timer (s)"))
vl_inact_overload.datatype = "uinteger"

vl_inact_freq = s:option(Value, "InactCheckInterval", translate("Inactive Check Frequency (s)"))
vl_inact_freq.datatype = "uinteger"
function vl_inact_freq.validate(self, value, section)
	local idle = vl_inact_idle:formvalue(section)
	local overload = vl_inact_overload:formvalue(section)
	return validate_inact_interval(value, idle, overload)
end

------------------------------------------------------------------------------------------------
--5 GHz Wlan Interface Setting
------------------------------------------------------------------------------------------------
s = m:section(NamedSection, "WlanIF5G", "WlanIF5G", translate("5 GHz Wlan Interface Settings"))
s.anonymous = true

vl_inact_idle = s:option(Value, "InactIdleThreshold", translate("Normal Inactive timer (s)"))
vl_inact_idle.datatype = "uinteger"
vl_inact_overload = s:option(Value, "InactOverloadThreshold", translate("Overload Inactive timer (s)"))
vl_inact_overload.datatype = "uinteger"

vl_inact_freq = s:option(Value, "InactCheckInterval", translate("Inactive Check Frequency (s)"))
vl_inact_freq.datatype = "uinteger"
function vl_inact_freq.validate(self, value, section)
	local idle = vl_inact_idle:formvalue(section)
	local overload = vl_inact_overload:formvalue(section)
	return validate_inact_interval(value, idle, overload)
end

------------------------------------------------------------------------------------------------
--Band Monitor Setting
------------------------------------------------------------------------------------------------
s = m:section(NamedSection, "BandMonitor", "BandMonitor", translate("Utilization Monitor Settings"))
s.anonymous = true

vl = s:option(Value, "MUReserve_W5", translate("5 GHz Reserved Utilization (%)"))
vl.datatype = "and(uinteger, range(0, 100))"

------------------------------------------------------------------------------------------------
--Advanced Settings / Diagnostic Logging Settings
------------------------------------------------------------------------------------------------
js = s:option(DummyValue, "script", translate("script for WHC"))
js.template = "whc/dview_js"

local form_adv, ferr_adv = loadfile(luci.util.libpath() .. "/model/cbi/whc/advanced.lua")
if form_adv then
	bt = s:option(DummyValue, "showadv", translate("show advanced"))
	bt.template = "whc/btn_adv"
	setfenv(form_adv, getfenv(1))(m, s)
end

local form_diag, ferr_diag = loadfile(luci.util.libpath() .. "/model/cbi/whc/diaglog.lua")
if form_diag then
	bt = s:option(DummyValue, "showdiag", translate("show diagnostic logging"))
	bt.template = "whc/btn_diag"
	setfenv(form_diag, getfenv(1))(m, s)
end

return m
