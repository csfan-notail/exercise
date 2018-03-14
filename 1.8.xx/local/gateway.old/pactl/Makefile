
obj-app = pactl
obj-dep = nf_portal_ioctl_main.o nf_portal_ioctl_cmd.o nf_portal_ioctl_ops.o lib_pa.o

CFLAGS  += -I$(GW_INCLUDE)
LDFLAGS += -Wl -static


LIB_NAME = lib_pa.a
build: $(obj-dep)
	@echo "BUILD         $(obj-app) $(LIB_NAME)"
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $(obj-app) $(obj-dep) $(LDLIBS)
	@$(AR) crus $(LIB_NAME) lib_pa.o nf_portal_ioctl_ops.o

install:
	@echo "INSTALL       $(obj-app) $(LIB_NAME)"

clean:
	@echo "CLEAN         $(obj-app) $(LIB_NAME)"
	@rm -rf *.o 
	@rm -rf $(obj-app) $(LIB_NAME)
	@rm -rf ipk*

