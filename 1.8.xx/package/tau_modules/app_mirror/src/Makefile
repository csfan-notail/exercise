    
CC = $(CC)
CFLAGS += -Wall

CFLAGS  += -I$(TAU_INCLUDE)
LDLIBS += -L$(TAU_LIBDIR) -lmirror

obj = mirror.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

mirror: mirror.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)
	
clean:
	rm -f *.o mirror