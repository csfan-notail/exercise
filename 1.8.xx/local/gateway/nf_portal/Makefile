ifeq ($(SYSTEM_TYPE),OPENWRT)
EXTRA_CFLAGS = -I$(GW_INCLUDE) -I$(GW_EXT_NET_INCLUDE) -DOPENWRT
ifeq ($(AD_SUPPORT),y)
EXTRA_CFLAGS +=-DAD_SUPPORT
endif
ifeq ($(KERNELRELEASE),)
all:							
else
obj-m += nf_portal.o
nf_portal-objs:= nf_portal_module.o nf_portal_main.o \
				nf_portal_ioctl.o nf_portal_ops.o \
				nf_portal_hash.o nf_portal_timer.o		
endif
clean:
	@rm -rf *.o
	@rm -rf nf_portal.ko
	@rm -rf *Module.symvers
	@rm -rf *modules.order
	@rm -rf ipk*

else

EXTRA_CFLAGS = -I$(NET_INCLUDE) 
ifeq ($(AD_SUPPORT),y)
EXTRA_CFLAGS +=-DAD_SUPPORT
endif

obj-m += nf_portal.o
nf_portal-objs:= nf_portal_module.o nf_portal_main.o \
				nf_portal_ioctl.o nf_portal_ops.o \
				nf_portal_hash.o nf_portal_timer.o		
								
ifeq ($(BOARD_TYPE),GW_X86)

EXTRA_CFLAGS := ${EXTRA_CFLAGS} -DGW_X86 -DHAVE_MULTI_WAN

else

ifeq ($(BOARD_TYPE),GW_ARM)

EXTRA_CFLAGS := ${EXTRA_CFLAGS} -DGW_ARM

else

ifeq ($(BOARD_TYPE),GW_AC)

EXTRA_CFLAGS := ${EXTRA_CFLAGS} -DGW_AC

else

DEST_PATH = $(TOPDIR)/build/bin/net/module

include $(MODULE_MAKE)

endif

endif

endif



endif