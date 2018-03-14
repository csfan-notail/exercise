    
CC = $(CC)
CFLAGS += -Wall

CFLAGS  += -I$(TAU_INCLUDE) -I$(LTE_INCLUDE)
LDLIBS += -L$(TAU_LIBDIR)  -lpal -lutil

obj = tr69mon.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

tr69mon: tr69mon.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)
	
clean:
	rm -f *.o tr69mon