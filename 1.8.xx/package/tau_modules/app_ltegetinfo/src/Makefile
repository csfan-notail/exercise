    
CC = $(CC)
CFLAGS += -Wall

CFLAGS  += -I$(TAU_INCLUDE) -I$(LTE_INCLUDE)
LDLIBS += -L$(TAU_LIBDIR) -llte -lpal

obj = ltegetinfo.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

ltegetinfo: ltegetinfo.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)
	
clean:
	rm -f *.o ltegetinfo