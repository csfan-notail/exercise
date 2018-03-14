
ifeq ($(BOARD_TYPE),GW_X86)
	LIB_DIR=$(AC_LIB_DIR)
	ROOTFS_LIB_DIR=$(AC_LIB_DIR)
else
	LIB_DIR=$(TOPDIR)/app/lib
	ROOTFS_LIB_DIR=$(TOPDIR)/build/fsbase/rootfs/lib
endif

ifeq ($(BOARD_TYPE),GW_X86)
CFLAGS  += -I$(APP_INCLUDE) $(AP_FLAG) $(AC_BUILD_PREDEFINE) -I$(SRCDIR)/../libevent/libevent \
          -I$(SRCDIR)/../libevent/libevent/include 
LDLIBS  += -L$(LIB_DIR) -lpthread -lag_share -levent 


else ifeq ($(SYSTEM_TYPE),OPENWRT)
CFLAGS += -DOPENWRT -D_BOXLL_ -I$(STAGING_DIR)/usr/include/event2  -I$(GW_INCLUDE) -I$(GW_EXT_APP_INCLUDE) -I$(GW_PAL_INCLUDE) 
LDLIBS +=  -L$(STAGING_DIR)/usr/lib  -lpthread   -levent -lpal -liw

LDLIBS +=  -L$(STAGING_DIR)/usr/lib/libevent -lag_share 

else
WLAN_CFLAGS  = -I$(TOPDIR)/wifi/wlan/os/linux/include -I$(TOPDIR)/wifi/wlan/include -I$(TOPDIR)/wifi/wlan/lmac/ath_dev
WLAN_CFLAGS += -I$(TOPDIR)/wifi/wireless_tools.29 -I$(TOPDIR)/wifi/gbcom_priv/include
CFLAGS += -I$(APP_INCLUDE) $(AP_FLAG) $(AC_BUILD_PREDEFINE) -I$(TOPDIR)/app/libevent/libevent \
          -I$(TOPDIR)/app/libevent/libevent/include  
CFLAGS += $(WLAN_CFLAGS) -D_BOXLL_
LDLIBS += -L$(LIB_DIR) -L$(ROOTFS_LIB_DIR) -lpthread -lwifi_pal -lag_share -levent \
		  -L$(TOPDIR)/wifi/wireless_tools.29 -liw
endif

ifeq ($(BOARD_TYPE),ap93fus)
LDFLAGS = -Wl -lrt
else
LDFLAGS = -Wl -lrt
endif


DEST_NAME = $(obj-app)

obj-app = aglink 
#obj-dep = ag_module.o aglink.o $(TOPDIR)/app/common/nvram_flash.o

ifeq ($(BOARD_TYPE),ap93fus)
obj-share = $(TOPDIR)/app/common/log.o
obj-dep = aglink.o ag_iw.o ag_module.o $(TOPDIR)/app/common/nvram_flash_ap93.o $(TOPDIR)/app/common/log.o
DEST_PATH = $(BIN_PATH)/app/bin
IPC_CONF_PATH = $(TOPDIR)/build/fsbase/rootfs/etc
endif

ifeq ($(BOARD_TYPE),db12x)
obj-share = $(TOPDIR)/app/common/log.o
obj-dep = aglink.o ag_iw.o ag_module.o  $(TOPDIR)/app/common/nvram_flash.o $(TOPDIR)/app/common/log.o
DEST_PATH = $(BIN_PATH)/app/bin
IPC_CONF_PATH = $(TOPDIR)/build/fsbase/rootfs/etc
endif

ifeq ($(BOARD_TYPE),ta40)
obj-share = $(TOPDIR)/app/common/log.o
obj-dep = aglink.o ag_iw.o ag_module.o  $(TOPDIR)/app/common/nvram_flash.o $(TOPDIR)/app/common/log.o
DEST_PATH = $(BIN_PATH)/app/bin
IPC_CONF_PATH = $(TOPDIR)/build/fsbase/rootfs/etc
endif

ifeq ($(BOARD_TYPE),GW_X86)
obj-share = $(APP_COMMON)/log.o
obj-dep = aglink.o ag_module.o  $(APP_COMMON)/nvram_flash.o
DEST_PATH = $(DEST_SBIN_PATH)
IPC_CONF_PATH = $(DEST_ETC_PATH)
endif

#obj-dep += ag_wlan.o ag_iw.o ag_event.o ag_stadct.o
#obj-dep += ag_wlan.o ag_iw.o

ifeq ($(SYSTEM_TYPE),OPENWRT)
obj-dep = aglink.o ag_module.o ag_iw.o ag_module_share.o
endif
all: build

build: libag_share.so aglink aglinkcli



ifeq ($(SYSTEM_TYPE),OPENWRT)

install:
	@echo "INSTALL       aglink libag_share.so"
	#@cp aglink $(DEST_PATH)/$(DEST_NAME)
	#@cp libag_share.so $(LIB_DIR) && cp libag_share.so $(ROOTFS_LIB_DIR)
	
clean:
	@echo "CLEAN         aglink libag_share.so"
	@rm -f *.o aglink libag_share.so 
	@rm -rf ipkg*

libag_share.so:
	@echo "BUILD        libag_share.so"
	@$(CC) $(CFLAGS) -L$(STAGING_DIR)/usr/lib/ -levent -fPIC -shared -o libag_share.so  ag_module_share.c
	@cp libag_share.so $(STAGING_DIR)/usr/lib/	


aglink: $(obj-dep)
	@echo "BUILD         $(obj-app)"
	@$(CC) $(CFLAGS) $(LDFLAGS)  -o $(obj-app) $(obj-dep) $(LDLIBS)
	#@$(STRIP) -d $(obj-app)
	#@cp aglink $(DEST_PATH)/$(DEST_NAME)
	#@cp ag_ipc.conf $(IPC_CONF_PATH)

aglinkcli: aglinkcli.o
	@echo "BUILD         aglinkcli_4"
	@$(CC) $(CFLAGS) $(LDFLAGS)  -o aglinkcli aglinkcli.o $(LDLIBS)
	#@$(STRIP) -d aglinkcli
	#@cp aglinkcli $(DEST_PATH)/$(DEST_NAME)	
else





install:
	@echo "INSTALL       aglink aglinckcli libag_share.so"
	@cp aglink  $(DEST_PATH)/$(DEST_NAME)
	@cp aglinkcli  $(DEST_PATH)
	@cp libag_share.so $(LIB_DIR) && cp libag_share.so $(ROOTFS_LIB_DIR)
	
clean:
	@echo "CLEAN         aglink libag_share.so"
	@rm -f *.o aglink aglinkcli libag_share.so

libag_share.so:
	@echo "BUILD        libag_share.so"
	@$(CC) $(CFLAGS) -L$(LIB_DIR) -levent -fPIC -shared -o libag_share.so $(obj-share) ag_module_share.c
	@pwd
	@cp libag_share.so $(LIB_DIR) && cp libag_share.so $(ROOTFS_LIB_DIR)	
			
aglink: $(obj-dep)
	@echo "BUILD         $(obj-app)"
	@$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -o $(obj-app) $(obj-dep) $(LDLIBS)
	@$(STRIP) -d $(obj-app)
	@cp aglink $(DEST_PATH)/$(DEST_NAME)
	@cp ag_ipc.conf $(IPC_CONF_PATH)

aglinkcli: aglinkcli.o
	@echo "BUILD         aglinkcli_4"
	@$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -o aglinkcli aglinkcli.o $(LDLIBS)
	@$(STRIP) -d aglinkcli
	@cp aglinkcli $(DEST_PATH)/$(DEST_NAME)
endif	
