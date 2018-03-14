    
CC = $(CC)
CFLAGS += -Wall

CFLAGS  += -I$(TAU_INCLUDE) -I$(LTE_INCLUDE)
LDLIBS += -L$(TAU_LIBDIR) -llte2 -lpal

obj = lteauto2.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

lteauto2: lteauto2.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)
	
clean:
	rm -f *.o lteauto2