	
ifeq ($(SYSTEM_TYPE),OPENWRT)
obj-share = libhttp.a
obj-dep   = $(OBJS) 
CFLAGS  += -I$(GW_INCLUDE) -I$(GW_EXT_APP_INCLUDE) 
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

else

obj-app = libhttp.a
obj-dep = cJSON.o hashmap.o linklist.o md5lib.o https.o

ifeq ($(MAKE_ENV),GW_ENV)
CFLAGS  += -I$(APP_INCLUDE) $(AP_FLAG)
else
CFLAGS  = -I$(APP_INCLUDE) $(AP_FLAG)
endif
CFLAGS += -W -Wall
CFLAGS += -DENABLE_DEBUG

LDFLAGS += -lrt
ifeq ($(BOARD_TYPE),ap93fus)
LDFLAGS = -static
endif

ifeq ($(MAKE_ENV),GW_ENV)

include $(LIBMK)
	
else
	
DEST_PATH = $(TOPDIR)/app/lib
DEST_NAME = $(obj-app)

include $(TOPDIR)/build/lib.mk

endif
	
endif	

