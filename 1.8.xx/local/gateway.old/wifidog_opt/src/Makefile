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
obj-dep += sta_key.o

# add pa include
CFLAGS += -I$(STAGING_DIR)/usr/include/event2 -I$(GW_INCLUDE) -I../lib/include -I../include 
CFLAGS += -DOPENWRT
CFLAGS += -D_GNU_SOURCE
CFLAGS += -DHAVE_NETSNIFF

ifeq ($(DEV_MEM_TYPE),MEM_128M)
CFLAGS += -DMEM_128M
endif

CFLAGS += -g -O -Wall -Werror

LDFLAGS = -Wl -lrt

LDLIBS = -L$(STAGING_DIR)/usr/lib/libpa -L$(STAGING_DIR)/usr/lib/  -L../lib/ -lhttpd  -lpal -lag_share -levent -l_pa -lpthread -luci

wifidog: $(obj-dep)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(obj-app) $(obj-dep) $(LDLIBS)
	$(STRIP) -d $(obj-app)

gctl: gctl.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o gctl gctl.o $(LDLIBS)	
	$(STRIP) -d gctl

build:
	cd ../api/libhttpd && make libhttpd && cd -
	make wifidog
	make gctl
	
clean:
	cd ../api/libhttpd && make clean && cd -
	rm -f *.o wifidog gctl
	rm -f ../lib/libhttpd.a

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
	cp config/maxuser.html $(DEST_ETC_PATH)
	cp config/offline.html $(DEST_ETC_PATH)
	cp config/style.css $(DEST_ETC_PATH)
	cp config/first_page.html $(DEST_ETC_PATH)
	cp config/auth_ok.html $(DEST_ETC_PATH)
else

	cp wifidog $(DEST_PATH)/wifidog
	cp gctl $(DEST_PATH)/gctl
	cp config/wifidog-msg.html $(ETC_PATH)/wifidog-msg.html
	cp config/apple-success.html $(ETC_PATH)/apple-success.html
	cp config/maxuser.html $(ETC_PATH)/maxuser.html
	cp config/offline.html $(ETC_PATH)/offline.html
	cp config/style.css $(ETC_PATH)/style.css
	cp config/first_page.html $(ETC_PATH)/first_page.html
	cp config/auth_ok.html $(ETC_PATH)/auth_ok.html
ifeq ($(BOARD_TYPE), ap93fus)
	cp config/wifidog.conf $(ETC_PATH)/wifidog.conf
else
	cp config/wifidog.conf $(ETC_PATH)/wifidog.conf-tmp
endif

endif
