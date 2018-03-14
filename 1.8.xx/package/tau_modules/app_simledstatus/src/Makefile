    
CC = $(CC)
CFLAGS += -Wall

CFLAGS  += -I$(TAU_INCLUDE) -I$(LTE_INCLUDE)
LDLIBS += -L$(TAU_LIBDIR) -llte -lpal

obj = simledstatus.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

simledstatus: simledstatus.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)
	
clean:
	rm -f *.o ltegetinfo