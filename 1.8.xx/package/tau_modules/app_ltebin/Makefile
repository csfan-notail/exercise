#
# Copyright (C) 2006-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=ltebin
PKG_RELEASE:=1

#PKG_BUILD_DIR := $(KERNEL_BUILD_DIR)/$(PKG_NAME)
PKG_BUILD_DIR:=$(TOPDIR)/local/tau_modules/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/ltebin
  CATEGORY:=Local Packages
  DEPENDS:=+liblte +libevent2-core +liblte2
  SUBMENU:=TAU modules
  TITLE:=Update utility for trx firmware images
endef

define Package/ltebin/description
 This package ltebin app tool.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

target=$(firstword $(subst -, ,$(BOARD)))

MAKE_FLAGS += TARGET="$(target)"
TARGET_CFLAGS := -I$(LINUX_DIR)/include $(TARGET_CFLAGS) -Dtarget_$(target)=1 -Wall


define Package/ltebin/install
	if [ ! -d $(TARGET_DIR)/sbin/ ]; then \
		(mkdir -p $(TARGET_DIR)/sbin/ ); \
	fi
	
	$(INSTALL_DIR) $(1)/usr/sbin/
	$(MAKE) -C $(PKG_BUILD_DIR) TARGET_DIR="$(TARGET_DIR)" INSTALL_BIN="$(INSTALL_BIN)" LTEBIN_SBIN="$(1)/usr/sbin" install
	
	
	$(INSTALL_DIR) $(1)/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ltecomsrv/ltecomsev $(1)/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tool/ltectl $(1)/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ltecomsrv2/ltecomsev2 $(1)/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tool2/ltectl2 $(1)/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ipkg-ar71xx/ltebin/sbin/* \
	$(TARGET_DIR)/sbin/
	
endef

$(eval $(call BuildPackage,ltebin))
