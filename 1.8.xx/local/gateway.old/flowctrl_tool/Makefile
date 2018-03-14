
obj-app = flowctrl
obj-dep = flowctrl_main.o flowctrl_cmd.o

CFLAGS  += -I$(GW_INCLUDE)
LDFLAGS += -Wl -static


build: $(obj-dep)
	@echo "BUILD         $(obj-app) "
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $(obj-app) $(obj-dep) $(LDLIBS)

install:
	@echo "INSTALL       $(obj-app)"

clean:
	@echo "CLEAN         $(obj-app) "
	@rm -rf *.o 
	@rm -rf $(obj-app) 
	@rm -rf ipk*

