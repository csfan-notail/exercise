
CC = $(CC)
CFLAGS += -Wall -fPIC -I$(LTE_INCLUDE)

MYLDLIBS = -L$(TAU_LIBDIR) -lusb-1.0 -llte

obj = lte_startup.o

build:lte_startup

lte_startup: lte_startup.o
	$(CC) $(LDFLAGS) $(MYLDLIBS) -o $@ $^ $(LDLIBS)

lte_startup.o: lte_startup.c
	$(CC) $(CFLAGS) -c -o $@ $^

install:
	@echo "INSTALL       $(obj-app)"
	#$(INSTALL_BIN) lte_startup $(LTEBIN_SBIN)
	#$(INSTALL_BIN) lte_startup $(TARGET_DIR)/sbin/

