    
CC = $(CC)
CFLAGS += -Wall

CFLAGS  += -I$(TAU_INCLUDE) -I$(LTE_INCLUDE)
LDLIBS += -L$(TAU_LIBDIR) -llte

obj = lteupdate.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

lteupdate: lteupdate.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)
	
clean:
	rm -f *.o lteupdate
