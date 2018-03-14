	

obj-share = libhttp.a
obj-dep   = $(OBJS) 
CFLAGS  += -I$(GW_INCLUDE) 
LDFLAGS += -Wl -shared -static -fPIC 

SRCDIR    = ${shell pwd}
SRCS  	  = ${wildcard ${SRCDIR}/*.c}
OBJS      = ${patsubst ${SRCDIR}/%.c, ${SRCDIR}/%.o, ${SRCS}}


build: $(obj-dep)
	@echo "BUILD         $(obj-app) "
	@$(AR) crus  $(obj-share) $(obj-dep)  

install:
	@echo "INSTALL       $(obj-share) "

clean:
	@echo "CLEAN         $(obj-share) "
	@rm -rf *.o 
	@rm -rf $(obj-share) 
	@rm -rf ipk*

	

