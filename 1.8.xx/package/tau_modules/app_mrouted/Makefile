#
# Copyright (C) 2006-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=mrouted
PKG_RELEASE:=1

#PKG_BUILD_DIR := $(KERNEL_BUILD_DIR)/$(PKG_NAME)
PKG_BUILD_DIR:=$(TOPDIR)/local/tau_modules/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/mrouted
  CATEGORY:=Local Packages
  SUBMENU:=TAU modules
  TITLE:=Update utility for trx firmware images
  DEPENDS:=+liblte
endef

define Package/mrouted/description
 This package mrouted app tool.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

target=$(firstword $(subst -, ,$(BOARD)))

MAKE_FLAGS += TARGET="$(target)"
TARGET_CFLAGS := -I$(LTE_INCLUDE) -I$(LINUX_DIR)/include $(TARGET_CFLAGS) -Dtarget_$(target)=1 -Wall


define Package/mrouted/install

	if [ ! -d $(TARGET_DIR)/sbin/ ]; then \
		(mkdir -p $(TARGET_DIR)/sbin/ ); \
	fi
	if [ ! -d $(TARGET_DIR)/etc/ ]; then \
		(mkdir -p $(TARGET_DIR)/etc/ ); \
	fi


	$(INSTALL_DIR) $(1)/sbin/
	$(INSTALL_DIR) $(1)/etc/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mrouted.conf $(1)/etc/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mrouted.conf $(TARGET_DIR)/etc/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mrouted $(1)/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mrouted $(TARGET_DIR)/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/map-mbone $(1)/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/map-mbone $(TARGET_DIR)/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mrinfo $(1)/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mrinfo $(TARGET_DIR)/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mtrace $(1)/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mtrace $(TARGET_DIR)/sbin/
	
endef

$(eval $(call BuildPackage,mrouted))
