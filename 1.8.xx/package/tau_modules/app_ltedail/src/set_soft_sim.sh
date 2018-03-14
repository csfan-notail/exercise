#!/bin/sh
if [ "1"x = "$1"x ];then
	echo -en "AT^HVSST=11,0\r\n">/dev/ttyUSB8
	sleep 1
	echo -en "AT^HVSST=1,1\r\n">/dev/ttyUSB8
else
	sleep 1
	echo -en "AT^HVSSSINFO=\"$2\",$3,\"$4\",\"$5\"\r\n">/dev/ttyUSB8
fi
