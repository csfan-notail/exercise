 
CC = $(CC)
CFLAGS += -Wall

CFLAGS  += -I$(TAU_INCLUDE)
LDLIBS += -L$(TAU_LIBDIR) -lreg

obj = regcmd.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

regcmd: regcmd.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)
	
clean:
	rm -f *.o regcmd