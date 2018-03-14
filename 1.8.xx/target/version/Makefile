#
# liyugao: install final versions
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/image.mk

VERSION_DIR=$(BIN_DIR)/version
VERSION_TARGET_COMPILE=
VERSION_TARGET_INSTALL=

include version-rootfs.mk

#ifneq ($(UBOOT_VER),)
include version-uboot.mk
#endif

#ifneq ($(KERNEL_VER),)
include version-kernel.mk
#endif

$(VERSION_DIR):
	mkdir -p $@

define VAR_DUMP
	$(foreach var,$(1),echo $(var)=$(call $(var));)
endef

compile: $(VERSION_TARGET_COMPILE)
	
install: $(VERSION_TARGET_INSTALL)





