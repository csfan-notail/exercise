#!/bin/sh
#
# Copyright (c) 2013 The Linux Foundation. All rights reserved.
# Copyright (C) 2009 OpenWrt.org
#
#

led_set_attr() {
	[ -f "/sys/class/leds/$1/$2" ] && echo "$3" > "/sys/class/leds/$1/$2"
}

status_led_set_timer() {
	for led_dir in $status_led; do
		led_set_attr $led_dir "trigger" "timer"
		led_set_attr $led_dir "delay_on" "$1"
		led_set_attr $led_dir "delay_off" "$2"
	done
}

status_led_on() {
	for led_dir in $status_led; do
		led_set_attr $led_dir "trigger" "none"
		led_set_attr $led_dir "brightness" 255
	done
}

status_led_off() {
	for led_dir in $status_led; do
		led_set_attr $led_dir "trigger" "none"
		led_set_attr $led_dir "brightness" 0
	done
}

get_status_led() {

	platform=$(factory_env get BOOT_VERSION | awk -F= '{print $2}')
	
	case "$platform" in
		*ta62_51*)
				status_led="ta62:green:run" #dir name
				;;
		*ta37_22*)
				status_led="ta37:green:system"
				;;
		*ta37_40*)
				status_led="ta37:green:system"
				;;
		*)
				status_led="ta37:green:system"
				;;
	esac

}

status_led_blink() {

	status_led_off
	status_led_set_timer 100 100
	sleep 10
	status_led_off
	status_led_on

}

status_led=""
get_status_led
status_led_blink


