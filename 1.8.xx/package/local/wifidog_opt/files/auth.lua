
require("luci.sys")
require "luci.model.uci"

m=Map("gateway",translate("Auth"))
s = m:section(TypedSection, "gateway")
s.addremove = false



gfs=s:option(ListValue, "global_free_auth", translate("Global Free Auth"))
gfs:value("0",translate("Disable"))
gfs:value("1",translate("Enable"))
gfs.default="0"


pfs=s:option(MultiValue, "port_free_auth", translate("Port Free Auth"),translate("choose ports needs free auth,you can choose more than one port"))

pfs.rmempty = true

for k, v in ipairs(luci.sys.net.devices()) do
    if  string.sub(v,1,3)=="eth" or string.sub(v,1,3)=="ath"  then
            pfs:value(v)
    end
end
                



local apply = luci.http.formvalue("cbi.apply")
if apply then
	io.popen("/usr/sbin/free_auth.sh start")
end


return m
