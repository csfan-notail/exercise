
CC = $(CC)
CFLAGS += -Wall -fPIC -I$(LTE_INCLUDE)

MYLDLIBS = -L$(TAU_LIBDIR) -lusb-1.0 -llte

obj = checkalive.o

build:checkalive

checkalive: $(obj)
	$(CC) $(LDFLAGS) $(MYLDLIBS) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

install:
	@echo "INSTALL       $(obj-app)"
	#$(INSTALL_BIN) checkalive $(LTEBIN_SBIN)
	#$(INSTALL_BIN) checkalive $(TARGET_DIR)/sbin/