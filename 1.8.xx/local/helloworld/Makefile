
ifeq ($(KERNELRELEASE),) 
  all: helloworld hellokernel
  
  helloworld: helloworld.h

  hellokernel: helloworld.h
  
  clean:
	-rm -fr *.mod.c .*.o.cmd .*.ko.cmd *.o *.ko hellokernel helloworld;
	-rm -fr .tmp_versions modules.order Module.symvers;
else        
  obj-m := hellokmod.o  
endif

