module("luci.controller.admin.lte", package.seeall)

require("luci.http")

function index()

	local uci = require "luci.model.uci".cursor()
	local single_double_switch = uci.get("lte","set","single_double_switch")
	single_double_switch = single_double_switch.gsub(single_double_switch,"\n","")
	if '0' == single_double_switch then

	entry({"admin", "lte"}, alias("admin", "lte", "lte status info"), _("LTE Setting"), 60).index = true
	entry({"admin", "lte", "lte status info"}, template("tau_view/lte_status"), _("LTE Status Information"), 1)

	entry({"admin", "lte", "lte basic setting"}, alias("admin", "lte", "lte basic setting", "basic_set"), _("LTE Basic Setting"), 2)
	entry({"admin", "lte", "lte basic setting", "basic_set"}, template("tau_view/lte_basic_set"), _("Basic Setting"), 2).leaf = true
	entry({"admin", "lte", "lte_auto_connect_time"}, call("lte_auto_connect_time"), nil).leaf = true
	entry({"admin", "lte", "lte basic setting","lte wan"}, template("tau_view/ltewan"), _("LTE WAN"),3).leaf = true
	entry({"admin", "lte", "ltewan_config"}, call("ltewan_config"), nil).leaf = true
	
	entry({"admin", "lte", "lte higt setting"}, alias("admin", "lte", "lte higt setting", "access_set"), _("LTE Higt Setting"), 3)
	entry({"admin", "lte", "lte higt setting", "access_set"}, template("tau_view/lte_access_set")).leaf = true
	entry({"admin", "lte", "lte_lock_frequency"}, call("lte_lock_frequency"), nil).leaf = true
	entry({"admin", "lte", "lte_lock_spectrum"}, call("lte_lock_spectrum"), nil).leaf = true

	--entry({"admin", "lte", "lte higt setting", "pin_set"}, template("tau_view/lte_pin_set"), _("PIN Setting"), 4).leaf = true
	--entry({"admin", "lte", "lte_higt_simlock_sw"}, call("lte_higt_simlock_sw"), nil).leaf = true
	--entry({"admin", "lte", "lte_higt_pin_opt"}, call("lte_higt_pin_opt"), nil).leaf = true
	--lteupdate
	--entry({"admin", "lte", "lteupgrade"}, call("lte_upgrade"), _("LTE Update"), 5)
	--entry({"admin", "lte", "lteupgrade", "vflagstatus"}, call("action_vflag_status")).leaf = true	
	
	--lastroute
	entry({"admin", "lte", "last route"}, template("tau_view/lastroute"), _("Last Route"),6)
	entry({"admin", "lte", "last_switch"}, call("last_switch"), nil).leaf = true
	--qos
	--entry({"admin", "lte", "lte qos"}, template("tau_view/qos_config"), _("LTE QOS"),7)
	--entry({"admin", "lte", "qos_config"}, call("qos_config"), nil).leaf = true
	--sim switch
	entry({"admin", "lte", "lte sim"}, template("tau_view/lte_sim"), _("LTE SIM"),8)
	entry({"admin", "lte", "sim_switch"}, call("sim_switch"), nil).leaf = true
	
	--sim unlocked
	entry({"admin", "lte", "lte unlock"}, template("tau_view/lte_unlock"), _("LTE Unlocked"),8)
	entry({"admin", "lte", "lte_unlock"}, call("lte_unlock"), nil).leaf = true
	
	--single double module change
	entry({"admin", "lte", "lte module set"}, alias("admin", "lte", "lte module set", "lte_module_set"), _("LTE Single Double Set"), 10)
	entry({"admin", "lte", "lte module set","lte_module_set"}, template("tau_view/lte_single_double_module"))
	entry({"admin", "lte", "lte_single_double_set"}, call("lte_single_double_set"), nil).leaf = true
	
	entry({"admin", "lte", "set_infos"}, call("set_infos"), nil).leaf = true




	else
	
	entry({"admin", "lte"}, alias("admin", "lte", "lte status info"), _("LTE Setting"), 60).index = true
	entry({"admin", "lte", "lte status info"}, template("tau_view/lte_status1"), _("LTE Status Information"), 1)


	entry({"admin", "lte", "lte basic setting"}, alias("admin", "lte", "lte basic setting", "basic_set"), _("LTE Basic Setting"), 2)
	entry({"admin", "lte", "lte basic setting", "basic_set"}, template("tau_view/lte_basic_set"), _("Basic Setting"), 2).leaf = true
	entry({"admin", "lte", "lte_auto_connect_time"}, call("lte_auto_connect_time"), nil).leaf = true
	entry({"admin", "lte", "lte basic setting","lte wan"}, template("tau_view/ltewan"), _("LTE WAN"),3).leaf = true
	entry({"admin", "lte", "ltewan_config"}, call("ltewan_config"), nil).leaf = true
	
	entry({"admin", "lte", "lte basic setting", "basic_set2"}, template("tau_view/lte_basic_set2"), _("LTE2 Basic Setting"), 4).leaf = true
	entry({"admin", "lte", "lte_auto_connect_time2"}, call("lte_auto_connect_time2"), nil).leaf = true
	entry({"admin", "lte", "lte basic setting","lte wan2"}, template("tau_view/ltewan2"), _("LTE2 WAN"),5).leaf = true
	entry({"admin", "lte", "ltewan_config2"}, call("ltewan_config2"), nil).leaf = true
	
	entry({"admin", "lte", "lte higt setting"}, alias("admin", "lte", "lte higt setting", "access_set"), _("LTE Higt Setting"), 3)
	entry({"admin", "lte", "lte higt setting", "access_set"}, template("tau_view/lte_access_set"),_("LTE1 Hight Setting"),3).leaf = true
	entry({"admin", "lte", "lte_lock_frequency"}, call("lte_lock_frequency"), nil).leaf = true
	entry({"admin", "lte", "lte_lock_spectrum"}, call("lte_lock_spectrum"), nil).leaf = true

	entry({"admin", "lte", "lte higt setting", "access_set2"}, template("tau_view/lte_access_set2"),_("LTE2 Hight Setting"),4).leaf = true
	entry({"admin", "lte", "lte_lock_frequency2"}, call("lte_lock_frequency2"), nil).leaf = true
	entry({"admin", "lte", "lte_lock_spectrum2"}, call("lte_lock_spectrum2"), nil).leaf = true

	--entry({"admin", "lte", "lte higt setting", "pin_set"}, template("tau_view/lte_pin_set"), _("PIN Setting"), 4).leaf = true
	--entry({"admin", "lte", "lte_higt_simlock_sw"}, call("lte_higt_simlock_sw"), nil).leaf = true
	--entry({"admin", "lte", "lte_higt_pin_opt"}, call("lte_higt_pin_opt"), nil).leaf = true
	--lteupdate
	--entry({"admin", "lte", "lteupgrade"}, call("lte_upgrade"), _("LTE Update"), 5)
	--entry({"admin", "lte", "lteupgrade", "vflagstatus"}, call("action_vflag_status")).leaf = true	
	
	--lastroute
	entry({"admin", "lte", "last route"}, alias("admin", "lte", "last route", "last_route"), _("Last Route"), 6)
	entry({"admin", "lte", "last route","last_route"}, template("tau_view/lastroute"), _("LTE1 Last Route"),7).leaf = ture
	entry({"admin", "lte", "last_switch"}, call("last_switch"), nil).leaf = true
	
	entry({"admin", "lte", "last route","last_route2"}, template("tau_view/lastroute2"), _("LTE2 Last Route"),8).leaf = ture
	entry({"admin", "lte", "last_switch2"}, call("last_switch2"), nil).leaf = true
	
	--qos
	--entry({"admin", "lte", "lte qos"}, template("tau_view/qos_config"), _("LTE QOS"),7)
	--entry({"admin", "lte", "qos_config"}, call("qos_config"), nil).leaf = true
	--sim switch
	entry({"admin", "lte", "lte sim"}, alias("admin", "lte", "lte sim", "lte_sim"), _("LTE SIM"), 8)
	entry({"admin", "lte", "lte sim","lte_sim"}, template("tau_view/lte_sim"), _("LTE1 SIM"),8).leaf = ture
	entry({"admin", "lte", "sim_switch"}, call("sim_switch"), nil).leaf = true
	
	entry({"admin", "lte", "lte sim","lte_sim2"}, template("tau_view/lte_sim2"), _("LTE2 SIM"),9).leaf = ture
	entry({"admin", "lte", "sim_switch2"}, call("sim_switch2"), nil).leaf = true
	
	--sim unlocked
	entry({"admin", "lte", "lte unlock"}, alias("admin", "lte", "lte unlock", "lte_unlock"), _("LTE Unlocked"), 9)
	entry({"admin", "lte", "lte unlock","lte_unlock"}, template("tau_view/lte_unlock"), _("LTE1 Unlocked"),9).leaf = ture
	entry({"admin", "lte", "lte_unlock"}, call("lte_unlock"), nil).leaf = true
	
	entry({"admin", "lte", "lte unlock","lte_unlock2"}, template("tau_view/lte_unlock2"), _("LTE2 Unlocked"),10).leaf = ture
	entry({"admin", "lte", "lte_unlock2"}, call("lte_unlock2"), nil).leaf = true
	
	--single double module change
	entry({"admin", "lte", "lte module set"}, alias("admin", "lte", "lte module set", "lte_module_set"), _("LTE Single Double Set"), 10)
	entry({"admin", "lte", "lte module set","lte_module_set"}, template("tau_view/lte_single_double_module"))
	entry({"admin", "lte", "lte_single_double_set"}, call("lte_single_double_set"), nil).leaf = true
	
	entry({"admin", "lte", "set_infos"}, call("set_infos"), nil).leaf = true
	
	
	
	end
end

upgrade_file = "/tmp/lteupdate.info"
version_tmp = "/tmp/lteupdate.bin"

function lte_single_double_set()

	local module_set_switch = tonumber(luci.http.formvalue('param1'))
	luci.sys.exec("uci set lte.set.single_double_switch="..module_set_switch)
	luci.sys.exec("uci commit lte")
	
	
	return_write(0)
	
end

function action_vflag_status()
	
	local buff = tostring(luci.sys.exec("tail -n 1 "..upgrade_file.."  |cut -d':' -f1"))
		
	if buff == "SUCCESS\n" then
		luci.http.write(tostring(luci.sys.exec("tail -n 1 "..upgrade_file.." ")))
		nixio.fs.unlink(version_tmp);
	elseif buff == "ERROR\n" then
		luci.http.write(tostring(luci.sys.exec("tail -n 1 "..upgrade_file.." ")))
		nixio.fs.unlink(version_tmp);
	else
		luci.http.write(tostring(luci.sys.exec("tail -n 1 "..upgrade_file.." ")))
	end
end


function ltewan_config()

	local uci = require "luci.model.uci".cursor()
	
	local wan_dail 	= luci.http.formvalue('dialway')
	
	if '1' == wan_dail then
		local wan_ipadd = luci.http.formvalue('ipaddr')
		local wan_mask 	= luci.http.formvalue('netmask')
		local wan_gate 	= luci.http.formvalue('gateway')
		local wan_dns1 	= luci.http.formvalue('dnsser1')
		local wan_dns2 	= luci.http.formvalue('dnsser2')
		luci.sys.exec("uci set config.ltewan.ipaddr="..wan_ipadd)
		luci.sys.exec("uci set config.ltewan.netmask="..wan_mask)
		luci.sys.exec("uci set config.ltewan.gateway="..wan_gate)
		luci.sys.exec("uci set config.ltewan.dnsser1="..wan_dns1)
		luci.sys.exec("uci set config.ltewan.dnsser2="..wan_dns2)
	end
	
	luci.sys.exec("uci set config.ltewan.dialway="..wan_dail)
	luci.sys.exec("uci commit config")
	
	return_write(0)
    
end

function ltewan_config2()

	local uci = require "luci.model.uci".cursor()
	
	local wan_dail 	= luci.http.formvalue('dialway')
	
	if '1' == wan_dail then
		local wan_ipadd = luci.http.formvalue('ipaddr')
		local wan_mask 	= luci.http.formvalue('netmask')
		local wan_gate 	= luci.http.formvalue('gateway')
		local wan_dns1 	= luci.http.formvalue('dnsser1')
		local wan_dns2 	= luci.http.formvalue('dnsser2')
		luci.sys.exec("uci set config.ltewan2.ipaddr="..wan_ipadd)
		luci.sys.exec("uci set config.ltewan2.netmask="..wan_mask)
		luci.sys.exec("uci set config.ltewan2.gateway="..wan_gate)
		luci.sys.exec("uci set config.ltewan2.dnsser1="..wan_dns1)
		luci.sys.exec("uci set config.ltewan2.dnsser2="..wan_dns2)
	end
	
	luci.sys.exec("uci set config.ltewan2.dialway="..wan_dail)
	luci.sys.exec("uci commit config")
	
	return_write(0)
    
end

--[[function nat_config()
local uci = require "luci.model.uci".cursor()
	nat_num = uci.get("nat","nat","nat_num")
	local add_nat_switch = luci.http.formvalue('value1')
	local add_nat_default_switch = luci.http.formvalue('value2')
	local add_nat_rule_index = luci.http.formvalue('value3')
	local add_nat_rule_type = luci.http.formvalue('value4')
	local add_nat_rule_src_ip = luci.http.formvalue('value5')
	local add_nat_rule_src_netmask = luci.http.formvalue('value6') 
    local add_nat_rule_dst_ip = luci.http.formvalue('value7')
	local action = luci.http.formvalue('value8')
	if '0' == action then 
			if '1' == add_nat_switch then
					if '1' == add_nat_default_switch then
							luci.sys.exec("iptables -t nat -A POSTROUTING -j MASQUERADE")
					end
			end
	elseif '1' == action then 
			if '1' == add_nat_switch then
							if '0' == add_nat_rule_type then 
								luci.sys.exec("iptables -t nat -I POSTROUTING -s "..add_nat_rule_src_ip.." -j SNAT --to "..add_nat_rule_dst_ip)
								luci.sys.exec("uci set nat.nat.srcip_"..(nat_num+1).."="..add_nat_rule_src_ip)
								luci.sys.exec("uci set nat.nat.dstip_"..(nat_num+1).."="..add_nat_rule_dst_ip)
								luci.sys.exec("uci set nat.nat.type_"..(nat_num+1).."="..add_nat_rule_type)
								
								luci.sys.exec("uci set nat.nat.srcnetmask_"..(nat_num+1).."="..add_nat_rule_src_netmask)
								luci.sys.exec("uci set nat.nat.index_"..(nat_num+1).."="..add_nat_rule_index)
								luci.sys.exec("uci set nat.nat.nat_num="..(nat_num+1))
							else
								luci.sys.exec("iptables -t nat -I PREROUTING -d "..add_nat_rule_src_ip.." -j DNAT --to "..add_nat_rule_dst_ip)
								luci.sys.exec("uci set nat.nat.srcip_"..(nat_num+1).."="..add_nat_rule_src_ip)
								luci.sys.exec("uci set nat.nat.dstip_"..(nat_num+1).."="..add_nat_rule_dst_ip)
								luci.sys.exec("uci set nat.nat.type_"..(nat_num+1).."="..add_nat_rule_type)
								
								luci.sys.exec("uci set nat.nat.srcnetmask_"..(nat_num+1).."="..add_nat_rule_src_netmask)
								luci.sys.exec("uci set nat.nat.index_"..(nat_num+1).."="..add_nat_rule_index)
								luci.sys.exec("uci set nat.nat.nat_num="..(nat_num+1))
							end	
			end
	else
		--luci.sys.exec("itabbles -t nat -F")
		loop_act_black_ip(add_nat_rule_src_ip,add_nat_rule_dst_ip,add_nat_rule_type,nat_num)
		luci.sys.exec("uci set nat.nat.nat_num="..(nat_num-1))
	end

	luci.sys.exec("uci commit secure")
	return_write(0)
end



function loop_act_black_ip(ipaddr,sevpro,sevport,number)

	local loop_r = 0
	
	for num=1,tonumber(number) do

		if tonumber(number) == tonumber(num) then 

			
			luci.sys.exec("uci delete nat.nat.srcip_"..num)
			luci.sys.exec("uci delete nat.nat.dstip_"..num)
			luci.sys.exec("uci delete nat.nat.type_"..num)
			luci.sys.exec("uci delete nat.nat.srcnetmask_"..num)
			luci.sys.exec("uci delete nat.nat.index_"..num)
			if sevport == 'SNAT' then 
				luci.sys.exec("iptables -t nat -D POSTROUTING -s "..ipaddr.." -j "..sevport.." --to "..sevpro)
			else
				luci.sys.exec("iptables -t nat -D PREROUTING -d "..ipaddr.." -j "..sevport.." --to "..sevpro)
			end
			
		else		 

			local ip_str = "srcip_"..num
			local dstip_str = "dstip_"..num
			local type_str = "type_"..num
			local mask_str = "srcnetmask_"..num
			local index_str = "index_"..num
			local str = uci.get("secure","ip",ip_str)
			
			ip_addr = uci.get("nat","nat",ip_str)
			dst_addr = uci.get("nat","nat",dstip_str)
			type_addr = uci.get("nat","nat",type_str)
			mask_addr = uci.get("nat","nat",mask_str)
			index_addr = uci.get("nat","nat",index_str)
			if ip_addr == ipaddr and sevpro == dst_addr and sevport == type_addr or  1 == loop_r then 
				if 0 == loop_r then 
					
					if sevport == 'SNAT' then 
						luci.sys.exec("iptables -t nat -D POSTROUTING -s "..ipaddr.." -j "..sevport.." --to "..sevpro)
					else
						luci.sys.exec("iptables -t nat -D PREROUTING -d "..ipaddr.." -j "..sevport.." --to "..sevpro)
					end
				end
				
				ip_addr = uci.get("nat","nat","srcip_"..(num+1))
				dst_addr = uci.get("nat","nat","dstip_"..(num+1))
				type_addr = uci.get("nat","nat","type_"..(num+1))
				mask_addr = uci.get("nat","nat","srcnetmask_"..(num+1))
				index_addr = uci.get("nat","nat","index_"..(num+1))

				luci.sys.exec("uci set nat.nat."..ip_str.."="..ip_addr)
				luci.sys.exec("uci set nat.nat."..dstip_str.."="..dst_addr)
				luci.sys.exec("uci set nat.nat."..type_addr.."="..type_addr)
				luci.sys.exec("uci set nat.nat."..mask_str.."="..mask_addr)
				luci.sys.exec("uci set nat.nat."..index_str.."="..index_addr)
				loop_r = 1
			end
			
		end
		
	end		
	
end
--]]

function qos_config()

	local uci = require "luci.model.uci".cursor()
	local lte_qos_cid_id = luci.http.formvalue('value1')
	local lte_qos_qci_id = luci.http.formvalue('value2')
	local lte_qos_dlgbr_id = luci.http.formvalue('value3')
	local lte_qos_ulgbr_id = luci.http.formvalue('value4')
	local lte_qos_dlmbr_id = luci.http.formvalue('value5')
	local lte_qos_ulmbr_id = luci.http.formvalue('value6')
	local lte_qos_action = luci.http.formvalue('value7')
	if lte_qos_action == '0' then 
			
			luci.sys.exec("ltectl removeattach")
			luci.sys.exec("usleep 100000")
			
			luci.sys.exec("uci set epsqos.qos.lte_qos_cid="..lte_qos_cid_id)
			luci.sys.exec("uci set epsqos.qos.lte_qos_qci="..lte_qos_qci_id)
			luci.sys.exec("uci set epsqos.qos.lte_qos_dlgbr="..lte_qos_dlgbr_id)	
			luci.sys.exec("uci set epsqos.qos.lte_qos_ulgbr="..lte_qos_ulgbr_id)
			luci.sys.exec("uci set epsqos.qos.lte_qos_dlmbr="..lte_qos_dlmbr_id)
			luci.sys.exec("uci set epsqos.qos.lte_qos_ulmbr="..lte_qos_ulmbr_id)
			luci.sys.exec("echo "..lte_qos_cid_id.." > /tmp/uci/iQosCid")
			luci.sys.exec("echo "..lte_qos_qci_id.." > /tmp/uci/iQosQci")
			luci.sys.exec("echo "..lte_qos_dlgbr_id.." > /tmp/uci/iQosDlGbr")
			luci.sys.exec("echo "..lte_qos_ulgbr_id.." > /tmp/uci/iQosUlGbr")
			luci.sys.exec("echo "..lte_qos_dlmbr_id.." > /tmp/uci/iQosDlMbr")
			luci.sys.exec("echo "..lte_qos_ulmbr_id.." > /tmp/uci/iQosUlMbr")
			luci.sys.exec("ltectl setepsqos $(uci get epsqos.qos.lte_qos_cid) $(uci get epsqos.qos.lte_qos_qci) $(uci get epsqos.qos.lte_qos_dlgbr) $(uci get epsqos.qos.lte_qos_ulgbr) $(uci get epsqos.qos.lte_qos_dlmbr) $(uci get epsqos.qos.lte_qos_ulmbr)")
	else
			luci.sys.exec("ltectl setepsqos $(uci get epsqos.qos.lte_qos_cid)")
			luci.sys.exec("uci delete epsqos.qos.lte_qos_cid="..lte_qos_cid_id)
			luci.sys.exec("uci delete epsqos.qos.lte_qos_qci="..lte_qos_qci_id)
			luci.sys.exec("uci delete epsqos.qos.lte_qos_dlgbr="..lte_qos_dlgbr_id)	
			luci.sys.exec("uci delete epsqos.qos.lte_qos_ulgbr="..lte_qos_ulgbr_id)
			luci.sys.exec("uci delete epsqos.qos.lte_qos_dlmbr="..lte_qos_dlmbr_id)
			luci.sys.exec("uci delete epsqos.qos.lte_qos_ulmbr="..lte_qos_ulmbr_id)
			luci.sys.exec("echo 0 > /tmp/uci/iQosCid")
			luci.sys.exec("echo 0 > /tmp/uci/iQosQci")
			luci.sys.exec("echo 0 > /tmp/uci/iQosDlGbr")
			luci.sys.exec("echo 0 > /tmp/uci/iQosUlGbr")
			luci.sys.exec("echo 0 > /tmp/uci/iQosDlMbr")
			luci.sys.exec("echo 0 > /tmp/uci/iQosUlMbr")
	end
	luci.sys.exec("uci commit epsqos")
	return_write(0)
end


function lte_lock_frequency()

	  local lte_lock_frequency_sw  = luci.http.formvalue('param1')	
	  local lte_frequency          = luci.http.formvalue('param2')
	  local lte_physical_community = luci.http.formvalue('param3')
	  local lte_bandstart          = luci.http.formvalue('param4')
	  local lte_bandend            = luci.http.formvalue('param5')

	  local libluas = require "libluas".lua_lockfrequency_set(lte_lock_frequency_sw,lte_frequency,lte_physical_community,lte_bandstart,lte_bandend)

	if -1 == libluas then
		return_write(-1)
	else
		luci.sys.exec("uci set lte.frequency.frequency="..lte_frequency)
	  luci.sys.exec("uci set lte.frequency.lock_sw="..lte_lock_frequency_sw)
	  luci.sys.exec("uci set lte.frequency.lock_community="..lte_physical_community)
	  luci.sys.exec("uci set lte.frequency.bandstart="..lte_bandstart)
	  luci.sys.exec("uci set lte.frequency.bandend="..lte_bandend)
	  luci.sys.exec("uci commit lte")

      return_write(0)
	
	  end
	
end

function lte_lock_frequency2()

	  local lte_lock_frequency_sw  = luci.http.formvalue('param1')	
	  local lte_frequency          = luci.http.formvalue('param2')
	  local lte_physical_community = luci.http.formvalue('param3')
	  local lte_bandstart          = luci.http.formvalue('param4')
	  local lte_bandend            = luci.http.formvalue('param5')

	  local libluas = require "libluas2".lua_lockfrequency_set(lte_lock_frequency_sw,lte_frequency,lte_physical_community,lte_bandstart,lte_bandend)

	if -1 == libluas then
		return_write(-1)
	else
		luci.sys.exec("uci set lte.frequency.frequency2="..lte_frequency)
	  luci.sys.exec("uci set lte.frequency.lock_sw2="..lte_lock_frequency_sw)
	  luci.sys.exec("uci set lte.frequency.lock_community2="..lte_physical_community)
	  luci.sys.exec("uci set lte.frequency.bandstart2="..lte_bandstart)
	  luci.sys.exec("uci set lte.frequency.bandend2="..lte_bandend)
	  luci.sys.exec("uci commit lte")

      return_write(0)
	
	  end
	
end

function lte_higt_simlock_sw()
	
	local simlock_sw = luci.http.formvalue('port_val')

	local libluas = require "libluas".lua_simlock_set(simlock_sw)

	if -1 == libluas then
		return_write(-1)
	else 
		return_write(0)
	end
	
end

function lte_higt_pin_opt()

	local lte_pin_opt    = luci.http.formvalue('port_val')
	local lte_PinCodeOld = luci.http.formvalue('port_val2')
	local lte_PinCodeNew = luci.http.formvalue('port_val3')
	local libluas = require "libluas".lua_pin_opt(lte_pin_opt,lte_PinCodeOld,lte_PinCodeNew)

	if -1 == libluas then
		return_write(-1)
	else 
		return_write(0)
	end
	
end

function last_switch()
	
	local switch 	= luci.http.formvalue('port_val')
	local libluas
	libluas = require "libluas".lua_lastroute_set(switch)
	if '-1' == libluas then
		return_write(-1)
	else 
		luci.sys.exec("uci set config.lastroute.switch="..switch)
		luci.sys.exec("uci commit config")
	
		return_write(0)
	end
    
end

function last_switch2()
	
	local switch 	= luci.http.formvalue('port_val')
	local libluas
	libluas = require "libluas2".lua_lastroute_set(switch)
	if '-1' == libluas then
		return_write(-1)
	else 
		luci.sys.exec("uci set config.lastroute.switch2="..switch)
		luci.sys.exec("uci commit config")
	
		return_write(0)
	end
    
end

function lte_upgrade()
	local lte_version_step = 1
    local h    = require "luci.http"
    local io   = require "nixio"
    local flag = true
    local run  = true
    local fd   = nil
    h.setfilehandler(
        function(field, chunk, eof)
          
            if not field or not run then 
            	return 
            end
            
            
            if not fd then
                fd = io.open(version_tmp, "w")
            end
            
            fd:write(chunk)
             
            if eof and fd then
                fd:close()
                fd = nil
            end    
      
          end

    )

	local version_step = tonumber(luci.http.formvalue("version_step") or 1)
		
	if version_step ~= 2 then	
		luci.template.render("tau_view/lteupgrade", {nil})
	else
		luci.template.render("tau_view/upgrade", {nil})
		luci.sys.exec("/sbin/lteupdate &")
	end
end
function lte_auto_connect_time()

	local value = tonumber(luci.http.formvalue('param2'))
	
	luci.sys.exec("uci set lte.set.basic_connecttime="..value)
	luci.sys.exec("uci commit lte")

	luci.sys.exec("killall -9 lteauto")
	luci.sys.exec("sleep 1")

	if value == 0 then
		io.popen("/sbin/lteauto &")
	else
		io.popen("/sbin/lteauto "..value.." &")
	end

	return_write(0)	
end

function lte_auto_connect_time2()

	local value = tonumber(luci.http.formvalue('param2'))
	
	luci.sys.exec("uci set lte.set.basic_connecttime2="..value)
	luci.sys.exec("uci commit lte")

	luci.sys.exec("killall -9 lteauto2")
	luci.sys.exec("sleep 1")

	if value == 0 then
		io.popen("/sbin/lteauto2 &")
	else
		io.popen("/sbin/lteauto2 "..value.." &")
	end

	return_write(0)	
end

function sim_switch()
	local sim_switch 	= luci.http.formvalue('value1')
	
	local sim_imsi = luci.http.formvalue('value2')
	local sim_mnc_len = luci.http.formvalue('value3')
	local sim_suauthpara = luci.http.formvalue('value4')
	local sim_sualg = luci.http.formvalue('value5')
	local sim_key = luci.http.formvalue('value6')
	local sim_op = luci.http.formvalue('value7')
		
	local libluas = require "libluas".lua_lte_simswitch_set(sim_switch,sim_imsi,sim_mnc_len,sim_suauthpara,sim_sualg,sim_key,sim_op)
	if -1 == libluas then
		return_write(-1)
	else 
		if '0' == sim_switch then	
			luci.sys.exec("uci set lte.simswitch.imsi="..sim_imsi)
			luci.sys.exec("uci set lte.simswitch.mnc_len="..sim_mnc_len)
			luci.sys.exec("uci set lte.simswitch.suauthpara="..sim_suauthpara)
			luci.sys.exec("uci set lte.simswitch.sualg="..sim_sualg)
			luci.sys.exec("uci set lte.simswitch.key="..sim_key)
			luci.sys.exec("uci set lte.simswitch.op="..sim_op)
		end
	
		luci.sys.exec("uci set lte.simswitch.switch="..sim_switch)
		luci.sys.exec("uci commit lte")
	
		return_write(0)
	end
end

function sim_switch2()
	local sim_switch 	= luci.http.formvalue('value1')
	
	local sim_imsi = luci.http.formvalue('value2')
	local sim_mnc_len = luci.http.formvalue('value3')
	local sim_suauthpara = luci.http.formvalue('value4')
	local sim_sualg = luci.http.formvalue('value5')
	local sim_key = luci.http.formvalue('value6')
	local sim_op = luci.http.formvalue('value7')
		
	local libluas = require "libluas2".lua_lte_simswitch_set(sim_switch,sim_imsi,sim_mnc_len,sim_suauthpara,sim_sualg,sim_key,sim_op)

	if '-1' == libluas then
		return_write(-1)
	else 
		if '0' == sim_switch then	
			luci.sys.exec("uci set lte.simswitch.imsi2="..sim_imsi)
			luci.sys.exec("uci set lte.simswitch.mnc_len2="..sim_mnc_len)
			luci.sys.exec("uci set lte.simswitch.suauthpara2="..sim_suauthpara)
			luci.sys.exec("uci set lte.simswitch.sualg2="..sim_sualg)
			luci.sys.exec("uci set lte.simswitch.key2="..sim_key)
			luci.sys.exec("uci set lte.simswitch.op2="..sim_op)
		end
	
		luci.sys.exec("uci set lte.simswitch.switch2="..sim_switch)
		luci.sys.exec("uci commit lte")
	
		return_write(0)
	end
end

function lte_unlock()

	local unlock_num = luci.http.formvalue('value1')
	
	luci.sys.exec("uci set lte.module_info.unlock_num="..tostring(unlock_num))
	luci.sys.exec("uci commit lte")

	luci.sys.exec("ltectl nvdatalock "..unlock_num)

	return_write(0)	
end

function lte_unlock2()

	local unlock_num = luci.http.formvalue('value1')
	
	luci.sys.exec("uci set lte.module_info.unlock_num2="..tostring(unlock_num))
	luci.sys.exec("uci commit lte")

	luci.sys.exec("ltectl2 nvdatalock "..unlock_num)

	return_write(0)	
end

function return_write(val)
	
	local o = {}
	
	if 0 == val then
		o['result'] = 'success'
	else
		o['result'] = 'failed'
	end
	
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
