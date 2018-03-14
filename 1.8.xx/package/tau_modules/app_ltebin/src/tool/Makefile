
CC = $(CC)
CFLAGS += -Wall -fPIC -I$(LTE_INCLUDE)

MYLDLIBS = -L$(TAU_LIBDIR) -lusb-1.0 -llte

obj = lte_ctl.o

build:ltectl

ltectl: $(obj)
	$(CC) $(LDFLAGS) $(MYLDLIBS) -o $@ $^ $(LDLIBS)

$(obj): lte_ctl.c
	$(CC) $(CFLAGS) -c -o $@ $^

install:
	@echo "INSTALL       $(obj-app)"
	#$(INSTALL_BIN) ltectl $(LTEBIN_SBIN)/
	#$(INSTALL_BIN) ltectl $(TARGET_DIR)/sbin/
