
require("luci.sys")
require "luci.model.uci"

m=Map("gateway", translate("Remote Manage"))
s = m:section(TypedSection, "gateway")
s.addremove = false
ws=s:option(ListValue, "trap_switch", translate("Remote Manage Switch"))
ws:value("0",translate("Disable"))
ws:value("1",translate("Enable"))
ws.default="0"
s:option(Value, "trap_ip", translate("Remote Manage Server"))
--#s:option(Value, "trap_port", "TrapPort:")



local apply = luci.http.formvalue("cbi.apply")
if apply then
	io.popen("/etc/init.d/snmpagentd reload")
end


return m
