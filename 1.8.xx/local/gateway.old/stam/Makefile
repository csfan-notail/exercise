
obj-app = stam 

ifeq ($(BOARD_TYPE),ap93fus)
obj-dep = stam_iw.o stam.o $(TOPDIR)/app/common/nvram_flash_ap93.o   $(TOPDIR)/app/common/log.o
else
ifeq ($(SYS_TYPE),OPENWRT)
obj-dep = stam_iw.o stam.o 

else
obj-dep = stam_iw.o stam.o $(TOPDIR)/app/common/nvram_flash.o   $(TOPDIR)/app/common/log.o
endif
endif


ifeq ($(SYS_TYPE),OPENWRT)
CFLAGS  =  -D_BOXLL_ -I$(GW_INCLUDE) -DOPENWRT -I$(STAGING_DIR)/usr/include/event2
LDLIBS = -lpal  -lrt -lag_share -lcrypt -lpthread  -levent -liw
LDFLAGS = -Wall

build: $(obj-dep)
	@echo "BUILD         $(obj-app)"
	@$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -o $(obj-app) $(obj-dep) $(LDLIBS)
	@$(STRIP) -d $(obj-app)

install:
	@echo "INSTALL       $(obj-app)"
	@cp $(obj-app) $(DEST_PATH)/$(DEST_NAME)

#For "find && rm" operation.
export REMOVE_PATTERN= \( -name '*.[oas]' \
                        -o -name '*.ko'  \
                        -o -name 'modules.order' \
                        -o -name 'Module.symvers' \
                        -o -name '*.cmd' \
                        -o -name '*.d'   \
                        -o -name '*.tmp' \
                        -o -name '*.scc' \
                        -o -name '*.tmp_versions' \
                        -o -name '*.depend' \
                        -o -name '*.mod.c' \)
clean:
	@echo "CLEAN         $(obj-app)"
	@find . -follow $(REMOVE_PATTERN) | xargs rm -fr
	@rm -f $(obj-app)


else

#CFLAGS  = -I$(APP_INCLUDE) $(AP_FLAG)
CFLAGS  =  -D_BOXLL_ -I$(APP_INCLUDE) $(AP_FLAG) -I$(TOPDIR)/app/libevent/libevent -I$(TOPDIR)/app/libevent/libevent/include -I$(TOPDIR)/app/powerap/ap-agent/ifWifi  -I$(TOPDIR)/wifi/wireless_tools.29 -I$(TOPDIR)/wifi/gbcom_priv/include
LDFLAGS = -Wall

#LDFLAGS += -L$(AC_LIB_DIR) -lrt -lag_share -levent -lcrypt -lpthread -lwifi_pal -L$(TOPDIR)/wifi/wireless_tools.29 -liw
LDLIBS = -L$(AC_LIB_DIR) -lrt -lag_share -lcrypt -lpthread -lwifi_pal -levent -L$(TOPDIR)/wifi/wireless_tools.29 -liw
# -lcommon 
DEST_PATH = $(BIN_PATH)/app/bin
DEST_NAME = $(obj-app)

include $(TOPDIR)/build/app.mk
endif
