    
CC = $(CC)
CFLAGS += -Wall

CFLAGS  += -I$(TAU_INCLUDE) -I$(LTE_INCLUDE)
LDLIBS += -L$(TAU_LIBDIR) -llte -lpal -lm

obj = gpsctl.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

gpsctl: gpsctl.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)
	
clean:
	rm -f *.o gpsctl
