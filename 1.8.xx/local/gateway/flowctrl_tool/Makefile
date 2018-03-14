
obj-app = flowctrl
obj-dep = flowctrl_main.o flowctrl_cmd.o

ifeq ($(SYSTEM_TYPE),OPENWRT)

CFLAGS  += -I$(GW_INCLUDE) -I$(GW_EXT_NET_INCLUDE)
LDFLAGS += -Wl -static


build: $(obj-dep)
	@echo "BUILD         $(obj-app) "
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $(obj-app) $(obj-dep) $(LDLIBS)

install:
	@echo "INSTALL       $(obj-app)"

clean:
	@echo "CLEAN         $(obj-app) "
	@rm -rf *.o 
	@rm -rf $(obj-app) 
	@rm -rf ipk*
	
else

CFLAGS  = -I$(NET_INCLUDE)
LDFLAGS = -Wl

ifeq ($(BOARD_TYPE),GW_ARM)
LDFLAGS = 
endif

#xucongjiang add on 6/3/2013 start
#STATIC取值为y或者n，等于y时，静态编译；等于n时，动态编译
#STATIC定义在 $(TOPDIR)/build/config.mk 中
ifeq ($(STATIC), y)
LDFLAGS += -static
endif
#xucongjiang add on 6/3/2013 end

DEST_PATH = $(BIN_PATH)/net/bin
DEST_NAME = $(obj-app)

ifeq ($(MAKE_ENV),GW_ENV)

include $(APPMK)

else

include $(TOPDIR)/build/app.mk

endif


endif
