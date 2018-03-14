ifeq ($(SYSTEM_TYPE),OPENWRT)
EXTRA_CFLAGS = -I$(GW_INCLUDE) -I$(GW_EXT_NET_INCLUDE) -DOPENWRT 

ifeq ($(KERNELRELEASE),)
all:
else
obj-m += flowctrl.o
flowctrl-objs:= fc_main.o  fc_module.o fc_ioctl.o \
								fc_queue.o fc_hash.o   fc_timer.o\
								fc_sta.o   fc_vap.o    fc_vlan.o fc_ssid.o  fc_ap.o fc_event.o  fc_white_list.o
endif								

else

EXTRA_CFLAGS = -I$(NET_INCLUDE) 


obj-m += flowctrl.o
flowctrl-objs:= fc_main.o  fc_module.o fc_ioctl.o \
								fc_queue.o fc_hash.o   fc_timer.o\
								fc_sta.o   fc_vap.o    fc_vlan.o fc_ssid.o  fc_ap.o fc_event.o fc_white_list.o
								

ifeq ($(BOARD_TYPE),GW_X86)

EXTRA_CFLAGS += -DGW_X86

else ifeq ($(BOARD_TYPE),GW_ARM)

EXTRA_CFLAGS += -DGW_ARM

else

DEST_PATH = $(BIN_PATH)/net/module

ifeq ($(BOARD_TYPE),db12x)
ifeq ($(TEST),1)
KERNELPATH=$(TOPDIR)/build/linux-test/kernels/$(KERNEL)
DEST_PATH=$(TOPDIR)/build/image
endif
endif


include $(MODULE_MAKE)

endif


endif
