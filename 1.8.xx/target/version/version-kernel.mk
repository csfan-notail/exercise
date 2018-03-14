#
# liyugao: rules to pack final kernel versions
#

#include $(TOPDIR)/rules.mk
#include $(INCLUDE_DIR)/image.mk 
include $(INCLUDE_DIR)/kernel.mk

KERNEL_VER?=1.4
VERSION_TARGET:=ta321

KERNEL_VER_STRING_FILE=$(LINUX_DIR)/.version

$(KERNEL_VER_STRING_FILE) : FORCE
	echo $(KERNEL_VER) > $@;

define KernelVersionDefault
#modified by xiaowei 2016-2-25 
#KERNEL_VERSION_$(1)=$(VERSION_DIR)/linux$(LINUX_VERSION)-openwrt-$(1)-$(KERNEL_VER).bin
KERNEL_VERSION_$(1)=$(VERSION_DIR)/linux$(LINUX_VERSION)-$(1)-$(KERNEL_VER).bin
KERNEL_VERSION_SRC_$(1)=$(BIN_DIR)/openwrt-ar71xx-generic-common-16M-kernel.bin

endef

define BuildKernelVersion
$(KERNEL_VERSION_$(1)): $(KERNEL_VERSION_SRC_$(1))
	@echo Generating `basename $$@`...;
	@cp $(KERNEL_VERSION_SRC_$(1)) $(KERNEL_VERSION_$(1))

endef


$(eval $(foreach board,$(VERSION_TARGET),$(call KernelVersionDefault,$(board))))

# override default variables here
# do nothing for now
#

$(eval $(foreach board,$(VERSION_TARGET),$(call BuildKernelVersion,$(board))))

VERSION_TARGET_COMPILE+=$(KERNEL_VER_STRING_FILE)
VERSION_TARGET_INSTALL+=$(foreach board,$(VERSION_TARGET),$(KERNEL_VERSION_$(board)))
