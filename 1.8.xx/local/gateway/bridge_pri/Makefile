EXTRA_CFLAGS = -I$(GW_INCLUDE) -I$(GW_EXT_NET_INCLUDE) -DOPENWRT 
#
# Makefile for the IEEE 802.1d ethernet bridging layer.
#
ifeq ($(KERNELRELEASE),)
all:							
else

obj-m += bridge.o

bridge-y	:= br.o br_device.o br_fdb.o br_forward.o br_if.o br_input.o \
			br_ioctl.o br_notify.o br_stp.o br_stp_bpdu.o \
			br_stp_if.o br_stp_timer.o br_netlink.o

bridge-y += br_sysfs_if.o br_sysfs_br.o

bridge-y += br_netfilter.o

bridge-y += br_multicast.o

obj-m += netfilter/

endif
clean:
	@rm -rf *.ko *.o *.order *.symvers *.mod.*
	@rm -rf netfilter/*.o netfilter/*.ko netfilter/*.order netfilter/*.mod.*
