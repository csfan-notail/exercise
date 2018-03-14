--[[
LuCI - Lua Configuration Interface

Copyright (c) 2014 Qualcomm Atheros, Inc.

All Rights Reserved.
Qualcomm Atheros Confidential and Proprietary.

]]--

module("luci.controller.whc", package.seeall)

function index()
	if not nixio.fs.access("/etc/config/lbd") then
		return
	end

	local page

	page = entry({"admin", "network", "whc"}, cbi("whc/whc"), _("Whole Home Coverage"))
	page.dependent = true

end
