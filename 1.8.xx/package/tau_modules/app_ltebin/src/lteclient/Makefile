
CC = $(CC)
CFLAGS += -Wall -fPIC -I$(LTE_INCLUDE) -I../ltecomsrv/include

MYLDLIBS = -L$(TAU_LIBDIR) -lusb-1.0 -llte -levent_core 

target = lteclient
object = lteclient.o

build:$(target)

$(target): $(object)
	$(CC) $(LDFLAGS) $(MYLDLIBS) -o $@ $^ $(LDLIBS)

$(object): lteclient.c
	$(CC) $(CFLAGS) -c -o $@ $^

install:
	@echo "INSTALL       $(obj-app)"
	#$(INSTALL_BIN) $(target) $(LTEBIN_SBIN)/
	#$(INSTALL_BIN) $(target) $(TARGET_DIR)/sbin/
