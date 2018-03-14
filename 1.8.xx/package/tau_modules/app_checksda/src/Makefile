    
CC = $(CC)
CFLAGS += -Wall

CFLAGS += -I$(TAU_INCLUDE) -I$(LTE_INCLUDE)
LDLIBS += -L$(TAU_LIBDIR) -lpthread

obj-app = checksda
obj-dep = checksda.o 

#%.o: %.c
#	$(CC) $(CFLAGS) -c -o $@ $^

#$(obj-app): $(obj-dep)
#	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

build: $(obj-dep)
	@$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -D_FILE_OFFSET_BITS=64 -D_LARGE_FILE -o $(obj-app) $(obj-dep)
