#
# Copyright (C) 2006-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=quacmd
PKG_RELEASE:=1

#PKG_BUILD_DIR := $(KERNEL_BUILD_DIR)/$(PKG_NAME)
PKG_BUILD_DIR:=$(TOPDIR)/local/tau_modules/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/quacmd
  CATEGORY:=Local Packages
  SUBMENU:=TAU modules
  TITLE:=Update utility for trx firmware images
  DEPENDS:=+libncurses +libreadline +libpthread +librt +libzebra +libospf
endef

define Package/quacmd/description
 This package quacmd app tool.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

target=$(firstword $(subst -, ,$(BOARD)))

MAKE_FLAGS += TARGET="$(target)"
TARGET_CFLAGS := -I$(LTE_INCLUDE) -I$(LINUX_DIR)/include $(TARGET_CFLAGS) -Dtarget_$(target)=1 -Wall


define Package/quacmd/install
	
	if [ ! -d $(TARGET_DIR)/sbin/ ]; then \
		(mkdir -p $(TARGET_DIR)/sbin/ ); \
	fi
	
	$(INSTALL_DIR) $(1)/sbin/
	$(INSTALL_DIR) $(1)/lib/

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/quacmd $(1)/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/quacmd $(TARGET_DIR)/sbin/
	$(INSTALL_BIN) $(STAGING_DIR)/root-ar71xx/usr/lib/libzebra.so.0.0.0  $(1)/lib/
	$(INSTALL_BIN) $(STAGING_DIR)/root-ar71xx/usr/lib/libospf.so.0.0.0  $(1)/lib/
	$(INSTALL_BIN) $(STAGING_DIR)/root-ar71xx/usr/lib/libncurses.so.5.7  $(1)/lib/
	$(INSTALL_BIN) $(STAGING_DIR)/root-ar71xx/usr/lib/libreadline.so.5.2  $(1)/lib/
	$(INSTALL_BIN) $(STAGING_DIR)/root-ar71xx/lib/libpthread-0.9.33.2.so  $(1)/lib/
	$(INSTALL_BIN) $(STAGING_DIR)/root-ar71xx/lib/librt-0.9.33.2.so  $(1)/lib/

endef

$(eval $(call BuildPackage,quacmd))
