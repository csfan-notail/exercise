
obj-app = pactl
obj-dep = nf_portal_ioctl_main.o nf_portal_ioctl_cmd.o nf_portal_ioctl_ops.o lib_pa.o

ifneq ($(SYSTEM_TYPE),OPENWRT)
CFLAGS  = -I$(APP_INCLUDE) -I$(NET_INCLUDE)
LDFLAGS = -Wl -static

ifeq ($(BOARD_TYPE),GW_X86)
CFLAGS  += -DGW_X86 -DHAVE_MULTI_WAN
endif

ifeq ($(BOARD_TYPE),GW_ARM)
CFLAGS  += -DGW_ARM
LDFLAGS = -static
endif

DEST_PATH = $(TOPDIR)/build/bin/net/bin
DEST_NAME = $(obj-app)

#include $(TOPDIR)/build/app.mk
LIB_NAME = lib_pa.a
build: $(obj-dep)
ifeq ($(MAKE_ENV),GW_ENV)
	@echo "BUILD         $(obj-app) $(LIB_NAME)"
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $(obj-app) $(obj-dep) $(LDLIBS)
	@$(AR) crus $(LIB_NAME) lib_pa.o nf_portal_ioctl_ops.o
	@chmod 755   $(LIB_NAME)
#	@$(STRIP) $(obj-app)
	@echo "INSTALL       $(obj-app) $(LIB_NAME)"
	${INSTALL} -d $(DEST_SBIN_PATH)
	${INSTALL} -d $(DEST_LIB_PATH)
	@cp $(obj-app) $(DEST_SBIN_PATH)
	@cp $(LIB_NAME) $(DEST_LIB_PATH)
	@rm -f $(obj-dep)
	@rm -f $(obj-app)
	@rm -f $(LIB_NAME)
else
	@echo "BUILD         $(obj-app) $(LIB_NAME)"
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $(obj-app) $(obj-dep) $(LDLIBS)
	@$(AR) crus $(LIB_NAME) lib_pa.o nf_portal_ioctl_ops.o
	@chmod 755   $(LIB_NAME)
	@$(STRIP) $(obj-app)
	@cp $(LIB_NAME) $(NET_INCLUDE)
endif


install:
	@echo "INSTALL       $(obj-app) $(LIB_NAME)"
	@cp $(obj-app) $(DEST_PATH)/$(DEST_NAME)
	@cp $(LIB_NAME) $(NET_INCLUDE)

clean:
	@echo "CLEAN         $(obj-app) $(LIB_NAME)"
ifneq ($(MAKE_ENV),GW_ENV)
	@find . -follow $(REMOVE_PATTERN) | xargs rm -fr
endif
	@rm -f $(obj-app)
	@rm -f $(LIB_NAME)

else
CFLAGS  += -I$(GW_INCLUDE) -I$(GW_EXT_NET_INCLUDE) -DOPENWRT
LDFLAGS += -Wl -static


LIB_NAME = lib_pa.a
build: $(obj-dep)
	@echo "BUILD         $(obj-app) $(LIB_NAME)"
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $(obj-app) $(obj-dep) $(LDLIBS)
	@$(AR) crus $(LIB_NAME) lib_pa.o nf_portal_ioctl_ops.o

install:
	@echo "INSTALL       $(obj-app) $(LIB_NAME)"

clean:
	@echo "CLEAN         $(obj-app) $(LIB_NAME)"
	@rm -rf *.o 
	@rm -rf $(obj-app) $(LIB_NAME)
	@rm -rf ipk*


endif
