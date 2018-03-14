#
# Copyright (c) 2013 The Linux Foundation. All rights reserved.
# Copyright (C) 2008-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=mirrormodule
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(TOPDIR)/local/tau_modules/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define KernelPackage/mirror
  SECTION:=Local
  CATEGORY:=Local Packages
  SUBMENU:=TAU modules
  TITLE:=register kernel module
  FILES:=$(PKG_BUILD_DIR)/mirror.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,30,mirror)
  KCONFIG:=
endef

define KernelPackage/mirror/description
  Kernel module to mirror
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

define KernelPackage/mirror/install
	@echo "----------------$(TARGET_DIR)"
	#[ -e ./files/ar71xx_mirror ] && \
	#  $(INSTALL_DIR) $(TARGET_DIR)/etc/uci-defaults && \
	#  $(INSTALL_BIN) ./files/ar71xx_mirror $(TARGET_DIR)/etc/uci-defaults/mirror || \
	#true
	
	if [ ! -d $(TARGET_DIR)/lib/modules/3.3.8/ ]; then \
		(mkdir -p $(TARGET_DIR)/lib/modules/3.3.8/ ); \
	fi
	
	if [ ! -d $(TARGET_DIR)/etc/modules.d/ ]; then \
		(mkdir -p $(TARGET_DIR)/etc/modules.d/ ); \
	fi
	
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ipkg-ar71xx/kmod-mirror/etc/modules.d/30-mirror \
	$(TARGET_DIR)/etc/modules.d
	
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ipkg-ar71xx/kmod-mirror/lib/modules/3.3.8/mirror.ko \
	$(TARGET_DIR)/lib/modules/3.3.8
	
endef

$(eval $(call KernelPackage,mirror))
