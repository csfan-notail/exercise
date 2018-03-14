
obj-share = libluas.so
obj-dep   = $(OBJS) 
CFLAGS  += -I$(TAU_INCLUDE) -I$(LTE_INCLUDE)  -fPIC 
LDFLAGS += -Wl -shared -L$(TAU_LIBDIR) -llte -lpal


SRCDIR    = ${shell pwd}
SRCS  	  = ${wildcard ${SRCDIR}/*.c}
OBJS      = ${patsubst ${SRCDIR}/%.c, ${SRCDIR}/%.o, ${SRCS}}


build: $(obj-dep)
	@echo "BUILD         $(obj-app) "
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $(obj-share) $(obj-dep) $(LDLIBS)

clean:
	@echo "CLEAN         $(obj-share) "
	@rm -rf *.o 
	@rm -rf $(obj-share) 
	@rm -rf ipk*	
