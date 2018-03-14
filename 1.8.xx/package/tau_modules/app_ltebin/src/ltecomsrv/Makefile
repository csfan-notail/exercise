obj-app = ltecomsev
obj-dep = $(OBJS) 

CFLAGS  =  -fPIC -I$(MYSRC)/include -I$(LTE_INCLUDE)
LDLIBS +=  -L$(TAU_LIBDIR) -lusb-1.0 -levent_core 
LDFLAGS += -Wall

#CFLAGS +=-D__LONGCHEER__ -D__LEADCORE__ -D__THINKWILL__ -D__ALTAIR__ -D__ZTE__ -D__ZTE_CDMA2000__ -D__ARCCRA__ -D__THINKWILL_MI660__ -D__YUGA_CEM600__ -D__ZTE_MICRO__ -D__YUGA__ -D__HUAWEI__ -D__MARVELL__ -D__HUAWEI4G__
CFLAGS += -D__HUAWEI4G__
MYSRC    = ${shell pwd}

BIN_SRC=$(shell echo $(MYSRC)/core/*.c)
#BIN_SRC+=$(shell find $(MYSRC)/evhandle -name *.c)
#BIN_SRC+=$(shell find $(MYSRC)/util -name *.c)
BIN_SRC+=$(shell echo $(MYSRC)/devops/pal/*.c)
#BIN_SRC+=$(shell echo $(MYSRC)/devops/longcheer/*.c)
#BIN_SRC+=$(shell echo $(MYSRC)/devops/leadcore/*.c)
#BIN_SRC+=$(shell find $(MYSRC)/devops/thinkwill -name *.c)
#BIN_SRC+=$(shell find $(MYSRC)/devops/altair -name *.c)
#BIN_SRC+=$(shell find $(MYSRC)/devops/zte -name *.c)
#BIN_SRC+=$(shell find $(MYSRC)/devops/zte-cdma2000 -name *.c)
#BIN_SRC+=$(shell echo $(MYSRC)/devops/arccra/*.c)	
#BIN_SRC+=$(shell find $(MYSRC)/devops/thinkwill-cdma-mi660 -name *.c)	
#BIN_SRC+=$(shell find $(MYSRC)/devops/yuga-cdma-cem600 -name *.c)
#BIN_SRC+=$(shell echo $(MYSRC)/devops/yuga/*.c)
#BIN_SRC+=$(shell echo $(MYSRC)/devops/zte-micro/*.c)
#BIN_SRC+=$(shell echo $(MYSRC)/devops/huawei/*.c)
#BIN_SRC+=$(shell echo $(MYSRC)/devops/marvell/*.c)
BIN_SRC+=$(shell echo $(MYSRC)/devops/huawei4G/*.c)

OBJS=$(patsubst %.c,%.o, $(BIN_SRC))

build: $(obj-app)

$(obj-app):$(obj-dep)
	@echo "BUILD         $(obj-app)  "
	$(CC) $(LDFLAGS) $(LDLIBS) -o $@ $^
	#@$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $(MYLDLIBS) -o $(obj-app) $(obj-dep) $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

install:
	@echo "INSTALL       $(obj-app)"
	#$(INSTALL_BIN) $(obj-app) $(LTEBIN_SBIN)/
	#$(INSTALL_BIN) $(obj-app) $(TARGET_DIR)/sbin/
