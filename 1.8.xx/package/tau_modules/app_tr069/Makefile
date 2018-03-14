#
# Copyright (C) 2006-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=tr069
PKG_RELEASE:=1

#PKG_BUILD_DIR := $(KERNEL_BUILD_DIR)/$(PKG_NAME)
PKG_BUILD_DIR:=$(TOPDIR)/local/tau_modules/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/tr069
  CATEGORY:=Local Packages
  #DEPENDS:=+liblte +libevent2-core +libomnet
  SUBMENU:=TAU modules
  TITLE:=Update utility for trx firmware images
endef

define Package/tr069/description
 This package tr069 app tool.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

target=$(firstword $(subst -, ,$(BOARD)))

MAKE_FLAGS += TARGET="$(target)"
TARGET_CFLAGS := -I$(LINUX_DIR)/include $(TARGET_CFLAGS) -Dtarget_$(target)=1 -Wall


define Package/tr069/install
	if [ ! -d $(TARGET_DIR)/sbin/ ]; then \
		(mkdir -p $(TARGET_DIR)/sbin/ ); \
	fi
	$(INSTALL_DIR) $(1)/lib
	
	$(CP) $(STAGING_DIR)/root-ar71xx/usr/lib/libssl.so.1.0.0 $(PKG_BUILD_DIR)/ipkg-ar71xx/tr069/lib/
	$(CP) $(STAGING_DIR)/root-ar71xx/usr/lib/libcrypto.so.1.0.0 $(PKG_BUILD_DIR)/ipkg-ar71xx/tr069/lib/
	$(CP) $(STAGING_DIR)/root-ar71xx/usr/lib/libftp.so.3 $(PKG_BUILD_DIR)/ipkg-ar71xx/tr069/lib/
	$(CP) $(STAGING_DIR)/root-ar71xx/usr/lib/libusb-1.0.so.0 $(PKG_BUILD_DIR)/ipkg-ar71xx/tr069/lib/
	$(CP) $(TARGET_DIR)/lib/libpal.so $(PKG_BUILD_DIR)/ipkg-ar71xx/tr069/lib/
	$(CP) $(STAGING_DIR)/root-ar71xx/lib/libomnet.so $(PKG_BUILD_DIR)/ipkg-ar71xx/tr069/lib/
	$(CP) $(STAGING_DIR)/root-ar71xx/lib/libreg.so $(PKG_BUILD_DIR)/ipkg-ar71xx/tr069/lib/
	$(INSTALL_DIR) $(1)/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/.build/tr069 $(1)/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ipkg-ar71xx/tr069/sbin/* \
	$(TARGET_DIR)/sbin/
	
endef

$(eval $(call BuildPackage,tr069))
