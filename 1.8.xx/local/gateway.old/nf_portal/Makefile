
EXTRA_CFLAGS = -I$(GW_INCLUDE) 
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

