
require("luci.sys")
require "luci.model.uci"

function gateway_status_get()
	luci.sys.exec("touch /tmp/wifidog.status")
	local file=assert(io.open("/tmp/wifidog.status","r"))
	local status = file:read("*all")
	file:close()
	return status
end

t_status = {}
wifidog_status = gateway_status_get()
wifidog_status = string.sub(wifidog_status,1,1)
if wifidog_status == "1" then
	t_status.wifidog_status = translate("Online")
else
	t_status.wifidog_status = translate("Offline")
end


m=SimpleForm("status", translate("Gateway Status"),translate("Dispaly wifidog/sta online status"))
--[[
m=Map("gateway", translate("Gateway Status"),translate("Dispaly wifidog/sta online status"))
]]--
m.reset = false                                                                                                         
m.submit = false


s = m:section(Table,t_status)
s.addremove = false
ws=s:option(DummyValue, "wifidog_status", translate("Hotspot Online"))
ws.value=t_status.wifidog_status

--s:option(DummyValue, "snmp_status", translate("Remote manage Online"))

return m
