    
CC = $(CC)
CFLAGS += -Wall

CFLAGS  += -I$(TAU_INCLUDE) -I$(LTE_INCLUDE)
LDLIBS += -L$(TAU_LIBDIR) -llte -lpal

obj = ledstatus.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

ledstatus: ledstatus.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)
	
clean:
	rm -f *.o ltegetinfo