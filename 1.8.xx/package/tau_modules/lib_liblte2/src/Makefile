obj-share = liblte2.so
obj-dep   = $(OBJS) 

#CFLAGS  += -I./include -fPIC -D__LONGCHEER__ -D__LEADCORE__ -D__THINKWILL__ -D__ZTE__ -D__ZTE_CDMA2000__ -D__ARCCRA__ -D__THINKWILL_MI660__ -D__YUGA_CEM600__ -D__ZTE_MICRO__ -D__HUAWEI__ -D__YUGA__	-D__MARVELL__ -D__HUAWEI4G__
CFLAGS  += -I./include -fPIC  -D__HUAWEI4G__
LDFLAGS += -Wl -shared -L $(TAU_LIBDIR) -lusb-1.0

SRCDIR    = ${shell pwd}
libsrc = $(shell echo ${SRCDIR}/core/*.c)
#libsrc+=$(shell echo ${SRCDIR}/longcheer/*.c)
#libsrc+=$(shell echo ${SRCDIR}/leadcore/*.c)
#libsrc+=$(shell echo ${SRCDIR}/thinkwill/*.c)
#libsrc+=$(shell echo ./altair/*.c)
#libsrc+=$(shell echo ${SRCDIR}/zte/*.c)
#libsrc+=$(shell echo ${SRCDIR}/zte-cdma2000/*.c)
#libsrc+=$(shell echo ${SRCDIR}/arccra/*.c)	
#libsrc+=$(shell echo ${SRCDIR}/thinkwill-cdma-mi660/*.c)	
#libsrc+=$(shell echo ${SRCDIR}/yuga-cdma-cem600/*.c)
#libsrc+=$(shell echo ${SRCDIR}/zte-micro/*.c)
#libsrc+=$(shell echo ${SRCDIR}/huawei/*.c)
#libsrc+=$(shell echo ${SRCDIR}/yuga/*.c)
#libsrc+=$(shell echo ${SRCDIR}/marvell/*.c)
libsrc+=$(shell echo ${SRCDIR}/huawei4G/*.c)
#SRCS  	  = ${wildcard ${SRCDIR}/core/*.c}
#SRCS  	  += ${wildcard ${SRCDIR}/longcheer/*.c}
#SRCS  	  += ${wildcard ${SRCDIR}/leadcore/*.c}
#SRCS  	  += ${wildcard ${SRCDIR}/thinkwill/*.c}
#libsrc+=$(shell echo ./altair/*.c)
#SRCS  	  += ${wildcard ${SRCDIR}/zte/*.c}
#SRCS  	  += ${wildcard ${SRCDIR}/zte-cdma2000/*.c}
###SRCS  	  += ${wildcard ${SRCDIR}/arccra/*.c}
##SRCS  	  += ${wildcard ${SRCDIR}/thinkwill-cdma-mi660/*.c}
#SRCS  	  += ${wildcard ${SRCDIR}/yuga-cdma-cem600/*.c}	
#SRCS  	  += ${wildcard ${SRCDIR}/zte-micro/*.c}
#SRCS  	  += ${wildcard ${SRCDIR}/huawei/*.c}
#SRCS  	  += ${wildcard ${SRCDIR}/yuga/*.c}
#SRCS  	  += ${wildcard ${SRCDIR}/marvell/*.c}
#SRCS  	  += ${wildcard ${SRCDIR}/huawei4G/*.c}
#SRCS  	  = ${wildcard *.c}
OBJS      = ${patsubst %.c, %.o, ${libsrc}}


build: $(obj-dep)
	@echo "BUILD         $(obj-app) "
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $(obj-share) $(obj-dep) $(LDLIBS)
