
CC = $(CC)
CFLAGS += -Wall

CFLAGS  += -I$(TAU_INCLUDE) -I$(LTE_INCLUDE)
#LDLIBS += -L$(TAU_LIBDIR)

obj-app = gpioex
obj-dep = gpioex_ioctl_main.o gpioex_ioctl_cmd.o gpioex_ioctl_ops.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

gpioex: $(obj-dep)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)