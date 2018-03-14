    
CC = $(CC)
CFLAGS += -Wall

CFLAGS  += -I$(TAU_INCLUDE) -I$(LTE_INCLUDE)
LDLIBS += -L$(TAU_LIBDIR) -lpal 

obj = portrate.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

portrate: portrate.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)
	
clean:
	rm -f *.o portrate