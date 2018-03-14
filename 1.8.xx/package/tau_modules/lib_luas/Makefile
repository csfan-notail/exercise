#
# Copyright (C) 2006-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=libluas
PKG_RELEASE:=1

#PKG_BUILD_DIR := $(KERNEL_BUILD_DIR)/$(PKG_NAME)
PKG_BUILD_DIR:=$(TOPDIR)/local/tau_modules/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/libluas
  CATEGORY:=Local Packages
  SUBMENU:=TAU modules
  TITLE:=Update utility for trx firmware images
  DEPENDS:=+liblte
endef

define Package/libluas/description
 This package libluas module app toll regcmd.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

target=$(firstword $(subst -, ,$(BOARD)))

MAKE_FLAGS += TARGET="$(target)"
TARGET_CFLAGS := -I$(LINUX_DIR)/include $(TARGET_CFLAGS) -Dtarget_$(target)=1 -Wall


define Package/libluas/install
	if [ ! -d $(TARGET_DIR)/usr/lib/lua/ ]; then \
		(mkdir -p $(TARGET_DIR)/usr/lib/lua/ ); \
	fi

	#$(CP) $(TARGET_DIR)/lib/liblte.so $(PKG_BUILD_DIR)/liblte.so	
	$(CP) $(TARGET_DIR)/lib/libpal.so $(PKG_BUILD_DIR)/libpal.so
	$(INSTALL_DIR) $(1)/usr/lib/lua/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libluas.so $(1)/usr/lib/lua/
	$(INSTALL_BIN) $(1)/usr/lib/lua/libluas.so $(TARGET_DIR)/usr/lib/lua/

	
	$(INSTALL_DIR) $(1)/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libpal.so $(1)/lib/



endef

$(eval $(call BuildPackage,libluas))
