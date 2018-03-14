--[[
LuCI - Lua Configuration Interface

Copyright 2008 Steven Barth <steven@midlink.org>
Copyright 2008 Jo-Philipp Wich <xm@leipzig.freifunk.net>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

$Id$
]]--

module("luci.controller.wifidog", package.seeall)

function index()
	if not nixio.fs.access("/etc/config/gateway") then
		return
	end
	
	local page

--[[	
	page = entry({"admin", "network", "wifidog"}, cbi("wifidog/gateway"), _("Gateway Config"), 60)
	page.dependent = true

	page = entry({"admin", "network", "wifidog","wifidog"}, cbi("wifidog/wifidog"), _("Hotspot Config"), 60)
	page.dependent = true


	page = entry({"admin", "network", "wifidog","auth"}, cbi("wifidog/auth"), _("Auth"), 60)
	page.dependent = true


	--page = entry({"admin", "network","snmp"}, cbi("wifidog/snmp"), _("Remote Manage"), 60)
	page.dependent = true
]]--
end
