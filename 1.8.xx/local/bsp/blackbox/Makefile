
ifeq ($(KERNELRELEASE),) 
  all: reboot showreboot rebootrecordtimer

  clean:
	-rm reboot showreboot rebootrecordtimer
	-rm -fr *.mod.c .*.o.cmd .*.ko.cmd *.o *.ko ;
	-rm -fr .tmp_versions modules.order Module.symvers;
else        
  obj-m:=bb.o
  bb-objs:=  blackbox.o
endif

