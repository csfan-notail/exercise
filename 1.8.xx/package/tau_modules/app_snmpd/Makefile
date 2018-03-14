#
# Copyright (C) 2006-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=mini_snmpd
PKG_RELEASE:=1

#PKG_BUILD_DIR := $(KERNEL_BUILD_DIR)/$(PKG_NAME)
PKG_BUILD_DIR:=$(TOPDIR)/local/tau_modules/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/mini_snmpd
  CATEGORY:=Local Packages
  SUBMENU:=TAU modules
  TITLE:=Update utility for trx firmware images
  DEPENDS:=+liblte +libftp +libpal +libpthread +libevent +libomnet
endef

define Package/mini_snmpd/description
 This package register module app toll mini_snmpd.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

target=$(firstword $(subst -, ,$(BOARD)))

MAKE_FLAGS += TARGET="$(target)"
TARGET_CFLAGS := -I$(LINUX_DIR)/include $(TARGET_CFLAGS) -Dtarget_$(target)=1 -Wall

define Package/mini_snmpd/install

	if [ ! -d $(TARGET_DIR)/sbin/ ]; then \
		(mkdir -p $(TARGET_DIR)/sbin/ ); \
	fi
	
	$(INSTALL_DIR) $(1)/sbin/
	$(INSTALL_DIR) $(1)/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mini_snmpd $(1)/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mini_snmpd $(TARGET_DIR)/sbin/
	
	$(CP) $(TARGET_DIR)/lib/liblte.so $(PKG_BUILD_DIR)/ipkg-ar71xx/mini_snmpd/lib/
	$(CP) $(TARGET_DIR)/lib/libpal.so $(PKG_BUILD_DIR)/ipkg-ar71xx/mini_snmpd/lib/
	$(CP) $(STAGING_DIR)/root-ar71xx/lib/libpthread.so.0 $(PKG_BUILD_DIR)/ipkg-ar71xx/mini_snmpd/lib/
	$(CP) $(STAGING_DIR)/root-ar71xx/usr/lib/libevent-2.0.so.5 $(PKG_BUILD_DIR)/ipkg-ar71xx/mini_snmpd/lib/
	$(CP) $(STAGING_DIR)/root-ar71xx/usr/lib/libftp.so.3 $(PKG_BUILD_DIR)/ipkg-ar71xx/mini_snmpd/lib/
	
endef

$(eval $(call BuildPackage,mini_snmpd))
