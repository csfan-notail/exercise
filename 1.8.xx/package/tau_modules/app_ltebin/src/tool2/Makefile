
CC = $(CC)
CFLAGS += -Wall -fPIC -I$(LTE_INCLUDE2)

MYLDLIBS = -L$(TAU_LIBDIR) -lusb-1.0 -llte2

obj = lte_ctl2.o

build:ltectl2

ltectl2: $(obj)
	$(CC) $(LDFLAGS) $(MYLDLIBS) -o $@ $^ $(LDLIBS)

$(obj): lte_ctl2.c
	$(CC) $(CFLAGS) -c -o $@ $^

install:
	@echo "INSTALL       $(obj-app)"
	#$(INSTALL_BIN) ltectl2 $(LTEBIN_SBIN)/
	#$(INSTALL_BIN) ltectl2 $(TARGET_DIR)/sbin/
