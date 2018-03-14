    
CC = $(CC)
CFLAGS += -Wall

CFLAGS  += -I$(TAU_INCLUDE) -I$(LTE_INCLUDE)
LDLIBS += -L$(TAU_LIBDIR) -lreg -lomnet 

obj = trackint.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

trackint: trackint.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)
	
clean:
	rm -f *.o trackint