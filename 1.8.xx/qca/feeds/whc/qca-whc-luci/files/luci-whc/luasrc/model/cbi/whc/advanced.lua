--[[
LuCI - Lua Configuration Interface

Copyright (c) 2014 Qualcomm Atheros, Inc.

All Rights Reserved.
Qualcomm Atheros Confidential and Proprietary.

]]--

local m, s = ...

------------------------------------------------------------------------------------------------
--Station database - Advanced
------------------------------------------------------------------------------------------------
s = m:section(NamedSection, "StaDB_Adv", "StaDB_Adv", translate("Station Database Advanced"))
s.anonymous = true

vl = s:option(Value, "AgingSizeThreshold", translate("Size Threshold For Aging Timer"))
vl.datatype = "uinteger"
vl = s:option(Value, "AgingFrequency", translate("Aging Timer Frequency (s)"))
vl.datatype = "uinteger"
vl = s:option(Value, "OutOfNetworkMaxAge", translate("Max Age for Out-of-Network Client (s)"))
vl.datatype = "uinteger"
vl = s:option(Value, "InNetworkMaxAge", translate("Max Age for In-Network Client (s)"))
vl.datatype = "uinteger"
vl = s:option(Value, "ProbeMaxInterval", translate("Max interval in seconds allowed for two probe requests to be averaged"))
vl.datatype = "uinteger"

function validate_mu_interval(interval, period)
	if tonumber(interval) > tonumber(period) then
		return nil, "Medium Utilization check interval cannot be longer than the average period"
	end
	return interval
end

------------------------------------------------------------------------------------------------
--2.4 GHz Wlan Interface Setting - Advanced
------------------------------------------------------------------------------------------------
s = m:section(NamedSection, "WlanIF2G_Adv", "WlanIF2G_Adv", translate("2.4 GHz Wlan Interface Advanced Settings"))
s.anonymous = true

vl = s:option(Value, "RSSISteeringPoint", translate("RSSI value indicating a node associated on 5 GHz should be steered to 2.4 GHz (dB)"))
vl.datatype = "uinteger"
vl = s:option(Value, "LowRSSIXingThreshold", translate("RSSI threshold to generate an indication when a client crosses it (dB)"))
vl.datatype = "uinteger"
vl_mu_period = s:option(Value, "MUAvgPeriod", translate("Time to average before generating a new utilization report on 2.4 GHz (s)"))
vl_mu_period.datatype = "uinteger"

vl_mu_interval = s:option(Value, "MUCheckInterval", translate("The frequency to check medium utilization on 2.4 GHz (s)"))
vl_mu_interval.datatype = "uinteger"
function vl_mu_interval.validate(self, value, section)
	local period = vl_mu_period:formvalue(section)
	return validate_mu_interval(value, period)
end

------------------------------------------------------------------------------------------------
--5 GHz Wlan Interface Setting - Advanced
------------------------------------------------------------------------------------------------
s = m:section(NamedSection, "WlanIF5G_Adv", "WlanIF5G_Adv", translate("5 GHz Wlan Interface Advanced Settings"))
s.anonymous = true

vl = s:option(Value, "RSSISteeringPoint", translate("RSSI value indicating a node associated on 2.4 GHz should be steered to 5 GHz (dB)"))
vl.datatype = "uinteger"
vl = s:option(Value, "LowRSSIXingThreshold", translate("RSSI threshold to generate an indication when a client crosses it (dB)"))
vl.datatype = "uinteger"
vl_mu_period = s:option(Value, "MUAvgPeriod", translate("Time to average before generating a new utilization report on 5 GHz (s)"))
vl_mu_period.datatype = "uinteger"

vl_mu_interval = s:option(Value, "MUCheckInterval", translate("The frequency to check medium utilization on 5 GHz (s)"))
vl_mu_interval.datatype = "uinteger"
function vl_mu_interval.validate(self, value, section)
	local period = vl_mu_period:formvalue(section)
	return validate_mu_interval(value, period)
end

------------------------------------------------------------------------------------------------
--Station Monitor Setting - Advanced
------------------------------------------------------------------------------------------------
s = m:section(NamedSection, "StaMonitor_Adv", "StaMonitor_Adv", translate("Post-association steering decision maker"))
s.anonymous = true
vl = s:option(Value, "RSSIMeasureSamples_W2", translate("Number of RSSI measurements on 2.4 GHz band"))
vl.datatype = "uinteger"
vl = s:option(Value, "RSSIMeasureSamples_W5", translate("Number of RSSI measurements on 5 GHz band"))
vl.datatype = "uinteger"
vl = s:option(Value, "RSSIDiff_EstW5FromW2", translate("Difference when estimating 5 GHz RSSI value from the one measured on 2.4 GHz"))
vl.datatype = "integer"
vl = s:option(Value, "RSSIDiff_EstW2FromW5", translate("Difference when estimating 2.4 GHz RSSI value from the one measured on 5 GHz"))
vl.datatype = "integer"
vl = s:option(Value, "RSSIAgeLimit", translate("Maximum number of seconds elapsed allowed for a 'recent' RSSI measurement"))
vl.datatype = "uinteger"
vl = s:option(Value, "ProbeCountThreshold", translate("Number of probe requests required for the RSSI averaging"))
vl.datatype = "uinteger"

------------------------------------------------------------------------------------------------
--Band Monitor Settings - Advanced
------------------------------------------------------------------------------------------------
s = m:section(NamedSection, "BandMonitor_Adv", "BandMonitor_Adv", translate("Utilization Monitor Advanced Settings"))
s.anonymous = true

vl = s:option(Value, "MUSlightOverloadThreshold_W2", translate("Medium utilization threshold for a slight overload condition on 2.4 GHz (%)"))
vl.datatype = "and(uinteger, range(0, 100))"
vl = s:option(Value, "MUHeavyOverloadThreshold_W2", translate("Medium utilization threshold for a heavy overload condition on 2.4 GHz (%)"))
vl.datatype = "and(uinteger, range(0, 100))"
vl = s:option(Value, "MUSlightOverloadThreshold_W5", translate("Medium utilization threshold for a slight overload condition on 5 GHz (%)"))
vl.datatype = "and(uinteger, range(0, 100))"
vl = s:option(Value, "MUHeavyOverloadThreshold_W5", translate("Medium utilization threshold for a heavy overload condition on 5 GHz (%)"))
vl.datatype = "and(uinteger, range(0, 100))"
vl = s:option(Value, "RSSIMaxAge", translate("Max Age for RSSI measurement allowed for pre-association decision (s)"))
vl.datatype = "uinteger"
vl = s:option(Value, "ProbeCountThreshold", translate("Number of probe requests required for the RSSI averaging"))
vl.datatype = "uinteger"

------------------------------------------------------------------------------------------------
--Steer Executor Settings - Advanced
------------------------------------------------------------------------------------------------
s = m:section(NamedSection, "SteerExec_Adv", "SteerExec_Adv", translate("Steering Executor Advanced Settings"))
s.anonymous = true
vl = s:option(Value, "SteeringProhibitTime", translate("Time to wait before steering the client again after completing steering (s)"))
vl.datatype = "uinteger"
vl = s:option(Value, "TSteering", translate("Maximum time for client to associate on target band before AP aborts steering (s)"))
vl.datatype = "uinteger"
vl = s:option(Value, "InitialAuthRejCoalesceTime", translate("Time to coalesce multiple authentication rejects down to a single one (s)"))
vl.datatype = "uinteger"
vl = s:option(Value, "AuthRejMax", translate("Max consecutive authentication rejects after which the device is marked as steering unfriendly"))
vl.datatype = "uinteger"
vl = s:option(Value, "SteeringUnfriendlyTime", translate("The amount of time a device is considered steering unfriendly before another attempt (s)"))
vl.datatype = "uinteger"
vl = s:option(Value, "TargetLowRSSIThreshold_W2", translate("RSSI threshold indicating 2.4 GHz band is not strong enough for association (dB)"))
vl.datatype = "uinteger"
vl = s:option(Value, "TargetLowRSSIThreshold_W5", translate("RSSI threshold indicating 5 GHz band is not strong enough for association (dB)"))
vl.datatype = "uinteger"
vl = s:option(Value, "BlacklistTime", translate("The amount of time (in seconds) before automatically removing the blacklist (s)"))
vl.datatype = "uinteger"
