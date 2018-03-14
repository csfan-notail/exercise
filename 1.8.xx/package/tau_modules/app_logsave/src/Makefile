    
CC = $(CC)
CFLAGS += -Wall

CFLAGS += -I$(TAU_INCLUDE) -I$(LTE_INCLUDE)
LDLIBS += -L$(TAU_LIBDIR) -lpcap

obj-app = logsave
obj-dep = logsave.o 

#%.o: %.c
#	$(CC) $(CFLAGS) -c -o $@ $^

#$(obj-app): $(obj-dep)
#	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

build: $(obj-dep)
	@$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -o $(obj-app) $(obj-dep)
