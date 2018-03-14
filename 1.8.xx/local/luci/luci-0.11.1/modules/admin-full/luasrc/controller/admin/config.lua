--[[
LuCI - Lua Configuration Interface

Copyright 2008 Steven Barth <steven@midlink.org>
Copyright 2011 Jo-Philipp Wich <xm@subsignal.org>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

$Id$
]]--

module("luci.controller.admin.config", package.seeall)

require("luci.http")

function index()
	entry({"admin", "config"}, alias("admin", "config", "port mirror template"), _("Configuration"), 60).index = true
	--port mirrot
	entry({"admin", "config", "port mirror template"}, template("tau_view/mirror"), _("Port Mirror"), 1)
	entry({"admin", "config", "port_switch"}, call("port_switch"), nil).leaf = true
	entry({"admin", "config", "port_mirror"}, call("port_mirror"), nil).leaf = true
	entry({"admin", "config", "port_observe"}, call("port_observe"), nil).leaf = true
	--portmap
	entry({"admin", "config", "port map"}, template("tau_view/port_map"), _("Port map"),2)
	entry({"admin", "config", "port_map"}, call("port_map"), nil).leaf = true
	--nat
	entry({"admin", "config", "lte nat"}, template("tau_view/nat_config"), _("LTE NAT"),3)
	entry({"admin", "config", "nat_config"}, call("nat_config"), nil).leaf = true
	--ospf
	entry({"admin", "config", "ospf config"}, template("tau_view/ospf_config"), _("Ospf config"),4)
	entry({"admin", "config", "ospf_config"}, call("ospf_config"), nil).leaf = true
	entry({"admin", "config", "ospf_sw_config"}, call("ospf_sw_config"), nil).leaf = true
	--gre
	entry({"admin", "config", "gre template"}, template("tau_view/gre"), _("GRE"), 5)
	entry({"admin", "config", "gre_switch"}, call("gre_switch"), nil).leaf = true
	--IPsec
	entry({"admin","config","ipsec_set"}, alias("admin", "config", "ipsec_set", "ipsec_config"), _("IPSec"), 6)
	entry({"admin","config","ipsec_set","ipsec_config"},template("tau_view/ipsec_config"),_("IPSec Config"),1)
	entry({"admin","config","ipsec_config"},call("ipsec_config"),nil).leaf = true	
	entry({"admin","config","ipsec_set","status"}, call("action_ipsec_status")).leaf = true
	entry({"admin","config","ipsec_set","psk_auth"},template("tau_view/ipsec_psk"),_("PSK "),2)
	entry({"admin","config","psk_auth"},call("psk_auth"),nil).leaf = true	
	entry({"admin","config","ipsec_set","rsa_auth"},template("tau_view/ipsec_rsa"),_("RSA"),3)
	entry({"admin","config","rsa_auth"},call("rsa_auth"),nil).leaf = true
	entry({"admin","config","ipsec_set","x509_auth"},template("tau_view/ipsec_x509"),_("X.509"),4)
	entry({"admin","config","x509_auth"},call("x509_auth"),nil).leaf = true
	--muticast
	entry({"admin","config","muticast set"},template("tau_view/muticast"),_("Muticast"),7)
	entry({"admin", "config", "muticast_set"}, call("muticast_set"), nil).leaf = true
	--vrrp
	entry({"admin","config","vrrp_set"}, template("tau_view/vrrp"), _("VRRP"), 8)
	entry({"admin","config","vrrp_conf_set"},call("vrrp_conf_set"),nil).leaf = true 
	--attenuator
	--entry({"admin","config","attenuator set"},template("tau_view/attenuator"),_("Attenuator"),9)
	--entry({"admin", "config", "attenuator_set"}, call("attenuator_set"), nil).leaf = true
	--mini_snmpd
	entry({"admin", "config", "mini_snmpd"}, template("tau_view/mini_snmpd"), _("Remote Manage"), 10)
	entry({"admin", "config", "snmpd_switch"}, call("snmpd_switch"), nil).leaf = true
	--logexport
	entry({"admin", "config", "log export"}, call("log_export"), _("Log export"), 11)
	entry({"admin", "config", "log_config"}, call("log_config"), nil).leaf = true
	entry({"admin", "config", "delete_config"}, call("delete_config"), nil).leaf = true
	--packexport
	entry({"admin", "config", "pack export"}, call("pack_export"), _("Pack export"), 12)
	entry({"admin", "config", "pack_config"}, call("pack_config"), nil).leaf = true
	entry({"admin", "config", "packet_delete"}, call("packet_delete"), nil).leaf = true
	--webuser
	entry({"admin", "config", "web user"}, template("tau_view/webuser"), _("Web user"),13)
	entry({"admin", "config", "web_user"}, call("web_user"), nil).leaf = true
	--manual_set_time
	entry({"admin","config","time set"},template("tau_view/manual_setting_time"),_("Manual Setting Time"),14)
	entry({"admin", "config", "time_set"}, call("action_clock_status")).leaf = true
	--command line
	entry({"admin", "config", "cmd line"}, template("tau_view/cmd_line"), _("Cmd line"),15)
	entry({"admin", "config", "cmd_line"}, call("cmd_line"), nil).leaf = true
	
	--Time synchronization
	entry({"admin", "config", "time synchronization"}, template("tau_view/time_synchronization"), _("Time Synchronization"),16)
	entry({"admin", "config", "time_synchronization"}, call("time_synchronization"), nil).leaf = true
	--Tr069
	entry({"admin", "config", "tr069 config"}, template("tau_view/tr069"), _("Tr069 config"),17)
	entry({"admin", "config", "tr069_config"}, call("tr069_config"), nil).leaf = true
	
	--GD contrl
	--entry({"admin", "config", "gd contrl"}, template("tau_view/gdcontrl"), _("GD Contrl"),18)
	--entry({"admin", "config", "gd_contrl"}, call("gd_contrl"), nil).leaf = true
	
	--GD contrl
	--entry({"admin","config","gd contrl"}, alias("admin", "config", "gd contrl", "gd contrl"), _("GD Contrl"), 19)
	--entry({"admin","config","gd contrl","gd contrl"},template("tau_view/gdcontrl"),_("GD Contrl "),1)
	--entry({"admin","config","gd_contrl"},call("gd_contrl"),nil).leaf = true	
	--entry({"admin","config","gd contrl","gd server"},template("tau_view/gdserver"),_("GD Server"),2)
	--entry({"admin","config","gd_server"},call("gd_server"),nil).leaf = true
	
	--route rule
	entry({"admin", "config", "policy router"}, template("tau_view/policy_router"), _("Policy Router"),20)
	entry({"admin", "config", "policy_router"}, call("policy_router"), nil).leaf = true
	
	--route config
	entry({"admin", "config", "router config"}, template("tau_view/route_config"), _("Router Config"),21)
	entry({"admin", "config", "route_config"}, call("route_config"), nil).leaf = true

	--virtual ip
	entry({"admin", "config", "virtual ip"}, template("tau_view/virtual_ip"), _("Virtual IP"),23)
	entry({"admin", "config", "virtual_ip"}, call("virtual_ip"), nil).leaf = true
	
	entry({"admin", "config", "set_infos"}, call("set_infos"), nil).leaf = true

end

function virtual_list_del(number)

	local next_num=""
	local next_ifname=""
	local next_ip_addr=""
	local next_ip_mask=""
	local next_interface=""
	
	if 0 == tonumber(number) then 
		return
	end
	
	for num=1,tonumber(number) do
		if  luci.sys.exec("uci get vip.@list"..num.."[0].vip_addr") == "" then 
			
			if tonumber(number) == tonumber(num) then 
				luci.sys.exec("uci delete vip.@list"..number.."[0]")
			else
				next_num=num+1
				next_ip_addr=luci.sys.exec("uci get vip.@list"..next_num.."[0].vip_addr")
				next_ip_mask=luci.sys.exec("uci get vip.@list"..next_num.."[0].vip_mask")
				next_ifname=luci.sys.exec("uci get vip.@list"..next_num.."[0].vip_ifname")
				next_interface=luci.sys.exec("uci get vip.@list"..next_num.."[0].vip_interface")
				
				luci.sys.exec("uci add vip list"..num)
				luci.sys.exec("uci set vip.@list"..num.."[0].vip_addr="..next_ip_addr)
				luci.sys.exec("uci set vip.@list"..num.."[0].vip_mask="..next_ip_mask)
				luci.sys.exec("uci set vip.@list"..num.."[0].vip_ifname="..next_ifname)
				luci.sys.exec("uci set vip.@list"..num.."[0].vip_interface="..next_interface)
				
				luci.sys.exec("uci delete vip.@list"..next_num.."[0].vip_addr")
				luci.sys.exec("uci delete vip.@list"..next_num.."[0].vip_mask")
				luci.sys.exec("uci delete vip.@list"..next_num.."[0].vip_ifname")
				luci.sys.exec("uci delete vip.@list"..next_num.."[0].vip_interface")
				luci.sys.exec("uci delete vip.@list"..next_num.."[0]")
			end
		end
		
	end	
		
end


function virtual_ip()
	local uci = require "luci.model.uci".cursor()
	
	local actions = luci.http.formvalue('param1')
	local vip_addr = luci.http.formvalue('param2')
	local vip_mask = luci.http.formvalue('param3')
	local vip_ifname = luci.http.formvalue('param4')
	local vip_interface = luci.http.formvalue('param5')
	
	local cmd = ""
	local opr_num = 0
	local ret = -1
	local check_list = 0
	
	local total_num = luci.sys.exec("uci get vip.@list[0].number")
	total_num = total_num.gsub(total_num,"\n","")
	
	luci.sys.exec("echo '\n\n---------- '$(date)'-----------' >> /tmp/vip_log")
	luci.sys.exec("echo 'actions="..actions..",vip_addr="..vip_addr..",vip_mask="..vip_mask..",vip_ifname="..vip_ifname..",vip_interface="..vip_interface.."' >> /tmp/vip_log")
	if 0 == tonumber(actions) then
		opr_num = total_num+1
		
		if 0 ~= tonumber(total_num) then
		for num=1,tonumber(total_num) do
			local get_ip_addr=luci.sys.exec("uci get vip.@list"..num.."[0].vip_addr")
			get_ip_addr = get_ip_addr.gsub(get_ip_addr,"\n","")
			vip_addr = vip_addr.gsub(vip_addr,"\n","")
			
			local get_ip_mask=luci.sys.exec("uci get vip.@list"..num.."[0].vip_mask")
			get_ip_mask = get_ip_mask.gsub(get_ip_mask,"\n","")
			vip_mask = vip_mask.gsub(vip_mask,"\n","")
			
			local get_ifname=luci.sys.exec("uci get vip.@list"..num.."[0].vip_ifname")
			get_ifname = get_ifname.gsub(get_ifname,"\n","")
			
			local get_interface=luci.sys.exec("uci get vip.@list"..num.."[0].vip_interface")
			get_interface = get_interface.gsub(get_interface,"\n","")
			vip_interface = vip_interface.gsub(vip_interface,"\n","")
			
			if (("" ~= get_ip_addr) and ("" ~= get_ip_mask) and ("" ~= get_interface)) then
				if (tostring(vip_addr) == tostring(get_ip_addr)) then	
					check_list = 1		
				end
				
				if (tostring(vip_ifname) == tostring(get_ifname)) then	
					check_list = 2		
				end
			end
		end
		end
		luci.sys.exec("echo 'total_num="..total_num.."' >> /tmp/vip_log")
		luci.sys.exec("echo 'check_list="..check_list.."' >> /tmp/vip_log")
		if 0 == tonumber(check_list) then
			cmd = "ifconfig "..vip_interface..":"..vip_ifname.." "..vip_addr.." netmask "..vip_mask.." up"
			ret = os.execute(tostring(cmd))
			luci.sys.exec("echo 'ret="..ret..", cmd="..cmd.."' >> /tmp/vip_log")
			if 0 == tonumber(ret) then 
				luci.sys.exec("uci add vip list"..opr_num)
				luci.sys.exec("uci set vip.@list"..opr_num.."[0].vip_addr="..vip_addr)
				luci.sys.exec("uci set vip.@list"..opr_num.."[0].vip_mask="..vip_mask)
				luci.sys.exec("uci set vip.@list"..opr_num.."[0].vip_ifname="..vip_ifname)
				luci.sys.exec("uci set vip.@list"..opr_num.."[0].vip_interface="..vip_interface)
		
				luci.sys.exec("uci set vip.@list[0].number="..opr_num)
			else 
				check_list = 3
			end

		end
	else 
		
		for num=1,tonumber(total_num) do
		
			local get_ip_addr=luci.sys.exec("uci get vip.@list"..num.."[0].vip_addr")
			get_ip_addr = get_ip_addr.gsub(get_ip_addr,"\n","")
			vip_addr = vip_addr.gsub(vip_addr,"\n","")
			
			local get_ip_mask=luci.sys.exec("uci get vip.@list"..num.."[0].vip_mask")
			get_ip_mask = get_ip_mask.gsub(get_ip_mask,"\n","")
			vip_mask = vip_mask.gsub(vip_mask,"\n","")
			
			local get_ifname=luci.sys.exec("uci get vip.@list"..num.."[0].vip_ifname")
			get_ifname = get_ifname.gsub(get_ifname,"\n","")
			
			local get_interface=luci.sys.exec("uci get vip.@list"..num.."[0].vip_interface")
			get_interface = get_interface.gsub(get_interface,"\n","")
			vip_interface = vip_interface.gsub(vip_interface,"\n","")
			
			if (("" ~= get_ip_addr) and ("" ~= get_ip_mask) and ("" ~= get_interface)) then
			if ((vip_addr == get_ip_addr) and (vip_mask == get_ip_mask) and (tostring(vip_ifname) == tostring(get_ifname)) and (vip_interface == get_interface)) then
				cmd = "ip addr del "..vip_addr.." dev "..vip_interface..":"..vip_ifname..""
				os.execute(tostring("/sbin/sysctl -w net.ipv4.conf.all.promote_secondaries=1"))
				ret=os.execute(tostring(cmd))			
				if 0 == tonumber(ret) then 
				
					luci.sys.exec("uci delete vip.@list"..num.."[0].vip_addr")
					luci.sys.exec("uci delete vip.@list"..num.."[0].vip_mask")
					luci.sys.exec("uci delete vip.@list"..num.."[0].vip_ifname")
					luci.sys.exec("uci delete vip.@list"..num.."[0].vip_interface")
					luci.sys.exec("uci delete vip.@list"..num.."[0]")
				
					virtual_list_del(total_num)
				
					luci.sys.exec("uci set vip.@list[0].number="..(total_num-1))
				end
			end
			end
		end
		if 0 == tonumber(ret) then
			check_list = 0
		else
			check_list = 4
		end
		
	end
	if 0 == tonumber(check_list) then
		luci.sys.exec("echo '"..cmd.."' >> /tmp/vip_log")
	
		luci.sys.exec("uci commit vip")
		luci.sys.exec("nohup service network restart &")
		return_write(0)
	else
		luci.sys.exec("echo 'error="..check_list..", cmd="..cmd..", ret="..ret.."' >> /tmp/vip_log")
		luci.sys.exec("echo '############' >> /tmp/vip_log")
		return_write(check_list)
	end
end

function route_list_del(number)

	local next_num=""
	local next_gateway=""
	local next_ip_addr=""
	local next_ip_mask=""
	local next_interface=""
	
	if 0 == tonumber(number) then 
		return
	end
	
	for num=1,tonumber(number) do
		if  luci.sys.exec("uci get route.@list"..num.."[0].ip_addr") == "" then 
			
			if tonumber(number) == tonumber(num) then 
				luci.sys.exec("uci delete route.@list"..number.."[0]")
			else
				next_num=num+1
				next_ip_addr=luci.sys.exec("uci get route.@list"..next_num.."[0].ip_addr")
				next_ip_mask=luci.sys.exec("uci get route.@list"..next_num.."[0].ip_mask")
				next_gateway=luci.sys.exec("uci get route.@list"..next_num.."[0].gateway")
				next_interface=luci.sys.exec("uci get route.@list"..next_num.."[0].interface")
				
				luci.sys.exec("uci add route list"..num)
				luci.sys.exec("uci set route.@list"..num.."[0].ip_addr="..next_ip_addr)
				luci.sys.exec("uci set route.@list"..num.."[0].ip_mask="..next_ip_mask)
				luci.sys.exec("uci set route.@list"..num.."[0].gateway="..next_gateway)
				luci.sys.exec("uci set route.@list"..num.."[0].interface="..next_interface)
				
				luci.sys.exec("uci delete route.@list"..next_num.."[0].ip_addr")
				luci.sys.exec("uci delete route.@list"..next_num.."[0].ip_mask")
				luci.sys.exec("uci delete route.@list"..next_num.."[0].gateway")
				luci.sys.exec("uci delete route.@list"..next_num.."[0].interface")
				luci.sys.exec("uci delete route.@list"..next_num.."[0]")
			end
		end
		
	end	
		
end


function route_config()
	local uci = require "luci.model.uci".cursor()
	
	local actions = luci.http.formvalue('param1')
	local ip_addr = luci.http.formvalue('param2')
	local ip_mask = luci.http.formvalue('param3')
	local gateway = luci.http.formvalue('param4')
	local interface = luci.http.formvalue('param5')
	
	local opt,fun,cfg,opr_num,ret
	local check_list=0
	
	if 0 == tonumber(actions) then
		opt = "add"
	else
		opt = "del"
	end 
	
	if "" == gateway then
		cfg = ""
		gateway = 0
	else
		cfg = "gw"
	end 
	
	local total_num = luci.sys.exec("uci get route.@list[0].number")
	total_num = total_num.gsub(total_num,"\n","")
	
	
	if 0 == tonumber(actions) then
		opr_num = total_num+1
		
		if 0 ~= tonumber(total_num) then
		for num=1,tonumber(total_num) do
			local get_ip_addr=luci.sys.exec("uci get route.@list"..num.."[0].ip_addr")
			get_ip_addr = get_ip_addr.gsub(get_ip_addr,"\n","")
			ip_addr = ip_addr.gsub(ip_addr,"\n","")
			
			local get_ip_mask=luci.sys.exec("uci get route.@list"..num.."[0].ip_mask")
			get_ip_mask = get_ip_mask.gsub(get_ip_mask,"\n","")
			ip_mask = ip_mask.gsub(ip_mask,"\n","")
			
			local get_gateway=luci.sys.exec("uci get route.@list"..num.."[0].gateway")
			get_gateway = get_gateway.gsub(get_gateway,"\n","")
			
			local get_interface=luci.sys.exec("uci get route.@list"..num.."[0].interface")
			get_interface = get_interface.gsub(get_interface,"\n","")
			interface = interface.gsub(interface,"\n","")
			
			if (("" ~= get_ip_addr) and ("" ~= get_ip_mask) and ("" ~= get_interface)) then
			if ((ip_addr == get_ip_addr) and (ip_mask == get_ip_mask) and (tostring(gateway) == tostring(get_gateway)) and (interface == get_interface)) then	
				check_list = 1		
			end
			end
		end
		end
		if 0 == tonumber(check_list) then

			if "0" == tostring(gateway) then
				ret=os.execute(tostring("route "..opt.." -net "..ip_addr.." netmask "..ip_mask.." dev "..interface))
			else
				ret=os.execute(tostring("route "..opt.." -net "..ip_addr.." netmask "..ip_mask.." "..cfg.." "..gateway.." dev "..interface))
			end
			
			if 0 == tonumber(ret) then 
				luci.sys.exec("uci add route list"..opr_num)
				luci.sys.exec("uci set route.@list"..opr_num.."[0].ip_addr="..ip_addr)
				luci.sys.exec("uci set route.@list"..opr_num.."[0].ip_mask="..ip_mask)
				luci.sys.exec("uci set route.@list"..opr_num.."[0].gateway="..gateway)
				luci.sys.exec("uci set route.@list"..opr_num.."[0].interface="..interface)
		
				luci.sys.exec("uci set route.@list[0].number="..opr_num)
			else 
				check_list = 1
			end
			
		end
		
	else 
		
		for num=1,tonumber(total_num) do
		
			local get_ip_addr=luci.sys.exec("uci get route.@list"..num.."[0].ip_addr")
			get_ip_addr = get_ip_addr.gsub(get_ip_addr,"\n","")
			ip_addr = ip_addr.gsub(ip_addr,"\n","")
			
			local get_ip_mask=luci.sys.exec("uci get route.@list"..num.."[0].ip_mask")
			get_ip_mask = get_ip_mask.gsub(get_ip_mask,"\n","")
			ip_mask = ip_mask.gsub(ip_mask,"\n","")
			
			local get_gateway=luci.sys.exec("uci get route.@list"..num.."[0].gateway")
			get_gateway = get_gateway.gsub(get_gateway,"\n","")
			
			local get_interface=luci.sys.exec("uci get route.@list"..num.."[0].interface")
			get_interface = get_interface.gsub(get_interface,"\n","")
			interface = interface.gsub(interface,"\n","")
			
			if (("" ~= get_ip_addr) and ("" ~= get_ip_mask) and ("" ~= get_interface)) then
			if ((ip_addr == get_ip_addr) and (ip_mask == get_ip_mask) and (tostring(gateway) == tostring(get_gateway)) and (interface == get_interface)) then
				
				
				if "0" == tostring(gateway) then
					ret=os.execute(tostring("route "..opt.." -net "..ip_addr.." netmask "..ip_mask.." dev "..interface))
				else
					ret=os.execute(tostring("route "..opt.." -net "..ip_addr.." netmask "..ip_mask.." "..cfg.." "..gateway.." dev "..interface))
				end 
							
				if 0 == tonumber(ret) then 
				
					luci.sys.exec("uci delete route.@list"..num.."[0].ip_addr")
					luci.sys.exec("uci delete route.@list"..num.."[0].ip_mask")
					luci.sys.exec("uci delete route.@list"..num.."[0].gateway")
					luci.sys.exec("uci delete route.@list"..num.."[0].interface")
					luci.sys.exec("uci delete route.@list"..num.."[0]")
				
					route_list_del(total_num)
				
					luci.sys.exec("uci set route.@list[0].number="..(total_num-1))
				else 
					check_list = 1
				end
				
			end
			end
		end
		
	end
	
	if 0 == tonumber(check_list) then
		luci.sys.exec("echo 'route "..opt.." -net "..ip_addr.." netmask "..ip_mask.." "..cfg.." "..gateway.." dev "..interface.."' > /tmp/router_config")
	
		luci.sys.exec("uci commit route")
		return_write(0)
	else
		luci.sys.exec("echo 'error="..ret.."' > /tmp/router_config")
		return_write(-1)
	end
end

function policy_list_del(number)

	local next_num=""
	local next_gateway=""
	local next_table=""
	local next_way=""
	local next_net=""
	
	if 0 == tonumber(number) then 
		return
	end
	
	for num=1,tonumber(number) do
		if  luci.sys.exec("uci get policy.@list"..num.."[0].gateway") == "" then 
			
			if tonumber(number) == tonumber(num) then 
				luci.sys.exec("uci delete policy.@list"..number.."[0]")
			else
				next_num=num+1
				next_gateway=luci.sys.exec("uci get policy.@list"..next_num.."[0].gateway")
				next_table=luci.sys.exec("uci get policy.@list"..next_num.."[0].table")
				next_way=luci.sys.exec("uci get policy.@list"..next_num.."[0].way")
				next_net=luci.sys.exec("uci get policy.@list"..next_num.."[0].net")
				
				
				luci.sys.exec("uci add policy list"..num)
				luci.sys.exec("uci set policy.@list"..num.."[0].gateway="..next_gateway)
				luci.sys.exec("uci set policy.@list"..num.."[0].table="..next_table)
				luci.sys.exec("uci set policy.@list"..num.."[0].way="..next_way)
				luci.sys.exec("uci set policy.@list"..num.."[0].net="..next_net)
				
				luci.sys.exec("uci delete policy.@list"..next_num.."[0].gateway")
				luci.sys.exec("uci delete policy.@list"..next_num.."[0].table")
				luci.sys.exec("uci delete policy.@list"..next_num.."[0].way")
				luci.sys.exec("uci delete policy.@list"..next_num.."[0].net")
				luci.sys.exec("uci delete policy.@list"..next_num.."[0]")
			end
		end
		
	end	
		
end

function policy_router()
	
	local uci = require "luci.model.uci".cursor()
	
	local actions = luci.http.formvalue('param1')
	local gateway = luci.http.formvalue('param2')
	local table = luci.http.formvalue('param3')
	local way = luci.http.formvalue('param4')
	local net = luci.http.formvalue('param5')
	
	local opt,fun,cfg,opr_num
	local ret = -1
	
	if 0 == tonumber(actions) then
		opt = "add"
		cfg = "set"
	else
		opt = "del"
		cfg = "delete"
	end 
	if 0 == tonumber(way) then
		fun = "from"
	else
		fun = "to"
	end 
	
	local total_num = luci.sys.exec("uci get policy.@list[0].number")
	total_num = total_num.gsub(total_num,"\n","")
	
	
	if 0 == tonumber(actions) then
		opr_num = total_num+1
		luci.sys.exec("ip route "..opt.." 0/0 via "..gateway.." table "..table)
		ret = os.execute(tostring("ip rule "..opt.." "..fun.." "..net.." table "..table))
		if (0 == ret) then
		luci.sys.exec("uci add policy list"..opr_num)
		luci.sys.exec("uci set policy.@list"..opr_num.."[0].gateway="..gateway)
		luci.sys.exec("uci set policy.@list"..opr_num.."[0].table="..table)
		luci.sys.exec("uci set policy.@list"..opr_num.."[0].way="..fun)
		luci.sys.exec("uci set policy.@list"..opr_num.."[0].net="..net)
		
		luci.sys.exec("uci set policy.@list[0].number="..opr_num)
		
		end
	else 
		
		for num=1,tonumber(total_num) do
		
			local get_gateway=luci.sys.exec("uci get policy.@list"..num.."[0].gateway")
			get_gateway = get_gateway.gsub(get_gateway,"\n","")
			gateway = gateway.gsub(gateway,"\n","")
			
			local get_table=luci.sys.exec("uci get policy.@list"..num.."[0].table")
			get_table = get_table.gsub(get_table,"\n","")
			table = table.gsub(table,"\n","")
			
			local get_fun=luci.sys.exec("uci get policy.@list"..num.."[0].way")
			get_fun = get_fun.gsub(get_fun,"\n","")
			
			local get_net=luci.sys.exec("uci get policy.@list"..num.."[0].net")
			get_net = get_net.gsub(get_net,"\n","")
			net = net.gsub(net,"\n","")
			
			
			if (("" ~= get_gateway) and ("" ~= get_table) and ("" ~= get_fun) and ("" ~= get_net)) then
			if ((gateway == get_gateway) and (get_table == table) and (get_fun == fun) and (get_net == net)) then
				
				luci.sys.exec("uci delete policy.@list"..num.."[0].gateway")
				luci.sys.exec("uci delete policy.@list"..num.."[0].table")
				luci.sys.exec("uci delete policy.@list"..num.."[0].way")
				luci.sys.exec("uci delete policy.@list"..num.."[0].net")
				luci.sys.exec("uci delete policy.@list"..num.."[0]")
				
				luci.sys.exec("ip route "..opt.." 0/0 via "..gateway.." table "..table)
				luci.sys.exec("ip rule "..opt.." "..fun.." "..net.." table "..table)
				
				policy_list_del(total_num)
				
				luci.sys.exec("uci set policy.@list[0].number="..(total_num-1))
				ret = 0
			end
			end
		end
		
	end
	
	luci.sys.exec("echo 'gateway="..gateway.." table="..table.." fun="..fun.." net="..net.."' > /tmp/policy")
	
	luci.sys.exec("uci commit policy")
	return_write(ret)
	
end


function nat_config()
local uci = require "luci.model.uci".cursor()
	nat_num = uci.get("nat","nat","nat_num")
	--local add_nat_switch = luci.http.formvalue('value1')
	--local add_nat_default_switch = luci.http.formvalue('value2')
	local add_nat_rule_index = luci.http.formvalue('value1')
	local add_nat_rule_type = luci.http.formvalue('value2')
	local add_nat_rule_src_ip = luci.http.formvalue('value3')
    local add_nat_rule_dst_ip = luci.http.formvalue('value4')
	local action = luci.http.formvalue('value5')
	if '1' == action then 
							if '0' == add_nat_rule_type then 
								luci.sys.exec("iptables -t nat -I POSTROUTING -s "..add_nat_rule_src_ip.." -j SNAT --to "..add_nat_rule_dst_ip)
								luci.sys.exec("uci set nat.nat.srcip_"..(nat_num+1).."="..add_nat_rule_src_ip)
								luci.sys.exec("uci set nat.nat.dstip_"..(nat_num+1).."="..add_nat_rule_dst_ip)
								luci.sys.exec("uci set nat.nat.type_"..(nat_num+1).."="..add_nat_rule_type)
								luci.sys.exec("uci set nat.nat.index_"..(nat_num+1).."="..add_nat_rule_index)
								luci.sys.exec("uci set nat.nat.nat_num="..(nat_num+1))
							else
								luci.sys.exec("iptables -t nat -I PREROUTING -d "..add_nat_rule_src_ip.." -j DNAT --to "..add_nat_rule_dst_ip)
								luci.sys.exec("uci set nat.nat.srcip_"..(nat_num+1).."="..add_nat_rule_src_ip)
								luci.sys.exec("uci set nat.nat.dstip_"..(nat_num+1).."="..add_nat_rule_dst_ip)
								luci.sys.exec("uci set nat.nat.type_"..(nat_num+1).."="..add_nat_rule_type)
								
				
								luci.sys.exec("uci set nat.nat.index_"..(nat_num+1).."="..add_nat_rule_index)
								luci.sys.exec("uci set nat.nat.nat_num="..(nat_num+1))
							end	
			--end
	else
		--luci.sys.exec("itabbles -t nat -F")
		loop_act_black_ip(add_nat_rule_src_ip,add_nat_rule_dst_ip,add_nat_rule_type,nat_num)
		luci.sys.exec("uci set nat.nat.nat_num="..(nat_num-1))
	end

	luci.sys.exec("uci commit nat")
	return_write(0)
end



function loop_act_black_ip(ipaddr,sevpro,sevport,number)

	local loop_r = 0
	
	for num=1,tonumber(number) do

		if tonumber(number) == tonumber(num) then 

			
			luci.sys.exec("uci delete nat.nat.srcip_"..num)
			luci.sys.exec("uci delete nat.nat.dstip_"..num)
			luci.sys.exec("uci delete nat.nat.type_"..num)
			luci.sys.exec("uci delete nat.nat.index_"..num)
			if sevport == '0' then 
				luci.sys.exec("iptables -t nat -D POSTROUTING -s "..ipaddr.." -j SNAT --to "..sevpro)
			else
				luci.sys.exec("iptables -t nat -D PREROUTING -d "..ipaddr.." -j DNAT --to "..sevpro)
			end
			
		else		 

			local ip_str = "srcip_"..num
			local dstip_str = "dstip_"..num
			local type_str = "type_"..num
			local index_str = "index_"..num
			--local str = uci.get("secure","ip",ip_str)
			
			ip_addr = uci.get("nat","nat",ip_str)
			dst_addr = uci.get("nat","nat",dstip_str)
			type_addr = uci.get("nat","nat",type_str)
			index_addr = uci.get("nat","nat",index_str)
			if ip_addr == ipaddr and sevpro == dst_addr and sevport == type_addr or  1 == loop_r then 
				if 0 == loop_r then 
					
					if sevport == '0' then 
						luci.sys.exec("iptables -t nat -D POSTROUTING -s "..ipaddr.." -j SNAT --to "..sevpro)
					else
						luci.sys.exec("iptables -t nat -D PREROUTING -d "..ipaddr.." -j DNAT --to "..sevpro)
					end
					
				end
				
				ip_addr = uci.get("nat","nat","srcip_"..(num+1))
				dst_addr = uci.get("nat","nat","dstip_"..(num+1))
				type_addr = uci.get("nat","nat","type_"..(num+1))
				index_addr = uci.get("nat","nat","index_"..(num+1))

				luci.sys.exec("uci set nat.nat."..ip_str.."="..ip_addr)
				luci.sys.exec("uci set nat.nat."..dstip_str.."="..dst_addr)
				luci.sys.exec("uci set nat.nat."..type_str.."="..type_addr)
				luci.sys.exec("uci set nat.nat."..index_str.."="..index_addr)
				loop_r = 1
			end
			
		end
		
	end		
	
end





function port_switch()

	local uci = require "luci.model.uci".cursor()
	local switch 	= luci.http.formvalue('port_val')
	if  '0' == switch then 
		luci.sys.exec("mirror stop")
		luci.sys.exec("mirror setmirror close")
		luci.sys.exec("uci set config.mirror.mirror_port=close")
		luci.sys.exec("mirror setobserve 1 close")
		luci.sys.exec("uci set config.mirror.lan1_status=close")
		luci.sys.exec("mirror setobserve 2 close")
		luci.sys.exec("uci set config.mirror.lan2_status=close")
		luci.sys.exec("mirror setobserve 3 close")
		luci.sys.exec("uci set config.mirror.lan3_status=close")
		luci.sys.exec("mirror setobserve 4 close")
		luci.sys.exec("uci set config.mirror.lan4_status=close")
	else
		luci.sys.exec("mirror start")
	end
	
	luci.sys.exec("uci set config.mirror.mirror_switch="..switch)
	luci.sys.exec("uci commit config")
	
	return_write(0)
    
end

function port_mirror()

	local uci = require "luci.model.uci".cursor()
	local mirror 	= luci.http.formvalue('port_val')
	
	if '0' == mirror then 
		luci.sys.exec("mirror setmirror close")
		luci.sys.exec("uci set config.mirror.mirror_port=close")
	else
		local lan_num = "lan"..mirror.."_status"
		local port_get = uci.get('config','mirror',lan_num)
		
		if 'close' == port_get then
			luci.sys.exec("mirror setmirror "..mirror)	
			luci.sys.exec("uci set config.mirror.mirror_port="..mirror)
			
		else
			return_write(1)
			return
		end
	end
	
	luci.sys.exec("uci commit config")
	
	return_write(0)
	
end

function port_observe()
	
	local uci = require "luci.model.uci".cursor()
	local observe 	= luci.http.formvalue('port_val')

 	local num = 0
 	local oper 
	for a in string.gmatch(observe, "(%d*);") do
		num = num+1
    	
    	if '0' == a then
    		oper = 'close'
    	elseif '1' == a then
    		oper = 'ingress'
    	elseif '2' == a then
    		oper = 'egress'
    	elseif '3' == a then
    		oper = 'both'
    	end
    	
		luci.sys.exec("mirror setobserve "..num.." "..oper)
		luci.sys.exec("uci set config.mirror.lan"..num.."_status="..oper)
		
	end
	
	luci.sys.exec("uci commit config")
	
	return_write(0)
	
end

function snmpd_switch()
	
	local uci = require "luci.model.uci".cursor()
	local switch = luci.http.formvalue('value1')
	local trap_ip = luci.http.formvalue('value2')
	local status = luci.http.formvalue('value3')
	
	luci.sys.exec("killall -9 mini_snmpd")
	
	luci.sys.exec("uci set config.mini_snmpd.switch="..switch)
	luci.sys.exec("uci set config.mini_snmpd.trap_ip="..trap_ip)
	luci.sys.exec("uci set config.mini_snmpd.status="..status)
	luci.sys.exec("uci commit config")
	
	if '1' == switch  then
		io.popen("/sbin/mini_snmpd &")
	else
		luci.sys.exec("killall -9 mini_snmpd")
	end
	
	return_write(0)
end

function gre_switch()
	local uci = require "luci.model.uci".cursor()
	local switch 	  = luci.http.formvalue('param1')
	local name 	  = luci.http.formvalue('param2')
	local init_src_ip = luci.http.formvalue('param3')
	local init_des_ip = luci.http.formvalue('param4')
	local enca_src_ip = luci.http.formvalue('param5')
	local enca_des_ip = luci.http.formvalue('param6')
	local mtu         = luci.http.formvalue('param7')
	if '1' == switch  then
		luci.sys.exec("uci set config.gre.mtu="..mtu)
		luci.sys.exec("uci set config.gre.pipe_name="..name)
		luci.sys.exec("uci set config.gre.init_src_ip="..init_src_ip)
		luci.sys.exec("uci set config.gre.init_des_ip="..init_des_ip)
		luci.sys.exec("uci set config.gre.enca_src_ip="..enca_src_ip)
		luci.sys.exec("uci set config.gre.enca_des_ip="..enca_des_ip)
		luci.sys.exec("ip tunnel add gre"..name.." remote "..init_des_ip.." local "..init_src_ip.." ttl 255")
		luci.sys.exec("ip link set gre"..name.." up")
		luci.sys.exec("ip addr add "..enca_src_ip.." peer "..enca_des_ip.." dev gre"..name)
			
		luci.sys.exec("ifconfig gre"..name.." mtu "..mtu)
	else
		luci.sys.exec("ip link set gre"..name.." down")
		luci.sys.exec("ip tunnel del gre"..name)
		luci.sys.exec("uci set config.gre.mtu=1476")
		luci.sys.exec("uci set config.gre.pipe_name=gre")
		luci.sys.exec("uci set config.gre.init_src_ip=0.0.0.0")
		luci.sys.exec("uci set config.gre.init_des_ip=0.0.0.0")
		luci.sys.exec("uci set config.gre.enca_src_ip=0.0.0.0")
		luci.sys.exec("uci set config.gre.enca_des_ip=0.0.0.0")
		luci.sys.exec("route del 0.0.0.0")
	end
	luci.sys.exec("uci set config.gre.switch="..switch)
	
	luci.sys.exec("uci commit config")
	
	return_write(0)
end


function log_export()
	local file_name = luci.http.formvalue('file_name')
	local file_oper = luci.http.formvalue('file_oper')
	local file_type = luci.http.formvalue('file_type')
	
	local type
	if '0' == file_type then
		type="logsave"
	end
	if file_name then
		luci.sys.exec("echo '1' > /tmp/log/read")
		if "0" == file_oper then
			--local execmd = "tar zcvf /mnt/emmc/"..type.."/"..file_name..".tar.gz /mnt/emmc/"..type.."/"..file_name
			--luci.sys.exec(execmd)
			luci.http.header('Content-Disposition', 'attachment; filename="%s"' % {file_name})
			luci.http.prepare_content("text/html")
			local ex_file ="/mnt/emmc/"..type.."/"..file_name
			luci.ltn12.pump.all(luci.ltn12.source.file(io.open(ex_file)), luci.http.write)
			--luci.sys.exec("rm -rf "..ex_file)
		else
			luci.sys.exec("rm -rf /mnt/emmc/"..type.."/"..file_name)
			luci.template.render("tau_view/logexport")
		end	
		luci.sys.exec("echo '0' > /tmp/log/read")
	else
		luci.template.render("tau_view/logexport")
	end
	
end
function pack_export()
	local file_name = luci.http.formvalue('file_name')
	local file_oper = luci.http.formvalue('file_oper')
	--local file_export = luci.http.formvalue('packet_export_ip')
	if file_name then
		if "0" == file_oper then
			local execmd = "/mnt/emmc/packet/"..file_name
			--luci.sys.exec(execmd)
			luci.http.header('Content-Disposition', 'attachment; filename="%s"' % {file_name})
			luci.http.prepare_content("application/x-targz")
			luci.http.prepare_content("application/octet-stream")
			local ex_file = "/mnt/emmc/packet/"..file_name
			luci.ltn12.pump.all(luci.ltn12.source.file(io.open(ex_file)), luci.http.write)
			--luci.sys.exec("rm -rf "..ex_file)
			--local execmd = "/sbin/ftpput_sh  /mnt/emmc/packet/ "..file_export.." "..file_name
			--luci.sys.exec(execmd)
			--luci.sys.exec("uci set config.packet.export_ip="..file_export)
			--luci.sys.exec("uci commit config")
			--luci.template.render("tau_view/packexport")
		else
			luci.sys.exec("rm -rf /mnt/emmc/packet/"..file_name)
			luci.template.render("tau_view/packexport")
		end	
	else
		luci.template.render("tau_view/packexport")
	end
end

function web_user()
	local uci = require "luci.model.uci".cursor()
	
	local username 	= luci.http.formvalue('user')
	local userpass 	= luci.http.formvalue('pass')
	local userauth 	= luci.http.formvalue('auth')
	
	if "" == userpass and "" == userauth then 
		local userdelnum = luci.sys.exec("uci get webuser.@user[0].number")
		
		luci.sys.exec("uci delete webuser.@"..username.."[0].username")
		luci.sys.exec("uci delete webuser.@"..username.."[0].password")
		luci.sys.exec("uci delete webuser.@"..username.."[0].authority")
		luci.sys.exec("uci delete webuser.@"..username.."[0]")
		
		web_user_del(userdelnum)
	
		userdelnum=userdelnum-1
		luci.sys.exec("uci set webuser.@user[0].number="..userdelnum)
	else 
		local useraddnum = luci.sys.exec("uci get webuser.@user[0].number")
		useraddnum = useraddnum.gsub(useraddnum,"\n","")+1
	
		luci.sys.exec("uci add webuser user"..useraddnum)
		luci.sys.exec("uci set webuser.@user"..useraddnum.."[0].username="..username)
		luci.sys.exec("uci set webuser.@user"..useraddnum.."[0].password="..userpass)
		luci.sys.exec("uci set webuser.@user"..useraddnum.."[0].authority="..userauth)
		luci.sys.exec("uci set webuser.@user[0].number="..useraddnum)
	end
	luci.sys.exec("uci commit webuser")
	
	return_write(0)
end

function web_user_del(number)


	local next_num=""
	local next_name=""
	local next_pass=""
	local next_auth=""
	
	for num=1,tonumber(number) do
			
		if  luci.sys.exec("uci get webuser.@user"..num.."[0].username") == "" then 
			
			if tonumber(number) == tonumber(num) then 
				luci.sys.exec("uci delete webuser.@user"..number.."[0]")
			else
				next_num=num+1
				next_name=luci.sys.exec("uci get webuser.@user"..next_num.."[0].username")
				next_pass=luci.sys.exec("uci get webuser.@user"..next_num.."[0].password")
				next_auth=luci.sys.exec("uci get webuser.@user"..next_num.."[0].authority")
				
				
				luci.sys.exec("uci add webuser user"..num)
				luci.sys.exec("uci set webuser.@user"..num.."[0].username="..next_name)
				luci.sys.exec("uci set webuser.@user"..num.."[0].password="..next_pass)
				luci.sys.exec("uci set webuser.@user"..num.."[0].authority="..next_auth)
				
				luci.sys.exec("uci delete webuser.@user"..next_num.."[0].username")
				luci.sys.exec("uci delete webuser.@user"..next_num.."[0].password")
				luci.sys.exec("uci delete webuser.@user"..next_num.."[0].authority")
				luci.sys.exec("uci delete webuser.@user"..next_num.."[0]")
			end
		end
		
	end	
		
end

function vrrp_conf_set()

    local uci = require "luci.model.uci".cursor()
    local vrrp_routerid        = luci.http.formvalue('param1')
    local vrrp_virual_routerid = luci.http.formvalue('param2')
    local vrrp_priority        = luci.http.formvalue('param3')
    local vrrp_auth_passwd     = luci.http.formvalue('param4')
    local vrrp_virtual_ip      = luci.http.formvalue('param5')
    local vrrp_auth_sw         = luci.http.formvalue('param6')
    local vrrp_preempt_sw      = luci.http.formvalue('param7')
    local sw                   = luci.http.formvalue('param8')
    local vrrp_master          = luci.http.formvalue('param9')	
    local vrrp_interface       = luci.http.formvalue('param10')
    local vrrp_wan_vip         = luci.http.formvalue('param11')
    local vrrp_track_interface = luci.http.formvalue('param12')
    local vrrr_delay_time      = luci.http.formvalue('param13')
    local vrrp_lan             = luci.http.formvalue('param14')
    local ipv4_addr            = uci.get('network','lan','ipaddr')
    local ipv4_mac             = uci.get('network','lan','netmask')

	if '0' == sw then 
--[[		
	luci.sys.exec("uci set vrrp.global.router_id=0")
	luci.sys.exec("uci set vrrp.instance.mode=0")
	luci.sys.exec("uci set vrrp.instance.master=0")
	luci.sys.exec("uci set vrrp.instance.priority=0")
	luci.sys.exec("uci set vrrp.instance.interface=0")
	luci.sys.exec("uci set vrrp.instance.auth_type=0")
	luci.sys.exec("uci set vrrp.instance.auth_passwd=0")
	luci.sys.exec("uci set vrrp.instance.virtual_routeid=0")
	luci.sys.exec("uci set vrrp.instance.virtual_ip=0.0.0.0")
]]--
	luci.sys.exec("/etc/init.d/keepalived stop")
	luci.sys.exec("killall trackint")
	luci.sys.exec("rm /etc/vrrp.info")
	else
		local libluas = require "libluas".lua_vrrp(vrrp_routerid,vrrp_preempt_sw,vrrp_virual_routerid,vrrp_priority,
			vrrp_auth_sw,vrrp_auth_passwd,vrrp_virtual_ip,vrrp_master,vrrp_interface,vrrp_wan_vip,vrrp_track_interface)

    	if '-1' == libluas then
			return_write(-1)
		end
	luci.sys.exec("uci set vrrp.global.router_id="..vrrp_routerid)
	luci.sys.exec("uci set vrrp.instance.master="..vrrp_master)
	luci.sys.exec("uci set vrrp.instance.mode="..vrrp_preempt_sw)	
	luci.sys.exec("uci set vrrp.instance.priority="..vrrp_priority)
	luci.sys.exec("uci set vrrp.instance.auth_type="..vrrp_auth_sw)
	luci.sys.exec("uci set vrrp.instance.interface="..vrrp_interface)
	luci.sys.exec("uci set vrrp.instance.virtual_ip="..vrrp_virtual_ip)
	luci.sys.exec("uci set vrrp.instance.auth_passwd="..vrrp_auth_passwd)
	luci.sys.exec("uci set vrrp.instance.virtual_routeid="..vrrp_virual_routerid)
	luci.sys.exec("uci set vrrp.instance.wan_ip="..vrrp_wan_vip)
	luci.sys.exec("uci set vrrp.instance.delay_time="..vrrr_delay_time)
	luci.sys.exec("uci set vrrp.instance.track_interface="..vrrp_track_interface)
	luci.sys.exec("uci set vrrp.instance.lan="..vrrp_lan)
	luci.sys.exec("/etc/init.d/keepalived restart")
	luci.sys.exec("killall trackint")
	luci.sys.exec("/sbin/trackint -i "..vrrp_track_interface.." -l "..vrrp_lan.." -t "..vrrr_delay_time.."00  > /dev/null 2>&1 &")

		if vrrp_master == '0' then 
			luci.sys.exec("echo MASTER >> /etc/vrrp.info")
		else
			luci.sys.exec("echo BACKUP >> /etc/vrrp.info")
		end
	end

	luci.sys.exec("ifconfig br-lan "..ipv4_addr.." netmask "..ipv4_mac)
	luci.sys.exec("uci set vrrp.sw.sw="..sw)
	luci.sys.exec("uci commit vrrp")

	return_write(0)	
end

function ospf_config()

	local cmdline = ""
	local config = luci.http.formvalue('value1')
	local interface = luci.http.formvalue('value2')
	local ipaddr = luci.http.formvalue('value3')
	local network = luci.http.formvalue('value4')
	local area = luci.http.formvalue('value5')
	local oper = luci.http.formvalue('value6')
	local ospf_int = luci.http.formvalue('value7')
	local ospf_cost = luci.http.formvalue('value8')

	if 'zebra' == config  then
		if '0' == oper then 
			cmdline = "quacmd zebra add "..interface.." "..ipaddr
		else
			cmdline = "quacmd zebra del "..interface.." "..ipaddr
		end	
		luci.sys.exec(cmdline)
	else
		if '0' == oper then 
			cmdline = "quacmd ospfd add "..ipaddr.." "..network.." "..area.." "..ospf_int.." "..ospf_cost
		else
			cmdline = "quacmd ospfd del "..ipaddr.." "..network.." "..area.." "..ospf_int.." "..ospf_cost
		end	
		luci.sys.exec(cmdline)
	end

	return_write(0)
end

function ospf_sw_config()
	local oper = luci.http.formvalue('value6')
		if '0' == oper then
				luci.sys.exec("uci set config.ospf.ospf_sw="..oper)
				luci.sys.exec("/etc/init.d/quagga stop")
		else
				luci.sys.exec("uci set config.ospf.ospf_sw="..oper)
				luci.sys.exec("/etc/init.d/quagga start")
				luci.sys.exec("/usr/sbin/ospfd -d -f /etc/quagga/ospfd.conf")
		end
		luci.sys.exec("uci commit config")
		return_write(0)
end
function port_map()
	local uci = require "luci.model.uci".cursor()
	
	local port_prot = luci.http.formvalue('value1')
	local port_sip 	= luci.http.formvalue('value2')
	local port_sport= luci.http.formvalue('value3')
	local port_nat 	= luci.http.formvalue('value4')
	local port_dip 	= luci.http.formvalue('value5')
	local port_dport= luci.http.formvalue('value6')
	local port_opr 	= luci.http.formvalue('value7')

	local port_route = ""
	if "0" == port_prot then
		port_prot="tcp"
	else
		port_prot="udp"
	end
	if "0" == port_nat then
		port_nat="SNAT"
		port_route="POSTROUTING"
	else
		port_nat="DNAT"
		port_route="PREROUTING"
	end
		
	if "0" == port_opr then
		local portaddnum = luci.sys.exec("uci get portmap.map.map_num")
		portaddnum = portaddnum.gsub(portaddnum,"\n","")+1
		luci.sys.exec("uci add portmap port"..portaddnum)
		luci.sys.exec("uci set portmap.@port"..portaddnum.."[0].sip="..port_sip)
		luci.sys.exec("uci set portmap.@port"..portaddnum.."[0].sport="..port_sport)
		luci.sys.exec("uci set portmap.@port"..portaddnum.."[0].dip="..port_dip)
		luci.sys.exec("uci set portmap.@port"..portaddnum.."[0].dport="..port_dport)
		luci.sys.exec("uci set portmap.@port"..portaddnum.."[0].map_prot="..port_prot)
		luci.sys.exec("uci set portmap.@port"..portaddnum.."[0].map_trans="..port_nat)
		
		luci.sys.exec("uci set portmap.map.map_num="..portaddnum)
		luci.sys.exec("iptables -t nat -I "..port_route.."  -s "..port_sip.." -p "..port_prot.." --sport "..port_sport.." -j "..port_nat.." --to "..port_dip..":"..port_dport)
	else 
		local portdelnum = luci.sys.exec("uci get portmap.map.map_num")
		portdelnum = portdelnum.gsub(portdelnum,"\n","")
		if "0" == portdelnum then 
			return_write(1)
		else
			portdelnum=port_map_del(portdelnum,port_sip,port_sport,port_dip,port_dport,port_prot,port_nat)
	
			luci.sys.exec("uci set portmap.map.map_num="..portdelnum)
			luci.sys.exec("iptables -t nat -D "..port_route.." -s "..port_sip.." -p "..port_prot.." --sport "..port_sport.." -j "..port_nat.." --to "..port_dip..":"..port_dport)
		end
	end
	luci.sys.exec("uci commit portmap")
	
	return_write(0)
end

function port_map_del(number,sip,sport,dip,dport,prot,nat)

	local next_num=""
	local next_sip=""
	local next_sport=""
	local next_dip=""
	local next_dport=""
	local next_prot=""
	local next_nat=""
	local curr_sip=""
	local curr_sport=""
	local curr_dip=""
	local curr_dport=""
	local curr_prot=""
	local curr_nat=""
	local del_flag=0
	
	for num=1,tonumber(number) do
			
		curr_sip = luci.sys.exec("uci get portmap.@port"..num.."[0].sip")
		curr_sip = curr_sip.gsub(curr_sip,"\n","")
		curr_sport = luci.sys.exec("uci get portmap.@port"..num.."[0].sport")
		curr_sport = curr_sport.gsub(curr_sport,"\n","")
		curr_dip = luci.sys.exec("uci get portmap.@port"..num.."[0].dip")
		curr_dip = curr_dip.gsub(curr_dip,"\n","")
		curr_dport = luci.sys.exec("uci get portmap.@port"..num.."[0].dport")
		curr_dport = curr_dport.gsub(curr_dport,"\n","")
		curr_prot = luci.sys.exec("uci get portmap.@port"..num.."[0].map_prot")
		curr_prot = curr_prot.gsub(curr_prot,"\n","")
		curr_nat = luci.sys.exec("uci get portmap.@port"..num.."[0].map_trans")
		curr_nat = curr_nat.gsub(curr_nat,"\n","")
		
		if  curr_sip==sip and curr_sport==sport and curr_dip==dip and curr_dport==dport and curr_prot==prot and curr_nat==nat
		then 
			luci.sys.exec("uci delete portmap.@port"..num.."[0].sip")
			luci.sys.exec("uci delete portmap.@port"..num.."[0].sport")
			luci.sys.exec("uci delete portmap.@port"..num.."[0].dip")
			luci.sys.exec("uci delete portmap.@port"..num.."[0].dport")
			luci.sys.exec("uci delete portmap.@port"..num.."[0].map_prot")
			luci.sys.exec("uci delete portmap.@port"..num.."[0].map_trans")
			luci.sys.exec("uci delete portmap.@port"..num.."[0]")
			del_flag=1
		end
			
		if tonumber(number) == tonumber(num) then 
			if del_flag==1 then
				number=number-1
			else 
				number=number
			end	
		else
			if  luci.sys.exec("uci get portmap.@port"..num.."[0].sip") == "" then 
				next_num=num+1
				next_sip=luci.sys.exec("uci get portmap.@port"..next_num.."[0].sip")
				next_sport=luci.sys.exec("uci get portmap.@port"..next_num.."[0].sport")
				next_dip=luci.sys.exec("uci get portmap.@port"..next_num.."[0].dip")
				next_dport=luci.sys.exec("uci get portmap.@port"..next_num.."[0].dport")
				next_prot=luci.sys.exec("uci get portmap.@port"..next_num.."[0].map_prot")
				next_nat=luci.sys.exec("uci get portmap.@port"..next_num.."[0].map_trans")
				
				luci.sys.exec("uci add portmap port"..num)
				luci.sys.exec("uci set portmap.@port"..num.."[0].sip="..next_sip)
				luci.sys.exec("uci set portmap.@port"..num.."[0].sport="..next_sport)
				luci.sys.exec("uci set portmap.@port"..num.."[0].dip="..next_dip)
				luci.sys.exec("uci set portmap.@port"..num.."[0].dport="..next_dport)
				luci.sys.exec("uci set portmap.@port"..num.."[0].map_prot="..next_prot)
				luci.sys.exec("uci set portmap.@port"..num.."[0].map_trans="..next_nat)
				
				luci.sys.exec("uci delete portmap.@port"..next_num.."[0].sip")
				luci.sys.exec("uci delete portmap.@port"..next_num.."[0].sport")
				luci.sys.exec("uci delete portmap.@port"..next_num.."[0].dip")
				luci.sys.exec("uci delete portmap.@port"..next_num.."[0].dport")
				luci.sys.exec("uci delete portmap.@port"..next_num.."[0].map_prot")
				luci.sys.exec("uci delete portmap.@port"..next_num.."[0].map_trans")
				luci.sys.exec("uci delete portmap.@port"..next_num.."[0]")
			end	
		end			

	end	
	
	return number

end

function ipsec_config()
	local ipsec_sw  = luci.http.formvalue('param1')
	
	if ipsec_sw == '0' then
		luci.sys.exec("/etc/init.d/ipsec stop ")
		luci.sys.exec("uci set ipsec.config.ipsec_sw="..ipsec_sw)
	else 
		luci.sys.exec("uci set ipsec.config.ipsec_sw="..ipsec_sw)
		luci.sys.exec("/etc/init.d/ipsec restart ")
	end
	
	luci.sys.exec("uci commit ipsec")
	
	return_write(0)
	
end

function action_ipsec_status()

	local buff = tostring(luci.sys.exec("ipsec whack --status |grep  -s 'ISAKMP SA established'|awk '{print$5}' |head -n 1 "))
	local data = tostring(luci.sys.exec("ipsec whack --status |grep  -s 'ISAKMP SA established'|awk '{print$5}' |tail -n 1 "))
	if buff  == "(ISAKMP\n" or data  == "(ISAKMP\n" then
		luci.http.write("connect")
	else
		luci.http.write("disconnect")
	end

end

function psk_auth()

	local uci = require "luci.model.uci".cursor()
	local psk_select_sw    = luci.http.formvalue('param1')
--	local psk_authby_sw    = luci.http.formvalue('param2')
	local psk_pfs_sw       = luci.http.formvalue('param3')
	local psk_ike1_sw      = luci.http.formvalue('param4')
	local psk_ike2_sw      = luci.http.formvalue('param5')
	local psk_phase_sw     = luci.http.formvalue('param6')
	local psk_phasealg1_sw = luci.http.formvalue('param7')
	local psk_phasealg2_sw = luci.http.formvalue('param8')
	local psk_ikelifetime  = luci.http.formvalue('param9')
	local psk_salifetime   = luci.http.formvalue('param10')
	local psk_psk          = luci.http.formvalue('param11')
	local psk_src_ip       = luci.http.formvalue('param12')
	local psk_src_mask     = luci.http.formvalue('param13')
	local psk_src_subnet   = luci.http.formvalue('param14')
	local psk_src_nexthop  = luci.http.formvalue('param15')
	local psk_des_ip       = luci.http.formvalue('param16')
	local psk_des_mask     = luci.http.formvalue('param17')
	local psk_des_subnet   = luci.http.formvalue('param18')
	local psk_des_nexthop  = luci.http.formvalue('param19')

	if psk_select_sw == '0' then
		luci.sys.exec("uci set ipsec.psk.sw=0")
		luci.sys.exec("uci set ipsec.psk.pfs=0")
		luci.sys.exec("uci set ipsec.psk.psk=0")
		luci.sys.exec("uci set ipsec.psk.ike1=0")
		luci.sys.exec("uci set ipsec.psk.ike2=0")
		luci.sys.exec("uci set ipsec.psk.phase2=0")
		luci.sys.exec("uci set ipsec.psk.phase2alg1=0")
		luci.sys.exec("uci set ipsec.psk.phase2alg2=0")
		luci.sys.exec("uci set ipsec.psk.salifetime=8")
		luci.sys.exec("uci set ipsec.psk.ikelifetime=1")
		luci.sys.exec("uci set ipsec.psk.src_ip=0.0.0.0")
		luci.sys.exec("uci set ipsec.psk.des_ip=0.0.0.0")
		luci.sys.exec("uci set ipsec.psk.des_subnet=0.0.0.0")
		luci.sys.exec("uci set ipsec.psk.src_subnet=0.0.0.0")
		luci.sys.exec("uci set ipsec.psk.des_nexthop=0.0.0.0")
		luci.sys.exec("uci set ipsec.psk.src_nexthop=0.0.0.0")
		luci.sys.exec("uci set ipsec.psk.des_mask=24")
		luci.sys.exec("uci set ipsec.psk.src_mask=24")
		luci.sys.exec("ipsec setup stop")
	else

		local libluas = require "libluas".lua_ipsec(0,psk_pfs_sw,psk_ike1_sw,psk_ike2_sw,psk_phase_sw,psk_phasealg1_sw,psk_phasealg2_sw,psk_ikelifetime,psk_salifetime,
			psk_psk,psk_src_ip,psk_src_mask,psk_src_subnet,psk_src_nexthop,psk_des_ip,psk_des_mask,psk_des_subnet,psk_des_nexthop)
    	if '-1' == libluas then
			return_write(-1)
	 	end
   
	    luci.sys.exec("uci set ipsec.psk.sw="..psk_select_sw)
		luci.sys.exec("uci set ipsec.psk.pfs="..psk_pfs_sw)
		luci.sys.exec("uci set ipsec.psk.psk="..psk_psk)
		luci.sys.exec("uci set ipsec.psk.ike1="..psk_ike1_sw)
		luci.sys.exec("uci set ipsec.psk.ike2="..psk_ike2_sw)
		luci.sys.exec("uci set ipsec.psk.phase2="..psk_phase_sw)
		luci.sys.exec("uci set ipsec.psk.phase2alg1="..psk_phasealg1_sw)
		luci.sys.exec("uci set ipsec.psk.phase2alg2="..psk_phasealg2_sw)
		luci.sys.exec("uci set ipsec.psk.salifetime="..psk_salifetime)
		luci.sys.exec("uci set ipsec.psk.ikelifetime="..psk_ikelifetime)
		luci.sys.exec("uci set ipsec.psk.src_ip="..psk_src_ip)
		luci.sys.exec("uci set ipsec.psk.des_ip="..psk_des_ip)
		luci.sys.exec("uci set ipsec.psk.des_subnet="..psk_des_subnet)
		luci.sys.exec("uci set ipsec.psk.src_subnet="..psk_src_subnet)
		luci.sys.exec("uci set ipsec.psk.des_nexthop="..psk_des_nexthop)
		luci.sys.exec("uci set ipsec.psk.src_nexthop="..psk_src_nexthop)
		luci.sys.exec("uci set ipsec.psk.des_mask="..psk_des_mask)
		luci.sys.exec("uci set ipsec.psk.src_mask="..psk_src_mask)
		luci.sys.exec("usleep 100000")
		luci.sys.exec("ipsec setup restart")
		luci.sys.exec("usleep 100000")
		luci.sys.exec("ipsec auto --up net-to-net > /dev/null 2>&1 &")

	end

	luci.sys.exec("uci set ipsec.psk.sw="..psk_select_sw)
	luci.sys.exec("uci commit ipsec")

	return_write(0)
end

function rsa_auth()

	local uci = require "luci.model.uci".cursor()

	local rsa_select_sw    = luci.http.formvalue('param1')
	local rsa_remote_id    = luci.http.formvalue('param2')
	local rsa_pfs_sw       = luci.http.formvalue('param3')
	local rsa_ike1_sw      = luci.http.formvalue('param4')
	local rsa_ike2_sw      = luci.http.formvalue('param5')
	local rsa_phase_sw     = luci.http.formvalue('param6')
	local rsa_phasealg1_sw = luci.http.formvalue('param7')
	local rsa_phasealg2_sw = luci.http.formvalue('param8')
	local rsa_ikelifetime  = luci.http.formvalue('param9')
	local rsa_salifetime   = luci.http.formvalue('param10')
	local rsa_local_id     = luci.http.formvalue('param11')
	local rsa_src_ip       = luci.http.formvalue('param12')
	local rsa_src_mask     = luci.http.formvalue('param13')
	local rsa_src_subnet   = luci.http.formvalue('param14')
	local rsa_src_nexthop  = luci.http.formvalue('param15')
	local rsa_des_ip       = luci.http.formvalue('param16')
	local rsa_des_mask     = luci.http.formvalue('param17')
	local rsa_des_subnet   = luci.http.formvalue('param18')
	local rsa_des_nexthop  = luci.http.formvalue('param19')
	local rsa_remote_key   = luci.http.formvalue('param20')

	if rsa_remote_id == '1' then
	luci.sys.exec("uci set ipsec.rsa.flag=true")
	luci.sys.exec("ipsec newhostkey --output /etc/ipsec.secrets --random /dev/urandom")
	luci.sys.exec("usleep 100000")
	luci.sys.exec("ipsec showhostkey --left >> /etc/ipsec.conf")
	end

	if rsa_select_sw == '0' then
		luci.sys.exec("uci set ipsec.rsa.sw=0")
		luci.sys.exec("uci set ipsec.rsa.pfs=0")
		luci.sys.exec("uci set ipsec.rsa.ike1=0")
		luci.sys.exec("uci set ipsec.rsa.ike2=0")
		luci.sys.exec("uci set ipsec.rsa.phase2=0")
		luci.sys.exec("uci set ipsec.rsa.phase2alg1=0")
		luci.sys.exec("uci set ipsec.rsa.phase2alg2=0")
		luci.sys.exec("uci set ipsec.rsa.salifetime=8")
		luci.sys.exec("uci set ipsec.rsa.ikelifetime=1")
		luci.sys.exec("uci set ipsec.rsa.src_ip=0.0.0.0")
		luci.sys.exec("uci set ipsec.rsa.des_ip=0.0.0.0")
		luci.sys.exec("uci set ipsec.rsa.des_subnet=0.0.0.0")
		luci.sys.exec("uci set ipsec.rsa.src_subnet=0.0.0.0")
		luci.sys.exec("uci set ipsec.rsa.des_nexthop=0.0.0.0")
		luci.sys.exec("uci set ipsec.rsa.src_nexthop=0.0.0.0")
		luci.sys.exec("uci set ipsec.rsa.des_mask=24")
		luci.sys.exec("uci set ipsec.rsa.src_mask=24")
		luci.sys.exec("uci set ipsec.rsa.local_id=@A")
		luci.sys.exec("uci set ipsec.rsa.remote_id=@B")
		luci.sys.exec("uci set ipsec.rsa.remote_key=XXXX")
		luci.sys.exec("uci set ipsec.rsa.flag=false")
		luci.sys.exec("ipsec setup stop")
	else

		local libluas = require "libluas".lua_ipsec(1,rsa_pfs_sw,rsa_ike1_sw,rsa_ike2_sw,rsa_phase_sw,
			rsa_phasealg1_sw,rsa_phasealg2_sw,rsa_ikelifetime,rsa_salifetime,
			rsa_src_ip,rsa_src_mask,rsa_src_subnet,rsa_src_nexthop,rsa_des_ip,rsa_des_mask,
			rsa_des_subnet,rsa_des_nexthop,rsa_local_id,rsa_remote_id,rsa_remote_key)

    		if '-1' == libluas then
				return_write(-1)
	 		end
  
	    luci.sys.exec("uci set ipsec.rsa.sw="..rsa_select_sw)
		luci.sys.exec("uci set ipsec.rsa.pfs="..rsa_pfs_sw)
		luci.sys.exec("uci set ipsec.rsa.ike1="..rsa_ike1_sw)
		luci.sys.exec("uci set ipsec.rsa.ike2="..rsa_ike2_sw)
		luci.sys.exec("uci set ipsec.rsa.phase2="..rsa_phase_sw)
		luci.sys.exec("uci set ipsec.rsa.phase2alg1="..rsa_phasealg1_sw)
		luci.sys.exec("uci set ipsec.rsa.phase2alg2="..rsa_phasealg2_sw)
		luci.sys.exec("uci set ipsec.rsa.salifetime="..rsa_salifetime)
		luci.sys.exec("uci set ipsec.rsa.ikelifetime="..rsa_ikelifetime)
		luci.sys.exec("uci set ipsec.rsa.src_ip="..rsa_src_ip)
		luci.sys.exec("uci set ipsec.rsa.des_ip="..rsa_des_ip)
		luci.sys.exec("uci set ipsec.rsa.des_subnet="..rsa_des_subnet)
		luci.sys.exec("uci set ipsec.rsa.src_subnet="..rsa_src_subnet)
		luci.sys.exec("uci set ipsec.rsa.des_nexthop="..rsa_des_nexthop)
		luci.sys.exec("uci set ipsec.rsa.src_nexthop="..rsa_src_nexthop)
		luci.sys.exec("uci set ipsec.rsa.des_mask="..rsa_des_mask)
		luci.sys.exec("uci set ipsec.rsa.src_mask="..rsa_src_mask)
		luci.sys.exec("uci set ipsec.rsa.remote_id="..rsa_remote_id)
		luci.sys.exec("uci set ipsec.rsa.remote_key="..rsa_remote_key)
		luci.sys.exec("uci sec ipsec.rsa.local_id="..rsa_local_id)
		luci.sys.exec("uci set ipsec.rsa.flag=false")
		luci.sys.exec("ipsec showhostkey --left >> /etc/ipsec.conf")
		luci.sys.exec("usleep 100000")
		luci.sys.exec("ipsec setup restart")
		luci.sys.exec("usleep 100000")
		luci.sys.exec("ipsec auto --up net-to-net > /dev/null 2>&1 &")

	end
	luci.sys.exec("uci set ipsec.rsa.sw="..rsa_select_sw)
	luci.sys.exec("uci commit ipsec")

	return_write(0)
end

function x509_auth()

	local x509_select_sw    = luci.http.formvalue('param1')
	local x509_pfs_sw       = luci.http.formvalue('param2')
	local x509_passwd       = luci.http.formvalue('param3')
	local x509_ikelifetime  = luci.http.formvalue('param4')
	local x509_src_ip       = luci.http.formvalue('param5')
	local x509_src_mask     = luci.http.formvalue('param6')
	local x509_src_subnet   = luci.http.formvalue('param7')
	local x509_salifetime   = luci.http.formvalue('param8')
	local x509_des_ip       = luci.http.formvalue('param9')
	local x509_des_mask     = luci.http.formvalue('param10')
	local x509_des_subnet   = luci.http.formvalue('param11')
	local x509_ike1_sw      = luci.http.formvalue('param12')
	local x509_ike2_sw      = luci.http.formvalue('param13')
	local x509_phase_sw     = luci.http.formvalue('param14')
	local x509_phasealg1_sw = luci.http.formvalue('param15')
	local x509_phasealg2_sw = luci.http.formvalue('param16')
	local x509_src_nexthop  = luci.http.formvalue('param17')
	local x509_des_nexthop  = luci.http.formvalue('param18')
	local X509_remote_id    = luci.http.formvalue('param19')
	local X509_local_cert   = uci.get('ipsec','x509','local_cert')
	local X509_remote_cert  = uci.get('ipsec','x509','remote_cert')
	local X509_private_key  = uci.get('ipsec','x509','private_key')

	local fd_local_cert   = nil
	local fd_private_key  = nil 
	local fd_remote_cert  = nil
		
	if x509_select_sw == '0' then
		luci.sys.exec("uci set ipsec.x509.pfs=0")
		luci.sys.exec("uci set ipsec.x509.passwd=xxxx")
		luci.sys.exec("uci set ipsec.x509.src_mask=24")
		luci.sys.exec("uci set ipsec.x509.des_mask=24")
		luci.sys.exec("uci set ipsec.x509.src_ip=0.0.0.0")
		luci.sys.exec("uci set ipsec.x509.des_ip=0.0.0.0")
		luci.sys.exec("uci set ipsec.x509.src_subnet=0.0.0.0")
		luci.sys.exec("uci set ipsec.x509.des_subnet=0.0.0.0")
		luci.sys.exec("uci set ipsec.x509.ikelifetime=1")
		luci.sys.exec("uci set ipsec.x509.salifetime=8")
		luci.sys.exec("uci set ipsec.x509.ike1=0")
		luci.sys.exec("uci set ipsec.x509.ike2=0")
		luci.sys.exec("uci set ipsec.x509.phase2=0")
		luci.sys.exec("uci set ipsec.x509.phase2alg1=0")
		luci.sys.exec("uci set ipsec.x509.phase2alg2=0")
		luci.sys.exec("uci set ipsec.x509.src_nexthop=0.0.0.0")
		luci.sys.exec("uci set ipsec.x509.des_nexthop=0.0.0.0")
		luci.sys.exec("uci set ipsec.x509.remote_id=@A")
		luci.sys.exec("ipsec setup stop")
		luci.sys.exec("rm /etc/ipsec.d/cacerts/*")
		luci.sys.exec("rm /etc/ipsec.d/certs/*")
		luci.sys.exec("rm /etc/ipsec.d/private/*")
		luci.sys.exec("rm /etc/ipsec.d/crls/*")
	else	

		fd_local_cert = io.open("/tmp/"..X509_local_cert, "r")
   		if fd_local_cert then
        	luci.sys.exec("mv /tmp/"..X509_local_cert.." /etc/ipsec.d/certs/")   
		end	
		
		fd_remote_cert = io.open("/tmp/"..X509_remote_cert, "r")
    	if fd_remote_cert then
    	luci.sys.exec("mv /tmp/"..X509_remote_cert.." /etc/ipsec.d/certs/") 
    	end
    		
    	fd_private_key = io.open("/tmp/"..X509_private_key, "r")
    	if fd_private_key then
    	    luci.sys.exec("mv /tmp/"..X509_private_key.." /etc/ipsec.d/private/") 
    	end
	
		local libluas = require "libluas".lua_ipsec(2,x509_pfs_sw,x509_passwd,X509_local_cert,x509_src_ip,x509_src_mask,x509_src_subnet,X509_remote_cert,
									x509_des_ip,x509_des_mask,x509_des_subnet,X509_private_key,x509_ikelifetime,x509_salifetime,x509_ike1_sw,x509_ike2_sw,
									x509_phasealg1_sw,x509_phasealg2_sw,x509_phase_sw,x509_src_nexthop,x509_des_nexthop,X509_remote_id)

    		if '-1' == libluas then
				return_write(-1)
	 		end

		luci.sys.exec("uci set ipsec.x509.pfs="..x509_pfs_sw)
		luci.sys.exec("uci set ipsec.x509.passwd="..x509_passwd)	
		luci.sys.exec("uci set ipsec.x509.src_ip="..x509_src_ip)
		luci.sys.exec("uci set ipsec.x509.des_ip="..x509_des_ip)
		luci.sys.exec("uci set ipsec.x509.des_mask="..x509_des_mask)
		luci.sys.exec("uci set ipsec.x509.src_mask="..x509_src_mask)
		luci.sys.exec("uci set ipsec.x509.src_subnet="..x509_src_subnet)
		luci.sys.exec("uci set ipsec.x509.des_subnet="..x509_des_subnet)
		luci.sys.exec("uci set ipsec.x509.ikelifetime="..x509_ikelifetime)
		luci.sys.exec("uci set ipsec.x509.salifetime="..x509_salifetime)
		luci.sys.exec("uci set ipsec.x509.ike1="..x509_ike1_sw)
		luci.sys.exec("uci set ipsec.x509.ike2="..x509_ike2_sw)
		luci.sys.exec("uci set ipsec.x509.phase2="..x509_phase_sw)
		luci.sys.exec("uci set ipsec.x509.phase2alg1="..x509_phasealg1_sw)
		luci.sys.exec("uci set ipsec.x509.phase2alg2="..x509_phasealg2_sw)
		luci.sys.exec("uci set ipsec.x509.src_nexthop="..x509_src_nexthop)
		luci.sys.exec("uci set ipsec.x509.des_nexthop="..x509_des_nexthop)
		luci.sys.exec("uci set ipsec.x509.remote_id="..X509_remote_id)
		luci.sys.exec("ipsec setup restart")
		luci.sys.exec("usleep 100000")
		luci.sys.exec("ipsec auto --up net-to-net > /dev/null 2>&1 &")

	end

	luci.sys.exec("uci set ipsec.x509.sw="..x509_select_sw)
	luci.sys.exec("uci commit ipsec")

	return_write(0)
	
end

function cmd_line()
	local uci = require "luci.model.uci".cursor()
	local command=luci.http.formvalue('value1')
	local cmdline=luci.util.exec(tostring(command))
	
	local o = {}
	o['result'] = cmdline
	o['data'] = "ok"
	
	luci.http.prepare_content("application/json")
    luci.http.write_json(o)
end

function time_synchronization()
	local uci = require "luci.model.uci".cursor()
	local select_sw = luci.http.formvalue('param1')
	local primary = luci.http.formvalue('param2')
	local standby = luci.http.formvalue('param3')
    local syntime = luci.http.formvalue('param4')
	local server_port = luci.http.formvalue('param5')
	luci.sys.exec("killall -9 ntpctl")
	luci.sys.exec("killall -9 ntpclient")
	
	if select_sw  == "0" then	
		luci.sys.exec("/sbin/ntpctl -i "..syntime.." -p "..server_port.." -f "..primary.." -s "..standby.."  > /dev/null 2>&1 &")
		luci.sys.exec("uci set config.time_synchronization.time_synchronization_sw="..select_sw)
		luci.sys.exec("uci set config.time_synchronization.primary_server="..primary)
		luci.sys.exec("uci set config.time_synchronization.standby_server="..standby)
		luci.sys.exec("uci set config.time_synchronization.syntime="..syntime)
		luci.sys.exec("uci set config.time_synchronization.server_port="..server_port)
		
		
		
	else
		luci.sys.exec("killall -9 ntpctl")
		luci.sys.exec("killall -9 ntpclient")
		luci.sys.exec("uci set config.time_synchronization.time_synchronization_sw="..select_sw)
	end
	
	luci.sys.exec("uci commit config")

	return_write(0)
end

function muticast_set()

	local uci = require "luci.model.uci".cursor()
	local muticast_sw   = luci.http.formvalue('param1')
	local bsr_priority  = luci.http.formvalue('param2')
--	local rp_sw         = luci.http.formvalue('param3')
	local interface_sw  = luci.http.formvalue('param3')
	local rp_time       = luci.http.formvalue('param5')
	local rp_priority   = luci.http.formvalue('param6')
	local rp_addr       = luci.http.formvalue('param7')
--	local rp_group_addr = luci.http.formvalue('param8')
--	local rp_group_mask = luci.http.formvalue('param9')
	local dr_interface = luci.http.formvalue('param10')
	local dr_priority = luci.http.formvalue('param11')
	local route_ip = luci.http.formvalue('param12')
	local route_int = luci.http.formvalue('param13')
	
	if muticast_sw  == "1" then	

		if rp_sw == "1" then
			luci.sys.exec("ifconfig lo "..rp_addr)
		else
			luci.sys.exec("ifconfig lo 127.0.0.1")
		end

		luci.sys.exec("uci set config.muticast.group_ip="..route_ip)
		luci.sys.exec("uci set config.muticast.group_mask="..route_int)
--		luci.sys.exec("uci set config.muticast.rp_group_mask="..rp_group_mask)
--		luci.sys.exec("uci set config.muticast.rp_group_addr="..rp_group_addr)
		luci.sys.exec("uci set config.muticast.bsr_priority="..bsr_priority)
		luci.sys.exec("uci set config.muticast.rp_priority="..rp_priority)
		luci.sys.exec("uci set config.muticast.rp_time="..rp_time)
		luci.sys.exec("uci set config.muticast.rp_addr="..rp_addr)
		luci.sys.exec("uci set config.muticast.interface_sw="..interface_sw)
		luci.sys.exec("uci set config.muticast.dr_interface="..dr_interface)
		luci.sys.exec("uci set config.muticast.dr_priority="..dr_priority)
		
--		if interface_sw == "0" then
--			luci.sys.exec("route add 224.0.0.0 dev br-lan")
--		else
--			luci.sys.exec("route add 224.0.0.0 dev usb0")
--		end
		luci.sys.exec("route add "..route_ip.." dev "..route_int)

		if dr_interface == "" then
			luci.sys.exec("sed -i '119s:.*:#phyint "..dr_interface.." dr-priority "..dr_priority..":g' /etc/pimd.conf")
		else
			luci.sys.exec("sed -i '119s:.*:phyint "..dr_interface.." dr-priority "..dr_priority..":g' /etc/pimd.conf")
		end
		luci.sys.exec("usleep 100000")
		luci.sys.exec("sed -i '129s:.*:bsr-candidate priority "..bsr_priority..":g' /etc/pimd.conf")
		luci.sys.exec("usleep 100000")
		luci.sys.exec("sed -i '132s:.*:rp-candidate time "..rp_time.." priority "..rp_priority..":g' /etc/pimd.conf")
		luci.sys.exec("usleep 100000")
		luci.sys.exec("sed -i '135s:.*:group-prefix 224.0.0.0 masklen 4:g' /etc/pimd.conf")
		luci.sys.exec("usleep 100000")
		luci.sys.exec("sed -i '138s:.*:rp-address "..rp_addr.." 224.0.0.0/4:g' /etc/pimd.conf")
--		luci.sys.exec("sed -i '135s:.*:group-prefix "..group_ip.." masklen "..group_mask..":g' /etc/pimd.conf")
--		luci.sys.exec("usleep 100000")
--		luci.sys.exec("sed -i '138s:.*:rp-address "..rp_addr.." "..rp_group_addr.."/"..rp_group_mask..":g' /etc/pimd.conf")
		luci.sys.exec("usleep 100000")
		
		luci.sys.exec("killall pimd")
		luci.sys.exec("killall udpxy")
		
		luci.sys.exec("/sbin/pimd &")
		luci.sys.exec("/etc/init.d/igmpproxy start")
		luci.sys.exec("udpxy -p 8080 -a "..dr_interface.." > /dev/null 2>&1 &")
		
	else
		luci.sys.exec("uci set config.muticast.rp_sw=0")
--		luci.sys.exec("route del 224.0.0.0")
		luci.sys.exec("route del "..route_ip)
--		luci.sys.exec("ifconfig lo 127.0.0.1")
		
		
		luci.sys.exec("killall udpxy")
		luci.sys.exec("killall pimd")
	end

	luci.sys.exec("uci set config.muticast.muticast_sw="..muticast_sw)
	luci.sys.exec("uci commit config")

	return_write(0)
end

function attenuator_set()
	local select_sw = luci.http.formvalue('param1')
	local bit_0     = luci.http.formvalue('param2')
	local bit_1     = luci.http.formvalue('param3')
	local bit_2     = luci.http.formvalue('param4')
	local bit_3     = luci.http.formvalue('param5')
	local bit_4     = luci.http.formvalue('param6')
	local bit_5     = luci.http.formvalue('param7')
	local attenuation_strength = luci.http.formvalue('param8')

	if select_sw == '1' then
		luci.sys.exec("uci set config.attenuator.bit_0=1")
		luci.sys.exec("uci set config.attenuator.bit_1="..bit_1)
		luci.sys.exec("uci set config.attenuator.bit_2="..bit_2)
		luci.sys.exec("uci set config.attenuator.bit_3="..bit_3)
		luci.sys.exec("uci set config.attenuator.bit_4="..bit_4)
		luci.sys.exec("uci set config.attenuator.bit_5=1")
		luci.sys.exec("uci set config.attenuator.attenuation_strength="..attenuation_strength)
	else

		luci.sys.exec("uci set config.attenuator.select_sw=0")
		luci.sys.exec("uci set config.attenuator.bit_0=1")
		luci.sys.exec("uci set config.attenuator.bit_1=1")
		luci.sys.exec("uci set config.attenuator.bit_2=1")
		luci.sys.exec("uci set config.attenuator.bit_3=1")
		luci.sys.exec("uci set config.attenuator.bit_4=1")
		luci.sys.exec("uci set config.attenuator.bit_5=1")
		luci.sys.exec("uci set config.attenuator.attenuation_strength=0")
	end
	luci.sys.exec("echo 39 > /sys/class/gpio/export")
	luci.sys.exec("echo 40 > /sys/class/gpio/export")
	luci.sys.exec("echo 41 > /sys/class/gpio/export")
	luci.sys.exec("echo 42 > /sys/class/gpio/export")
	luci.sys.exec("echo 43 > /sys/class/gpio/export")
	luci.sys.exec("echo 44 > /sys/class/gpio/export")
	luci.sys.exec("usleep 100000")
	luci.sys.exec("echo out >/sys/class/gpio/gpio39/direction")
	luci.sys.exec("echo out >/sys/class/gpio/gpio40/direction")
	luci.sys.exec("echo out >/sys/class/gpio/gpio41/direction")
	luci.sys.exec("echo out >/sys/class/gpio/gpio42/direction")
	luci.sys.exec("echo out >/sys/class/gpio/gpio43/direction")
	luci.sys.exec("echo out >/sys/class/gpio/gpio44/direction")
	luci.sys.exec("usleep 100000")
	luci.sys.exec("echo "..bit_0.." >/sys/class/gpio/gpio39/value")
	luci.sys.exec("echo "..bit_1.." >/sys/class/gpio/gpio40/value")
	luci.sys.exec("echo "..bit_2.." >/sys/class/gpio/gpio41/value")
	luci.sys.exec("echo "..bit_3.." >/sys/class/gpio/gpio42/value")
	luci.sys.exec("echo "..bit_4.." >/sys/class/gpio/gpio43/value")
	luci.sys.exec("echo "..bit_5.." >/sys/class/gpio/gpio44/value")
	luci.sys.exec("usleep 100000")
	luci.sys.exec("uci set config.attenuator.select_sw="..select_sw)
	luci.sys.exec("uci commit config")

 	return_write(0)
end

function action_clock_status()
	local settime   = luci.http.formvalue("value1")
	
	luci.sys.exec("date -s '"..settime.."' ")
	
	return_write(0);
end


--[[function pack_export()
	local file_name = luci.http.formvalue('file_name')
	local file_oper = luci.http.formvalue('file_oper')
	
	local file_export = luci.http.formvalue('packet_export_ip')
	
	if file_name then

		if "0" == file_oper then
			--local execmd = "tar zcvf /mnt/emmc/packet/"..file_name..".tar.gz /mnt/emmc/packet/"..file_name
			--luci.sys.exec(execmd)
			--luci.http.header('Content-Disposition', 'attachment; filename="%s"' % {file_name})
			--luci.http.prepare_content("application/octet-stream")
			--local ex_file = "/mnt/emmc/packet/"..file_name
			--luci.ltn12.pump.all(luci.ltn12.source.file(io.open(ex_file)), luci.http.write)
			--luci.sys.exec("rm -rf "..ex_file)
			
			local execmd = "/sbin/ftpput_sh  /mnt/emmc/packet/ "..file_export.." "..file_name
			luci.sys.exec(execmd)
			
			luci.sys.exec("uci set config.packet.export_ip="..file_export)
			luci.sys.exec("uci commit config")
			
			luci.template.render("tau_view/packexport")
			
		else
			luci.sys.exec("rm -rf /mnt/emmc/packet/"..file_name)
			luci.template.render("tau_view/packexport")
		end	
		
	else
		luci.template.render("tau_view/packexport")
	end
	
end --]]
function pack_config()
	
	local filter_rule = luci.http.formvalue('value1')
	local filter_ip = luci.http.formvalue('value2')
	local remote_sw = luci.http.formvalue('value3')
	local remote_ip = luci.http.formvalue('value4')
	local packet_sw = luci.http.formvalue('value5')
	local lan_ip = luci.http.formvalue('value6')
	local wan_ip = luci.http.formvalue('value7')
	local packet_time = luci.http.formvalue('value8')
	local packet_size = luci.http.formvalue('value9')
	local packet_int = luci.http.formvalue('value10')
	
	if "0" == packet_sw then
		luci.sys.exec("killall -9 cache_cle_sh")
		luci.sys.exec("killall -9 ethdump")
		luci.sys.exec("killall -9 ethftpput")
	else
		local remote_cmd
		local filter_cmd
		local ethdump_cmd_l
		local ethdump_cmd_w
		
		luci.sys.exec("killall -9 cache_cle_sh")
		luci.sys.exec("killall -9 ethdump")
		luci.sys.exec("killall -9 ethftpput")
		
		if "0" == filter_rule then
			filter_cmd = " "
		elseif "1" == filter_rule then
			filter_cmd = " -d "..filter_ip
		elseif "2" == filter_rule then
			filter_cmd = " -s "..filter_ip
		end	
		
		--if "0" == remote_sw then
		--	remote_cmd = " "
		--else
		--	remote_cmd = " -r "..remote_ip
		--end	
		--luci.sys.exec("/sbin/ethdump "..filter_cmd.." "..remote_cmd.." -l "..lan_ip.." -w "..wan_ip.." -t "..packet_time.." -f "..packet_size.."  > /dev/null 2>&1 &")
		luci.sys.exec("/sbin/ethdump "..filter_cmd.." -l "..lan_ip.." -w "..wan_ip.." -t "..packet_time.." -f "..packet_size.." -i "..packet_int.." > /dev/null 2>&1 &")
		--luci.sys.exec("/sbin/cache_cle_sh > /dev/null 2>&1 &")
		luci.sys.exec("/sbin/cache > /dev/null 2>&1 &")
	end
	
	luci.sys.exec("uci set config.packet.filter_sw="..filter_rule)
	luci.sys.exec("uci set config.packet.filter_ip="..filter_ip)
	--luci.sys.exec("uci set config.packet.remote_sw="..remote_sw)
	--luci.sys.exec("uci set config.packet.remote_ip="..remote_ip)
	luci.sys.exec("uci set config.packet.packet_sw="..packet_sw)
	luci.sys.exec("uci set config.packet.lan_ip="..lan_ip)
	luci.sys.exec("uci set config.packet.wan_ip="..wan_ip)
	luci.sys.exec("uci set config.packet.packet_time="..packet_time)
	luci.sys.exec("uci set config.packet.packet_size="..packet_size)
	luci.sys.exec("uci set config.packet.packet_int="..packet_int)
	
	
	luci.sys.exec("uci commit config")
	
 	return_write(0)
end

function packet_delete()
	local uci = require "luci.model.uci".cursor()
	local filter_rule = luci.http.formvalue('value1')
	local filter_ip = luci.http.formvalue('value2')
	local remote_sw = luci.http.formvalue('value3')
	local remote_ip = luci.http.formvalue('value4')
	local packet_sw = luci.http.formvalue('value5')
	local lan_ip = luci.http.formvalue('value6')
	local wan_ip = luci.http.formvalue('value7')
	local packet_time = luci.http.formvalue('value8')
	local packet_size = luci.http.formvalue('value9')
	local packet_int = luci.http.formvalue('value10')
	
	luci.sys.exec("killall -9 ethdump")
	
	luci.sys.exec("umount /mnt/emmc/packet")
	luci.sys.exec("mkfs.ext4 /dev/sda2 ")
	luci.sys.exec("mount -t ext4 -rw /dev/sda2 /mnt/emmc/packet")
	if '1' == packet_sw then
		local remote_cmd
		local filter_cmd
		local ethdump_cmd_l
		local ethdump_cmd_w
		if "0" == filter_rule then
			filter_cmd = " "
		elseif "1" == filter_rule then
			filter_cmd = " -d "..filter_ip
		elseif "2" == filter_rule then
			filter_cmd = " -s "..filter_ip
		end	
		luci.sys.exec("/sbin/ethdump "..filter_cmd.." -l "..lan_ip.." -w "..wan_ip.." -t "..packet_time.." -f "..packet_size.." -i "..packet_int.." > /dev/null 2>&1 &")
		
	end
	return_write(0)
end

function tr069_config()

	local tr069_switch = luci.http.formvalue('value1')
	local tr069_ser_url = luci.http.formvalue('value2')
	local tr069_dev_url = luci.http.formvalue('value3')
	local tr069_dev_lan = luci.http.formvalue('value4')
	
	
	if "1" == tr069_switch then
	
		luci.sys.exec("uci set tr069.ManagementServer.URL="..tr069_ser_url)
		luci.sys.exec("uci set tr069.ManagementServer.ConnectionReqURL="..tr069_dev_url)
		luci.sys.exec("uci set tr069.lan.ipaddr_n="..tr069_dev_lan)
		luci.sys.exec("killall -9 tr069 &")
		luci.sys.exec("/sbin/tr069 -D > /dev/null 2>&1 &")
		
	else 	
	
		luci.sys.exec("killall -9 tr069 &")
	end
	
	luci.sys.exec("uci set tr069.config.switch="..tr069_switch)
	luci.sys.exec("uci commit tr069")
	
	return_write(0)
end

function log_config()
	
	local log_size = luci.http.formvalue('param1')
	luci.sys.exec("killall -9 logsave")
	luci.sys.exec("uci set config.logsave.log_size="..log_size)
	luci.sys.exec("/sbin/logsave -s "..log_size.."  > /dev/null 2>&1 &")
	return_write(0)
end

function delete_config()
	
	luci.sys.exec("killall logsave")
	luci.sys.exec("umount /mnt/emmc/logsave")
	luci.sys.exec("mkfs.ext4 /dev/sda1 ")
	luci.sys.exec("mount -t ext4 -rw /dev/sda1 /mnt/emmc/logsave")
	luci.sys.exec("/sbin/logsave > /dev/null 2>&1 &")

	return_write(0)

end


function Split(szFullString, szSeparator)    
	local nFindStartIndex = 1    
	local nSplitIndex = 1    
	local nSplitArray = {}    
	while true do    
		local nFindLastIndex = string.find(szFullString, szSeparator, nFindStartIndex)    
		if not nFindLastIndex then    
			nSplitArray[nSplitIndex] = string.sub(szFullString, nFindStartIndex, string.len(szFullString))    
			break    
		end    
		nSplitArray[nSplitIndex] = string.sub(szFullString, nFindStartIndex, nFindLastIndex - 1)    
		nFindStartIndex = nFindLastIndex + string.len(szSeparator)    
		nSplitIndex = nSplitIndex + 1    
	end
	return nSplitArray    
end  

function gd_contrl()
	
	local index_num = luci.http.formvalue('value1')
	local cross_id = luci.http.formvalue('value2')
	local cross_name = luci.http.formvalue('value3')
	local contrl_ip = luci.http.formvalue('value4')
	local gps_position = luci.http.formvalue('value5')
	local gps_longitude = luci.http.formvalue('value6')
	local gps_latitude = luci.http.formvalue('value7')
	local sensor_id = luci.http.formvalue('value8')
	local sensor_status = luci.http.formvalue('value9')
	local gdctl_opr = luci.http.formvalue('value10')
	
	local line = 0
	
	if '0' == gdctl_opr then 
		--add
		local add_val ="index"..index_num..";"..cross_id..";"..cross_name..";"..contrl_ip ..";"..gps_position ..";"..gps_longitude ..";"..gps_latitude ..";"..sensor_id ..";"..sensor_status..";"
		
		local file_line = 0
		file = io.open("/etc/gdcontrl","r")
		if file then
			for lines in file:lines() do
				if "" ~= lines and nil ~= lines then
					file_line = file_line+1
				end
			end
			file:close()
		else
			return_write(-1)
		end
		
		local curr_line = 0
		local write_flag = 0
		local seek_flag = 0
		file = io.open("/etc/gdcontrl","a+")
		if file then
			for lines in file:lines() do
			while true do
				curr_line = curr_line+1
					
				if 1 ~= curr_line then
					local list = Split(lines, ";")
					
					local num = string.sub(list[1],6,string.len(list[1]))
					
					if tonumber(index_num) == tonumber(num) then 
						--luci.sys.exec("sed -i '"..curr_line.."s/^.*$/"..add_val.."/' /etc/gdcontrl")
						break
					end
					
					if tonumber(index_num) < tonumber(num) then
						add_val = add_val.."\n"
						file:write(add_val)
						write_flag = 1
					end
					
					if tonumber(file_line) == tonumber(curr_line) and write_flag == 0 then 
						add_val = add_val.."\n"
						file:write(add_val)
					end
				end
				
				if tonumber(file_line) == tonumber(curr_line) and 1== tonumber(curr_line)then
					add_val = add_val.."\n"
					file:write(add_val)
				end
				
				break
			end
			end
		
			file:close()
			return_write(0)
		else
			return_write(-1)
		end
		
		
	else
		--del
		line = luci.sys.exec("cat /etc/gdcontrl | grep index"..index_num.." -n | cut -d':' -f1")
		line = line.gsub(line,"\n","")
		
		local del_val = luci.sys.exec("sed -n "..line.."p /etc/gdcontrl")
		local list = Split(del_val, ";")
    	
    	list[9] = list[9].gsub(list[9],"\n","")
    	if "index"..index_num == list[1] and cross_id == list[2] and cross_name == list[3] and 
    		contrl_ip == list[4] and gps_position == list[5] and gps_longitude == list[6] and 
    		gps_latitude == list[7] and sensor_id == list[8] and sensor_status == list[9] then 
    		
    		luci.sys.exec("sed -i '"..line.."d' /etc/gdcontrl")
    		return_write(0)
    	else
    		return_write(-1)
    	end
		
	end
		
	
end

function gd_server()
	
	local gps_longitude = luci.http.formvalue('value1')
	local gps_latitude = luci.http.formvalue('value2')
	local contrl_ip = luci.http.formvalue('value3')
	local gps_radius = luci.http.formvalue('value4')
	
	luci.sys.exec("sed -i '1s/^.*$/"..gps_longitude..";"..gps_latitude..";"..contrl_ip..";"..gps_radius..";/' /etc/gdcontrl")
	
	return_write(0)
end

function return_write(val)
	local o = {}

	if 0 == val then
		o['result'] = 'success'
	else
		o['result'] = 'failed'
	end
	
	o['errno'] = tostring(val)
	
	o['data'] = "ok"
	
	luci.http.prepare_content("application/json")
    luci.http.write_json(o)
end

function set_infos(status)
	luci.http.prepare_content("text/plain")
	
	if 'success' == status then
		luci.http.write("Success !")
	else
		luci.http.status(0,"Failed !")
		return 
	end 
	
	luci.http.write("\n")
	luci.http.status(0,"return 0")
end