#!/bin/sh

#auth:zhz
#date:2013/12/18
#fun:����ƽ̨������Դ�ļ�������
#$1:Ҫ������Դ��URL
#$2:���豸�ϵĴ洢·��
#$3:����ƽ̨�ϵõ������°汾��,���سɹ����д��/tmp/cloud_src_ver�ļ���
#$4:������Դ�ı�־: �ֱ�Ϊtpl��product��adv
#$5:�����������ļ�����



if [ $# != "5" ]; then
	echo "parameters is less than 5"
	exit 1
fi

CLOUD_SRC_VER_STORE=/tmp/cloud_src_ver
TPL=tpl
PRODUCT=product
AD=adv
HTTPDOWNLOAD=/usr/sbin/httpdownload

#httpdownloadָ��ִ�з���ֵ
download_return=1
iLoop=0

while [ "$iLoop" -lt "1" ]
do
	wget -P /tmp/ $1 -q #�����ļ���
	download_return=$?			#�ж����صķ���ֵ
	if [ "$download_return" -eq "0" ]; then
		if [ -e /tmp/$5 ];then
			rm -rf $2*			#��ɾ���ɵ��ļ�
			unzip /tmp/$5 -d $2		#��ѹZIP�ļ���ָ��Ŀ¼  ע��Ŀ¼��Լ��
			rm -rf /tmp/$5			#ɾ��ԭʼ�ļ�
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