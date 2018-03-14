#
# Copyright (c) 2013 The Linux Foundation. All rights reserved.
# Copyright (C) 2008-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=kmod_gpioex
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(TOPDIR)/local/tau_modules/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define KernelPackage/gpioex
  SECTION:=Local
  CATEGORY:=Local Packages
  SUBMENU:=TAU modules
  TITLE:=gpioex kernel module
  FILES:=$(PKG_BUILD_DIR)/gpioex.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,30,gpioex)
  KCONFIG:=
endef

define KernelPackage/gpioex/description
  Kernel module to gpioex
endef

MAKE_OPTS:= \
	ARCH="$(LINUX_KARCH)" \
	CROSS_COMPILE="$(TARGET_CROSS)" \
	SUBDIRS="$(PKG_BUILD_DIR)"

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" \
		$(MAKE_OPTS) \
		modules
endef

define KernelPackage/gpioex/install
	
	if [ ! -d $(TARGET_DIR)/lib/modules/3.3.8/ ]; then \
		(mkdir -p $(TARGET_DIR)/lib/modules/3.3.8/ ); \
	fi
	
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ipkg-ar71xx/kmod-gpioex/lib/modules/3.3.8/gpioex.ko \
	$(TARGET_DIR)/lib/modules/3.3.8
	
endef

$(eval $(call KernelPackage,gpioex))
