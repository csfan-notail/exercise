#
# Copyright (C) 2006-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=liblte2
PKG_RELEASE:=1

#PKG_BUILD_DIR := $(KERNEL_BUILD_DIR)/$(PKG_NAME)
PKG_BUILD_DIR:=$(TOPDIR)/local/tau_modules/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/liblte2
  CATEGORY:=Local Packages
  DEPENDS:=+libusb-1.0
  SUBMENU:=TAU modules
  TITLE:=Update utility for trx firmware images
endef

define Package/liblte2/description
 This package liblte2 module app toll regcmd.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

target=$(firstword $(subst -, ,$(BOARD)))

MAKE_FLAGS += TARGET="$(target)"
TARGET_CFLAGS := -I$(LINUX_DIR)/include $(TARGET_CFLAGS) -Dtarget_$(target)=1 -Wall


define Package/liblte2/install
	if [ ! -d $(TARGET_DIR)/lib/ ]; then \
		(mkdir -p $(TARGET_DIR)/lib/ ); \
	fi
	
	$(INSTALL_DIR) $(1)/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/liblte2.so $(1)/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/liblte2.so \
	$(TARGET_DIR)/lib/
	#cp $(PKG_BUILD_DIR)/include/lte.h 
endef

$(eval $(call BuildPackage,liblte2))
