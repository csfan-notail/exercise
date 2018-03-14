#!/bin/sh /etc/rc.common 


local g_free_auth=0
local p_free_auth
run_free_auth()
{
	config_get g_free_auth $1 global_free_auth
	config_get p_free_auth $1 port_free_auth
}

wifidog_status=`cat /tmp/wifidog.status`

config_init()
{
	config_load gateway
	config_foreach run_free_auth gateway
	#echo $g_free_auth
	#echo $p_free_auth
	sleep 1
	if [ "$g_free_auth" = "1" ] || [ "$wifidog_status" != "1"  ] ; then
		pactl free_auth enable
	else
		pactl free_auth disable
	fi

	pactl free_auth_list port flush
	for port in $p_free_auth
	do 
	echo $port
	pactl free_auth_list port add $port
	done	
}

start()
{
	config_init
}
