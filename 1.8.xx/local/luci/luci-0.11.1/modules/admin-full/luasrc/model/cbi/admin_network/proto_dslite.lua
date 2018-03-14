--[[
LuCI - Lua Configuration Interface

Copyright 2011 Jo-Philipp Wich <xm@subsignal.org>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0
]]--

local map, section, net = ...

local ip6addr, peeraddr, ipaddr, netmask, ttl, mtu


ip6addr = section:taboption("general", Value, "ip6addr", translate("Local IPv6 address"))
ip6addr.datatype = "ip6addr"

peeraddr = section:taboption("general", Value, "peeraddr", translate("Peer IPv6 address"))
peeraddr.rdatatype = "ip6addr"


ipaddr = section:taboption("general", Value, "ipaddr", translate("Tunnel address"))
ipaddr.rdatatype = "ip4addr"

netmask = section:taboption("general", Value, "netmask", translate("IPv4 netmask"))
netmask.datatype = "ip4addr"
netmask:value("255.255.255.0")
netmask:value("255.255.0.0")
netmask:value("255.0.0.0")

mtu = section:taboption("advanced", Value, "mtu", translate("Use MTU on tunnel interface"))
mtu.placeholder = "1500"
mtu.datatype    = "max(1500)"

ttl = section:taboption("advanced", Value, "ttl", translate("Use TTL on tunnel interface"))
ttl.placeholder = "64"
ttl.datatype    = "range(1,255)"
