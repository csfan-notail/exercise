#
# Copyright (c) 2013 The Linux Foundation. All rights reserved.
# Copyright (C) 2008-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=regmodule
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(TOPDIR)/local/tau_modules/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define KernelPackage/regmodule
  SECTION:=Local
  CATEGORY:=Local Packages
  SUBMENU:=TAU modules
  TITLE:=register kernel module
  FILES:=$(PKG_BUILD_DIR)/regmodule.$(LINUX_KMOD_SUFFIX)
  AUTOLOAD:=$(call AutoLoad,29,regmodule)
  KCONFIG:=
endef

define KernelPackage/regmodule/description
  Kernel module to regmodule
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

define KernelPackage/regmodule/install
	@echo "----------------$(TARGET_DIR)"
	#[ -e ./files/ar71xx_regmodule ] && \
	#  $(INSTALL_DIR) $(TARGET_DIR)/etc/uci-defaults && \
	#  $(INSTALL_BIN) ./files/ar71xx_regmodule $(TARGET_DIR)/etc/uci-defaults/regmodule || \
	#true
	
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ipkg-ar71xx/kmod-regmodule/lib/modules/3.3.8/regmodule.ko \
	$(TARGET_DIR)/lib/modules/3.3.8
	
endef

$(eval $(call KernelPackage,regmodule))
