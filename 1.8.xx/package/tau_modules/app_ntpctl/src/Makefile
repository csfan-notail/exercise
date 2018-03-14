    
CC = $(CC)
CFLAGS += -Wall

CFLAGS  += -I$(TAU_INCLUDE) -I$(LTE_INCLUDE)
LDLIBS += -L$(TAU_LIBDIR) 

obj = ntpctl.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

ntpctl: ntpctl.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)
	
clean:
	rm -f *.o ntpctl
