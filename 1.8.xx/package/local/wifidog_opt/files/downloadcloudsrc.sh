#!/bin/sh

#auth:zhz
#date:2013/12/18
#fun:从云平台下载资源文件到网关
#$1:要下载资源的URL
#$2:在设备上的存储路径
#$3:从云平台上得到的最新版本号,下载成功后才写入/tmp/cloud_src_ver文件中
#$4:三种资源的标志: 分别为tpl、product、adv
#$5:下载下来的文件名称



if [ $# != "5" ]; then
	echo "parameters is less than 5"
	exit 1
fi

CLOUD_SRC_VER_STORE=/tmp/cloud_src_ver
TPL=tpl
PRODUCT=product
AD=adv
HTTPDOWNLOAD=/usr/sbin/httpdownload

#httpdownload指令执行返回值
download_return=1
iLoop=0

while [ "$iLoop" -lt "1" ]
do
	wget -P /tmp/ $1 -q #下载文件中
	download_return=$?			#判断下载的返回值
	if [ "$download_return" -eq "0" ]; then
		if [ -e /tmp/$5 ];then
			rm -rf $2*			#先删除旧的文件
			unzip /tmp/$5 -d $2		#加压ZIP文件到指定目录  注意目录的约定
			rm -rf /tmp/$5			#删除原始文件
			if [ "$4" = "$TPL" ]; then
				sed -i '1c\'$3 $CLOUD_SRC_VER_STORE
			elif [ "$4" = "$PRODUCT" ]; then
				sed -i '2c\'$3 $CLOUD_SRC_VER_STORE
			elif [ "$4" = "$AD" ]; then
				sed -i '3c\'$3 $CLOUD_SRC_VER_STORE
			fi
			break
		fi
	fi
	iLoop=$(($iLoop+1))
done