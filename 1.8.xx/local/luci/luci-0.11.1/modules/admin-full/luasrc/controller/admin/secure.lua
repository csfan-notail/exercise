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

module("luci.controller.admin.secure", package.seeall)

require("luci.http")

function index()

	entry({"admin", "secure"}, alias("admin", "secure", "secure_opt"), _("Secure Setting"), 60).index = true
	entry({"admin", "secure", "secure_opt"}, template("tau_view/secure_ip"), _("Secure"), 1)

	entry({"admin", "secure"}, alias("admin", "secure", "secure_opt","ip_filter"), _("Secure"), 60).index = true
	entry({"admin", "secure","secure_opt","ip_filter"}, template("tau_view/secure_ip"), _("IP Filter"), 1)
	entry({"admin", "secure" , "ip_filter"},call("ip_filter"),nil).leaf = true

	entry({"admin", "secure","secure_opt","mac_filter"}, template("tau_view/secure_mac"), _("MAC Filter"), 2)
	entry({"admin", "secure" , "mac_filter"},call("mac_filter"),nil).leaf = true

	entry({"admin", "secure","secure_opt","url_filter"}, template("tau_view/secure_url"), _("URL Filter"), 3)
	entry({"admin", "secure" , "url_filter"},call("url_filter"),nil).leaf = true

	entry({"admin", "secure","secure_opt","sev_filter"}, template("tau_view/secure_sev"), _("Server Access Control"), 4)
	entry({"admin", "secure" , "sev_filter"},call("sev_filter"),nil).leaf = true

	entry({"admin", "secure","secure_opt","vlan_filter"}, template("tau_view/secure_vlan"), _("Vlan Filter"), 5)
	entry({"admin", "secure" , "vlan_filter"},call("vlan_filter"),nil).leaf = true
	
	entry({"admin", "secure", "set_infos"}, call("set_infos"), nil).leaf = true
end


function vlan_filter()
	local uci = require "luci.model.uci".cursor()
	
	local action = luci.http.formvalue('port_val')
	local vlanIdA = luci.http.formvalue('port_val2')
	local vlanIdB = luci.http.formvalue('port_val3')
	
	local a_if_name = "eth1."..vlanIdA
	local b_if_name = "eth1."..vlanIdB
	local a_ip_addr,a_ip_mask = vlan_get_ipinfo(a_if_name)
	local b_ip_addr,b_ip_mask = vlan_get_ipinfo(b_if_name)
	
	local vlan_num = uci.get("secure","vlan","vlan_num")
	local reta,retb
	local errno = 0
	
	luci.sys.exec("echo xxxxxxxxxxxx >> /tmp/vlan_f.log")

	if '0' == action then 
	
			reta = os.execute("iptables -I FORWARD -s "..a_ip_addr.."/"..a_ip_mask.." -d "..b_ip_addr.."/"..b_ip_mask.." -j DROP ")
			retb = os.execute("iptables -I FORWARD -s "..b_ip_addr.."/"..b_ip_mask.." -d "..a_ip_addr.."/"..a_ip_mask.." -j DROP ")
			if 0 == tonumber(reta) and 0 == tonumber(retb) then
					luci.sys.exec("uci set secure.vlan.vlanida_"..(vlan_num+1).."="..vlanIdA)
					luci.sys.exec("uci set secure.vlan.vlanidb_"..(vlan_num+1).."="..vlanIdB)
					luci.sys.exec("uci set secure.vlan.vlan_num="..(vlan_num+1))
					luci.sys.exec("uci commit secure")

			else
					errno = 2
			end
			luci.sys.exec("echo ADD:iptables -I FORWARD -s "..a_ip_addr.."/"..a_ip_mask.." -d "..b_ip_addr.."/"..b_ip_mask.." -j DROP,reta = "..reta.." >> /tmp/vlan_f.log")
			luci.sys.exec("echo ADD:iptables -I FORWARD -s "..b_ip_addr.."/"..b_ip_mask.." -d "..a_ip_addr.."/"..a_ip_mask.." -j DROP,retb = "..retb.." >> /tmp/vlan_f.log")
		
	elseif '2' == action then   
			reta = os.execute("iptables -D FORWARD -s "..a_ip_addr.."/"..a_ip_mask.." -d "..b_ip_addr.."/"..b_ip_mask.." -j DROP ")
			retb = os.execute("iptables -D FORWARD -s "..b_ip_addr.."/"..b_ip_mask.." -d "..a_ip_addr.."/"..a_ip_mask.." -j DROP ")

			if 0 == tonumber(reta) and 0 == tonumber(retb) then
					loop_act_white_vlan(vlan_num,vlanIdA,vlanIdB)
					luci.sys.exec("uci set secure.vlan.vlan_num="..(vlan_num-1))
					luci.sys.exec("uci commit secure")
			else
					errno = 3
			end
			luci.sys.exec("echo DEL:iptables -D FORWARD -s "..a_ip_addr.."/"..a_ip_mask.." -d "..b_ip_addr.."/"..b_ip_mask.." -j DROP,reta = "..reta.." >> /tmp/vlan_f.log")
			luci.sys.exec("echo DEL:iptables -D FORWARD -s "..b_ip_addr.."/"..b_ip_mask.." -d "..a_ip_addr.."/"..a_ip_mask.." -j DROP,retb = "..retb.." >> /tmp/vlan_f.log")

	end
	
	return_write(errno)
end

function loop_act_white_vlan(number,vlanIdA,vlanIdB)

	local loop_r = 0
	local a_vlanid_str,b_vlanid_str,a_vlanid_val,b_vlanid_val
	
	for num=1,tonumber(number) do

		if tonumber(number) == tonumber(num) then 
			luci.sys.exec("uci delete secure.vlan.vlanida_"..num)
			luci.sys.exec("uci delete secure.vlan.vlanidb_"..num)
		else	
		
			a_vlanid_str = "vlanida_"..num
			b_vlanid_str = "vlanidb_"..num
			
			a_vlanid_val = uci.get("secure","vlan",a_vlanid_str)
			b_vlanid_val = uci.get("secure","vlan",b_vlanid_str)
			
			if ((a_vlanid_val == vlanIdA and b_vlanid_val == vlanIdB) or  (0 < loop_r)) then
			
					a_vlanid_val = uci.get("secure","vlan","vlanida_"..(num+1))
					b_vlanid_val = uci.get("secure","vlan","vlanidb_"..(num+1))
					
					luci.sys.exec("uci set secure.vlan.vlanida_"..(num).."="..a_vlanid_val)
					luci.sys.exec("uci set secure.vlan.vlanidb_"..(num).."="..b_vlanid_val)
					
					loop_r = loop_r + 1
			end
			
		end
		
	end		
	
end

function vlan_get_ipinfo(if_name)

	local ip_addr = ""
	local ip_mask = ""
	local strcat = string.gsub(luci.util.exec(tostring("cat /etc/config/network |grep interface|awk '{print $3}'")), "'","")
	local arr = string.split(strcat, "\n")
	
	for i,value in ipairs(arr) do  
		if if_name == uci.get("network",value,"ifname") then 
				ip_addr = uci.get("network",value,"ipaddr")
				ip_mask = uci.get("network",value,"netmask")
		end
	end 
	
	return ip_addr,ip_mask
end

function ip_filter()

	local uci = require "luci.model.uci".cursor()
	ip_num = uci.get("secure","ip","ip_num")
	white_ip_num = uci.get("secure","white_ip","ip_num")
	local add_ip = luci.http.formvalue('port_val')
	local action = luci.http.formvalue('port_val2')
	local addport = luci.http.formvalue('port_val3')
	
	if '0' == action then 
			
					luci.sys.exec("iptables -I FORWARD -p tcp -d "..add_ip.." --dport "..addport.." -j DROP")
					luci.sys.exec("iptables -I FORWARD -p udp -d "..add_ip.." --dport "..addport.." -j DROP")
					luci.sys.exec("uci set secure.ip.ip_"..(ip_num+1).."="..add_ip)
					luci.sys.exec("uci set secure.ip.port_"..(ip_num+1).."="..addport)
					luci.sys.exec("uci set secure.ip.ip_num="..(ip_num+1))
		
	elseif '1' == action then 
					luci.sys.exec("iptables -I FORWARD -p tcp -d "..add_ip.." --dport "..addport.." -j ACCEPT")
					luci.sys.exec("iptables -I FORWARD -p udp -d "..add_ip.." --dport "..addport.." -j ACCEPT")
					luci.sys.exec("uci set secure.white_ip.ip_"..(white_ip_num+1).."="..add_ip)
					luci.sys.exec("uci set secure.white_ip.port_"..(white_ip_num+1).."="..addport)
					luci.sys.exec("uci set secure.white_ip.ip_num="..(white_ip_num+1))	
	elseif '2' == action then 
		loop_act_white_ip(add_ip,white_ip_num,addport)
		luci.sys.exec("uci set secure.white_ip.ip_num="..(white_ip_num-1))
	else
		loop_act_black_ip(add_ip,ip_num,addport)
		luci.sys.exec("uci set secure.ip.ip_num="..(ip_num-1))
	end

	luci.sys.exec("uci commit secure")
	
	return_write(0)

end

function loop_act_black_ip(ipaddr,number,sevport)

	local loop_r = 0
	
	for num=1,tonumber(number) do

		if tonumber(number) == tonumber(num) then 

			local del_cmd = "uci delete secure.ip.ip_"..num
			luci.sys.exec("uci delete secure.ip.ip_"..num)
			luci.sys.exec("uci delete secure.ip.port_"..num)
			luci.sys.exec("iptables -D FORWARD -p tcp -d "..ipaddr.." --dport "..sevport.." -j DROP")
			luci.sys.exec("iptables -D FORWARD -p udp -d "..ipaddr.." --dport "..sevport.." -j DROP")
		else		 

			local ip_str = "ip_"..num
			local port_str = "port_"..num
			
			ip_addr = uci.get("secure","ip",ip_str)
			port_addr = uci.get("secure","ip",port_str)
			if ip_addr == ipaddr and port_addr == sevport or  1 == loop_r then 
				if 0 == loop_r then 	
						luci.sys.exec("iptables -D FORWARD -p tcp -d "..ipaddr.." --dport "..sevport.." -j DROP")
						luci.sys.exec("iptables -D FORWARD -p udp -d "..ipaddr.." --dport "..sevport.." -j DROP")
				end
				ip_addr = uci.get("secure","ip","ip_"..(num+1))
				port_addr = uci.get("secure","ip","port_"..(num+1))
				
				luci.sys.exec("uci set secure.ip."..ip_str.."="..ip_addr)
				luci.sys.exec("uci set secure.ip."..port_str.."="..port_addr)
				
				loop_r = 1
			end
			
		end
		
	end		
	
end

function loop_act_white_ip(ipaddr,number,sevport)

	local loop_r = 0
	
	for num=1,tonumber(number) do

		if tonumber(number) == tonumber(num) then 

			local del_cmd = "uci delete secure.white_ip.ip_"..num
			luci.sys.exec("uci delete secure.white_ip.ip_"..num)
			luci.sys.exec("uci delete secure.white_ip.port_"..num)
			luci.sys.exec("iptables -D FORWARD -p tcp -d "..ipaddr.." --dport "..sevport.." -j ACCEPT")
			luci.sys.exec("iptables -D FORWARD -p udp -d "..ipaddr.." --dport "..sevport.." -j ACCEPT")
			
		else		 

			local ip_str = "ip_"..num
			local port_str = "port_"..num
			
			ip_addr = uci.get("secure","white_ip",ip_str)
			port_addr = uci.get("secure","white_ip",port_str)
			if ip_addr == ipaddr and port_addr == sevport or  1 == loop_r then 
				if 0 == loop_r then 
						luci.sys.exec("iptables -D FORWARD -p tcp -d "..ipaddr.." --dport "..sevport.." -j ACCEPT")
						luci.sys.exec("iptables -D FORWARD -p udp -d "..ipaddr.." --dport "..sevport.." -j ACCEPT")
				end
				ip_addr = uci.get("secure","white_ip","ip_"..(num+1))
				port_addr = uci.get("secure","white_ip","port_"..(num+1))
				
				luci.sys.exec("uci set secure.white_ip."..ip_str.."="..ip_addr)
				luci.sys.exec("uci set secure.white_ip."..port_str.."="..port_addr)
				loop_r = 1
			end
			
		end
		
	end		
	
end
function mac_filter()
	
	local uci = require "luci.model.uci".cursor()
	mac_num = uci.get("secure","mac","mac_num")
	white_mac_num = uci.get("secure","white_mac","mac_num")
	local add_mac = luci.http.formvalue('port_val')
	local action = luci.http.formvalue('port_val2')
	
	if '0' == action then 
				luci.sys.exec("iptables -A FORWARD -m mac --mac-source "..add_mac.." -j DROP")
				luci.sys.exec("iptables -A INPUT -m mac --mac-source "..add_mac.." -j DROP")
				luci.sys.exec("uci set secure.mac.mac_"..(mac_num+1).."="..add_mac)
				luci.sys.exec("uci set secure.mac.mac_num="..(mac_num+1))
	elseif  '1' == action then 
				luci.sys.exec("iptables -A FORWARD -m mac --mac-source "..add_mac.." -j ACCEPT")
				luci.sys.exec("iptables -A INPUT -m mac --mac-source "..add_mac.." -j ACCEPT")
				luci.sys.exec("uci set secure.white_mac.mac_"..(white_mac_num+1).."="..add_mac)
				luci.sys.exec("uci set secure.white_mac.mac_num="..(white_mac_num+1))
	elseif '2' == action then 
		loop_act_white_mac(add_mac,white_mac_num)
		luci.sys.exec("uci set secure.white_mac.mac_num="..(white_mac_num-1))
	else
		loop_act_black_mac(add_mac,mac_num)
		luci.sys.exec("uci set secure.mac.mac_num="..(mac_num-1))
	end

	luci.sys.exec("uci commit secure")
	
	return_write(0)
end

function loop_act_black_mac(macaddr,number)

	local loop_r = 0
	
	for num=1,tonumber(number) do

		if tonumber(number) == tonumber(num) then 

			local del_cmd = "uci delete secure.mac.mac_"..num
			luci.sys.exec("uci delete secure.mac.mac_"..num)
			luci.sys.exec("iptables -D INPUT -m mac --mac-source "..macaddr.." -j DROP")
			luci.sys.exec("iptables -D FORWARD -m mac --mac-source "..macaddr.." -j DROP")
		else		 

			local mac_str = "mac_"..num
			mac_addr = uci.get("secure","mac",mac_str)
			if mac_addr == macaddr or  1 == loop_r then 
				if 0 == loop_r then 
					luci.sys.exec("iptables -D INPUT -m mac --mac-source "..macaddr.." -j DROP")
					luci.sys.exec("iptables -D FORWARD -m mac --mac-source "..macaddr.." -j DROP")	
				end
				mac_addr = uci.get("secure","mac","mac_"..(num+1))
				luci.sys.exec("uci set secure.mac."..mac_str.."="..mac_addr)
				loop_r = 1
			end
			
		end
		
	end		
	
end
function loop_act_white_mac(macaddr,number)

	local loop_r = 0
	
	for num=1,tonumber(number) do

		if tonumber(number) == tonumber(num) then 

			local del_cmd = "uci delete secure.white_mac.mac_"..num
			luci.sys.exec("uci delete secure.white_mac.mac_"..num)
			luci.sys.exec("iptables -D INPUT -m mac --mac-source "..macaddr.." -j ACCEPT")
			luci.sys.exec("iptables -D FORWARD -m mac --mac-source "..macaddr.." -j ACCEPT")
		else		 

			local mac_str = "mac_"..num
			mac_addr = uci.get("secure","white_mac",mac_str)
			if mac_addr == macaddr or  1 == loop_r then 
				if 0 == loop_r then 
					luci.sys.exec("iptables -D INPUT -m mac --mac-source "..macaddr.." -j ACCEPT")
					luci.sys.exec("iptables -D FORWARD -m mac --mac-source "..macaddr.." -j ACCEPT")
				end
				mac_addr = uci.get("secure","white_mac","mac_"..(num+1))
				luci.sys.exec("uci set secure.white_mac."..mac_str.."="..mac_addr)
				loop_r = 1
			end
			
		end
		
	end		
	
end


function url_filter()
	
	local uci = require "luci.model.uci".cursor()
	url_num = uci.get("secure","url","url_num")
	white_url_num = uci.get("secure","white_url","url_num")
	local add_url = luci.http.formvalue('port_val')
	local action = luci.http.formvalue('port_val2')
	
	if '0' == action then  
		luci.sys.exec("iptables -I FORWARD -p tcp -d "..add_url.." --dport 80 -j DROP")
		luci.sys.exec("uci set secure.url.url_"..(url_num+1).."="..add_url)
		luci.sys.exec("uci set secure.url.url_num="..(url_num+1))
	elseif  '1' == action then
		luci.sys.exec("iptables -I FORWARD -p tcp -d "..add_url.." --dport 80 -j ACCEPT")
		luci.sys.exec("uci set secure.white_url.url_"..(white_url_num+1).."="..add_url)
		luci.sys.exec("uci set secure.white_url.url_num="..(white_url_num+1))
	elseif '2' == action then
		loop_act_white_url(add_url,white_url_num)
		luci.sys.exec("uci set secure.white_url.url_num="..(white_url_num-1))
	else
		loop_act_black_url(add_url,url_num)
		luci.sys.exec("uci set secure.url.url_num="..(url_num-1))
	end

	luci.sys.exec("uci commit secure")

	return_write(0)
end

function loop_act_black_url(urladdr,number)

	local loop_r = 0
	
	for num=1,tonumber(number) do

		if tonumber(number) == tonumber(num) then 

			local del_cmd = "uci delete secure.url.url_"..num
			luci.sys.exec("uci delete secure.url.url_"..num)
			luci.sys.exec("iptables -D FORWARD -p tcp -d "..urladdr.." --dport 80 -j DROP")

		else		 

			local url_str = "url_"..num
			url_addr = uci.get("secure","url",url_str)
			if url_addr == urladdr or  1 == loop_r then 
				if 0 == loop_r then 
					luci.sys.exec("iptables -D FORWARD -p tcp -d "..urladdr.." --dport 80 -j DROP")
				end
				url_addr = uci.get("secure","url","url_"..(num+1))
				luci.sys.exec("uci set secure.url."..url_str.."="..url_addr)
				loop_r = 1
			end
			
		end
		
	end		
	
end
function loop_act_white_url(urladdr,number)

	local loop_r = 0
	
	for num=1,tonumber(number) do

		if tonumber(number) == tonumber(num) then 

			local del_cmd = "uci delete secure.white_url.url_"..num
			luci.sys.exec("uci delete secure.white_url.url_"..num)
			luci.sys.exec("iptables -D FORWARD -p tcp -d "..urladdr.." --dport 80 -j ACCEPT")

		else		 

			local url_str = "url_"..num
			url_addr = uci.get("secure","white_url",url_str)
			if url_addr == urladdr or  1 == loop_r then 
				if 0 == loop_r then 
					luci.sys.exec("iptables -D FORWARD -p tcp -d "..urladdr.." --dport 80 -j ACCEPT")
				end
				url_addr = uci.get("secure","white_url","url_"..(num+1))
				luci.sys.exec("uci set secure.white_url."..url_str.."="..url_addr)
				loop_r = 1
			end
			
		end
		
	end		
	
end

function sev_filter()

	local uci = require "luci.model.uci".cursor()
	sev_num = uci.get("secure","sev","sev_num")
	white_sev_num = uci.get("secure","white_sev","sev_num")
	
	local action = luci.http.formvalue('port_val')
	local pro = luci.http.formvalue('port_val2')
	local intface = luci.http.formvalue('port_val3')
	if '0' == action then 
			if 'usb0' == intface then
						if '0' == pro then 
							luci.sys.exec("iptables -A INPUT -p tcp -i "..intface.." -j DROP")
							luci.sys.exec("uci set secure.sev.protocol_"..(sev_num+1).."="..pro)
							luci.sys.exec("uci set secure.sev.interface_"..(sev_num+1).."="..intface)
							luci.sys.exec("uci set secure.sev.sev_num="..(sev_num+1))
							
						elseif '1' == pro then
							luci.sys.exec("iptables -A INPUT -p udp -i "..intface.." -j DROP")
							luci.sys.exec("uci set secure.sev.protocol_"..(sev_num+1).."="..pro)
							luci.sys.exec("uci set secure.sev.interface_"..(sev_num+1).."="..intface)
							luci.sys.exec("uci set secure.sev.sev_num="..(sev_num+1))
						elseif '2' == pro then
							luci.sys.exec("iptables -A INPUT -p icmp -i "..intface.." -j DROP")
							luci.sys.exec("uci set secure.sev.protocol_"..(sev_num+1).."="..pro)
							luci.sys.exec("uci set secure.sev.interface_"..(sev_num+1).."="..intface)
							luci.sys.exec("uci set secure.sev.sev_num="..(sev_num+1))
						elseif '5' == pro then
							luci.sys.exec("iptables -A FORWARD -p tcp -i "..intface.." --dport 80 -j DROP")
							luci.sys.exec("uci set secure.sev.protocol_"..(sev_num+1).."="..pro)
							luci.sys.exec("uci set secure.sev.interface_"..(sev_num+1).."="..intface)
							luci.sys.exec("uci set secure.sev.sev_num="..(sev_num+1))
						else
							luci.sys.exec("iptables -A INPUT -i "..intface.." -j DROP")
							luci.sys.exec("uci set secure.sev.protocol_"..(sev_num+1).."="..pro)
							luci.sys.exec("uci set secure.sev.interface_"..(sev_num+1).."="..intface)
							luci.sys.exec("uci set secure.sev.sev_num="..(sev_num+1))
						end
			else
						if '0' == pro then 
							luci.sys.exec("iptables -A FORWARD -p tcp -i "..intface.." -j DROP")
							luci.sys.exec("uci set secure.sev.protocol_"..(sev_num+1).."="..pro)
							luci.sys.exec("uci set secure.sev.interface_"..(sev_num+1).."="..intface)
							luci.sys.exec("uci set secure.sev.sev_num="..(sev_num+1))
							
						elseif '1' == pro then
							luci.sys.exec("iptables -A FORWARD -p udp -i "..intface.." -j DROP")
							luci.sys.exec("uci set secure.sev.protocol_"..(sev_num+1).."="..pro)
							luci.sys.exec("uci set secure.sev.interface_"..(sev_num+1).."="..intface)
							luci.sys.exec("uci set secure.sev.sev_num="..(sev_num+1))
						elseif '2' == pro then
							luci.sys.exec("iptables -A FORWARD -p icmp -i "..intface.." -j DROP")
							luci.sys.exec("uci set secure.sev.protocol_"..(sev_num+1).."="..pro)
							luci.sys.exec("uci set secure.sev.interface_"..(sev_num+1).."="..intface)
							luci.sys.exec("uci set secure.sev.sev_num="..(sev_num+1))
						elseif '5' == pro then
							luci.sys.exec("iptables -A FORWARD -p tcp -i "..intface.." --dport 80 -j DROP")
							luci.sys.exec("uci set secure.sev.protocol_"..(sev_num+1).."="..pro)
							luci.sys.exec("uci set secure.sev.interface_"..(sev_num+1).."="..intface)
							luci.sys.exec("uci set secure.sev.sev_num="..(sev_num+1))
						else
							luci.sys.exec("iptables -A FORWARD -i "..intface.." -j DROP")
							luci.sys.exec("uci set secure.sev.protocol_"..(sev_num+1).."="..pro)
							luci.sys.exec("uci set secure.sev.interface_"..(sev_num+1).."="..intface)
							luci.sys.exec("uci set secure.sev.sev_num="..(sev_num+1))
						end
			end
	else
				
				loop_act_black_sev(sev_num,pro,intface)
				luci.sys.exec("uci set secure.sev.sev_num="..(sev_num-1))
				
	end

	luci.sys.exec("uci commit secure")

	return_write(0)

end

function loop_act_black_sev(number,sevpro,sevintface)

	local loop_r = 0
	
	for num=1,tonumber(number) do

		if tonumber(number) == tonumber(num) then 
				
				
				luci.sys.exec("uci delete secure.sev.protocol_"..num)
				luci.sys.exec("uci delete secure.sev.interface_"..num)
				if 'usb0' == sevintface then
						if '0' == sevpro then
							luci.sys.exec("iptables -D INPUT -p tcp -i "..sevintface.." -j DROP")
						elseif '1' == sevpro then
							luci.sys.exec("iptables -D INPUT -p udp -i "..sevintface.." -j DROP")
						elseif '2' == sevpro then
							luci.sys.exec("iptables -D INPUT -p icmp -i "..sevintface.." -j DROP")
						elseif '5' == sevpro then
							luci.sys.exec("iptables -D FORWARD -p tcp -i "..sevintface.." --dport 80 -j DROP")
						else
							luci.sys.exec("iptables -D INPUT -i "..sevintface.." -j DROP")
						end
				else
						if '0' == sevpro then
							luci.sys.exec("iptables -D FORWARD -p tcp -i "..sevintface.." -j DROP")
						elseif '1' == sevpro then
							luci.sys.exec("iptables -D FORWARD -p udp -i "..sevintface.." -j DROP")
						elseif '2' == sevpro then
							luci.sys.exec("iptables -D FORWARD -p icmp -i "..sevintface.." -j DROP")
						elseif '5' == sevpro then
							luci.sys.exec("iptables -D FORWARD -p tcp -i "..sevintface.." --dport 80 -j DROP")
						else
							luci.sys.exec("iptables -D FORWARD -i "..sevintface.." -j DROP")
						end
				end
				
		else		 

			
			local protocol_str = "protocol_"..num
			local interface_str = "interface_"..num
			
			protocol_addr = uci.get("secure","sev",protocol_str)
			interface_addr = uci.get("secure","sev",interface_str)
			
			if  protocol_addr == sevpro and  interface_addr == sevintface or  1 == loop_r then 
				if 0 == loop_r then 
						
						if 'usb0' == sevintface then
								if '0' == sevpro then
									luci.sys.exec("iptables -D INPUT -p tcp -i "..sevintface.." -j DROP")
								elseif '1' == sevpro then
									luci.sys.exec("iptables -D INPUT -p udp -i "..sevintface.." -j DROP")
								elseif '2' == sevpro then
									luci.sys.exec("iptables -D INPUT -p icmp -i "..sevintface.." -j DROP")
								elseif '5' == sevpro then
									luci.sys.exec("iptables -D FORWARD -p tcp -i "..sevintface.." --dport 80 -j DROP")
								else
									luci.sys.exec("iptables -D INPUT -i "..sevintface.." -j DROP")
								end
						else
								if '0' == sevpro then
									luci.sys.exec("iptables -D FORWARD -p tcp -i "..sevintface.." -j DROP")
								elseif '1' == sevpro then
									luci.sys.exec("iptables -D FORWARD -p udp -i "..sevintface.." -j DROP")
								elseif '2' == sevpro then
									luci.sys.exec("iptables -D FORWARD -p icmp -i "..sevintface.." -j DROP")
								elseif '5' == sevpro then
									luci.sys.exec("iptables -D FORWARD -p tcp -i "..sevintface.." --dport 80 -j DROP")
								else
									luci.sys.exec("iptables -D FORWARD -i "..sevintface.." -j DROP")
								end
						end
				end	
				protocol_addr = uci.get("secure","sev","protocol_"..(num+1))
				interface_addr = uci.get("secure","sev","interface_"..(num+1))
				
				luci.sys.exec("uci set secure.sev."..protocol_str.."="..protocol_addr)
				luci.sys.exec("uci set secure.sev."..interface_str.."="..interface_addr)
				
				loop_r = 1
			end
			
		end
		
	end		
	
end


function return_write(val)
	
	local o = {}
	
	if 0 == val then
		o['result'] = 'success'
	else
		o['result'] = 'failed'
	end
	
	o['data'] = "ok"
	o['val'] = tostring(val)
	
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