
require("luci.sys")
require "luci.model.uci"

m=Map("gateway", translate("Hotspot Config"))
s = m:section(TypedSection, "gateway")
s.addremove = false
ws=s:option(ListValue, "wifidog_switch", translate("Hotspot Switch"))
ws:value("0",translate("Disable"))
ws:value("1",translate("Enable"))
ws:value("3",translate("Third-party certification"))
ws.default="0"

hotspot_name=s:option(Value, "device_hotspot_name", translate("Hotspot Name"))
hotspot_name:depends("wifidog_switch","1")
hotspot_name:depends("wifidog_switch","0") 

s:option(Value, "device_hotspot_url",translate("Platform Address"))

hotspot_path=s:option(Value, "device_hotspot_path", translate("Platform PATH"))
hotspot_path:depends("wifidog_switch","1")
hotspot_path:depends("wifidog_switch","0")

--[[
s:option(Value, "ccsserver", "CcsServer:")

#iosap=s:option(ListValue, "IosAutoPortal", "IosAutoPortal:")
#iosap:value("0",translate("Disable"))
#iosap:value("1",translate("Enable"))
#iosap.default="1"

#ws=s:option(ListValue, "vlanfc_rule", "Vlanfc_Switch:")
#ws:value("0",translate("Disable"))
#ws:value("1",translate("Enable"))
#ws.default="0"
]]--
local apply = luci.http.formvalue("cbi.apply")
if apply then
	io.popen("/etc/init.d/wifidog reload")
end


return m
