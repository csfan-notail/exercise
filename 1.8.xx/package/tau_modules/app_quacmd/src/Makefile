
CC = $(CC)
CFLAGS += -Wall

CFLAGS  += -I$(TAU_INCLUDE) -I$(LTE_INCLUDE) 
LDLIBS += -L$(TAU_LIBDIR)  -L$(TAU_LIBDIR)/../usr/lib -lncurses -lreadline -lzebra -lospf

obj-app = quacmd
obj-dep = quacmd_cmd.o quacmd_main.o quacmd.o quacmd_user.o quacmd_config.o 

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

quacmd: $(obj-dep)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS) 