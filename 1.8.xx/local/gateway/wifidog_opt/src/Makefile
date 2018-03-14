#CC=/home/apenv/ar9344/build/gcc-4.3.3/build_mips/staging_dir/usr/bin/mips-linux-uclibc-gcc
#LD=/home/apenv/ar9344/build/gcc-4.3.3/build_mips/staging_dir/usr/bin/mips-linux-uclibc-ld
#AR=/home/apenv/ar9344/build/gcc-4.3.3/build_mips/staging_dir/usr/bin/mips-linux-uclibc-ar

obj-app = wifidog
obj-dep = util.o
obj-dep += log.o
obj-dep += conf.o
obj-dep += commandline.o
obj-dep += client_list.o
obj-dep += auth_server.o
obj-dep += firewall.o
obj-dep += b64.o
obj-dep += cJSON.o
obj-dep += http.o
obj-dep += redirect_portal.o
obj-dep += cloud_platform.o
obj-dep += gctl_thread.o
obj-dep += gateway.o
obj-dep += client_rdl.o
obj-dep += client_ap.o
obj-dep += client_dct.o
obj-dep += client_aglink.o


ifneq ($(SYSTEM_TYPE),OPENWRT)
ifeq ($(BOARD_TYPE),ap93fus)
obj-dep += $(APP_COMMON)/nvram_flash_ap93.o
else
ifeq ($(MAKE_ENV),GW_ENV)
obj-dep += $(APP_COMMON)/nvram_flash.o $(APP_COMMON)/tool.o
else
obj-dep += $(APP_COMMON)/nvram_flash.o $(APP_COMMON)/flow_control.o $(APP_COMMON)/tool.o
endif
endif
endif
ifeq ($(PCAP_SUPPORT),y)
obj-dep +=gw_pcap.o
endif
DEST_PATH = $(BIN_PATH)/app/bin
ETC_PATH = $(TOPDIR)/build/fsbase/rootfs/etc



ifeq ($(SYSTEM_TYPE),OPENWRT)
CFLAGS += -I$(STAGING_DIR)/usr/include/event2 -I$(GW_INCLUDE) -I$(GW_PAL_INCLUDE) -I$(GW_EXT_APP_INCLUDE) -I$(GW_EXT_NET_INCLUDE) -I../lib/include -I../include 
CFLAGS += -DOPENWRT
CFLAGS += $(AP_FLAG)

else

# add include
CFLAGS += -I../include

# add lib include
CFLAGS += -I../lib/include

# add pa include

CFLAGS += -I$(NET_INCLUDE) -I$(APP_INCLUDE) -I$(APP_INCLUDE)/../../app/libevent/libevent -I$(APP_INCLUDE)/../../app/libevent/libevent/include
endif
CFLAGS += -D_GNU_SOURCE
CFLAGS += -DHAVE_NETSNIFF

ifeq ($(DEV_MEM_TYPE),MEM_128M)
CFLAGS += -DMEM_128M
endif

CFLAGS += -g -O -Wall -Werror

LDFLAGS = -Wl -lrt


ifeq ($(SYSTEM_TYPE),OPENWRT)
LDLIBS = -L$(STAGING_DIR)/usr/lib/libpa -L$(STAGING_DIR)/usr/lib/  -L../lib/ -lhttpd  -lpal -lag_share -levent -l_pa -lpthread -luci
else


ifeq ($(MAKE_ENV),GW_ENV)
LDLIBS = -L../lib -lhttpd -L$(AC_LIB_DIR) -lag_share -levent -L$(DEST_LIB_PATH) -l_pa -lpthread
else
LDLIBS = -L../lib -lhttpd -L$(TOPDIR)/app/lib -lag_share -levent -L$(NET_INCLUDE) -l_pa -lpthread
CFLAGS += $(AP_FLAG)
endif

endif

ifeq ($(PCAP_SUPPORT),y)
LDLIBS += -L$(ROOTFS_LIB_PATH) -lpcap
endif

wifidog: $(obj-dep)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(obj-app) $(obj-dep) $(LDLIBS)
	$(STRIP) -d $(obj-app)

gctl: gctl.o
	$(CC) $(CFLAGS)  -o gctl gctl.o 	
	$(STRIP) -d gctl

build:
	cd ../api/libhttpd && make libhttpd && cd -
	make wifidog
	make gctl
	
clean:
	cd ../api/libhttpd && make clean && cd -
	rm -f *.o wifidog gctl

install:
ifeq ($(MAKE_ENV),GW_ENV)
	cp wifidog $(DEST_SBIN_PATH)
	cp gctl $(DEST_SBIN_PATH)
ifeq ($(BOARD_TYPE), GW_AC)
	cp config/wifidog.pcl.conf $(DEST_ETC_PATH)/wifidog.conf
else
	cp config/wifidog.conf $(DEST_ETC_PATH)
endif
	cp config/wifidog-msg.html $(DEST_ETC_PATH)
	cp config/apple-success.html $(DEST_ETC_PATH)
	cp config/apple-network.html $(DEST_ETC_PATH)
	cp config/maxuser.html $(DEST_ETC_PATH)
	cp config/offline.html $(DEST_ETC_PATH)
	cp config/style.css $(DEST_ETC_PATH)
	cp config/first_page.html $(DEST_ETC_PATH)
	cp config/auth_ok.html $(DEST_ETC_PATH)
	cp config/get_mac.html $(DEST_ETC_PATH) && chmod 777 $(DEST_ETC_PATH)/get_mac.html
else

	cp wifidog $(DEST_PATH)/wifidog
	cp gctl $(DEST_PATH)/gctl
	cp config/wifidog-msg.html $(ETC_PATH)/wifidog-msg.html
	cp config/apple-success.html $(ETC_PATH)/apple-success.html
	cp config/apple-network.html $(ETC_PATH)/apple-network.html
	cp config/maxuser.html $(ETC_PATH)/maxuser.html
	cp config/offline.html $(ETC_PATH)/offline.html
	cp config/style.css $(ETC_PATH)/style.css
	cp config/first_page.html $(ETC_PATH)/first_page.html
	cp config/auth_ok.html $(ETC_PATH)/auth_ok.html
	cp config/get_mac.html $(ETC_PATH)/get_mac.html
ifeq ($(BOARD_TYPE), ap93fus)
	cp config/wifidog.conf $(ETC_PATH)/wifidog.conf
else
	cp config/wifidog.conf $(ETC_PATH)/wifidog.conf-tmp
endif

endif
