    
CC = $(CC)
CFLAGS += -Wall

CFLAGS  += -I$(TAU_INCLUDE) -I$(LTE_INCLUDE)
LDLIBS += -L$(TAU_LIBDIR) -lpal -lreg

obj = portcount.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

portcount: portcount.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)
	
clean:
	rm -f *.o portcount