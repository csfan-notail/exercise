    
CC = $(CC)
CFLAGS += -Wall

CFLAGS += -I$(TAU_INCLUDE) -I$(LTE_INCLUDE)
LDLIBS += -L$(TAU_LIBDIR) -lm

obj-app = gdcontrl
obj-dep = gdcontrl.o 

#%.o: %.c
#	$(CC) $(CFLAGS) -c -o $@ $^

#$(obj-app): $(obj-dep)
#	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

build: $(obj-dep)
	@$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -o $(obj-app) $(obj-dep)
