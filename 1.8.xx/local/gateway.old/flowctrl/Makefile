
EXTRA_CFLAGS = -I$(GW_INCLUDE) -DOPENWRT 

ifeq ($(KERNELRELEASE),)
all:
else
obj-m += flowctrl.o
flowctrl-objs:= fc_main.o  fc_module.o fc_ioctl.o \
								fc_queue.o fc_hash.o   fc_timer.o\
								fc_sta.o   fc_vap.o    fc_vlan.o fc_ssid.o  fc_ap.o fc_event.o
endif								

