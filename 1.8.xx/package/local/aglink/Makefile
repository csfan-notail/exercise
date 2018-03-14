#
# Copyright (C) 2006,2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=aglink
PKG_VERSION:=20151106
PKG_RELEASE:=1

#PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
#PKG_SOURCE_URL:= @SF/$(PKG_NAME)
#PKG_MD5SUM:=e3ecacba67a91b6ea3c1072ba6c5a0b4

#PKG_FIXUP:=autoreconf
#PKG_INSTALL:=1

PKG_SOURCE_LOCAL:=$(TOPDIR)/local/gateway/$(PKG_NAME)
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

export GW_INCLUDE = $(TOPDIR)/local/gateway/include
export SYS_TYPE=OPENWRT
include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

define Package/aglink
  SUBMENU:=GateWay
  SECTION:=local
  CATEGORY:=Local Packages
  DEPENDS:=libevent2 pal librt libpthread libiw wireless-tools
  TITLE:=A private proto of gbcom for message between gw and ap
  URL:=http://www.gbcom.com.cn
endef

define Package/aglink/description
	The Wifidog project is a complete and embeddable captive
	portal solution for wireless community groups or individuals
	who wish to open a free Hotspot while still preventing abuse
	of their Internet connection.
endef

define Package/aglink/conffiles
/etc/aglink/ag_ipc.conf
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib/
	$(CP) $(PKG_BUILD_DIR)/*.so $(1)/usr/lib/
endef


define Package/aglink/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/aglink $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/aglinkcli $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/*.so* $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/etc/aglink
	$(INSTALL_DATA) ./files/ag_ipc.conf $(1)/etc/aglink

	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/aglink.init $(1)/etc/init.d/aglink
endef


define Package/aglink/postinst
#!/bin/sh
/etc/init.d/aglink enable
chmod +x /etc/init.d/aglink

/etc/init.d/aglink stop
/etc/init.d/aglink start
exit 0
endef

define Package/aglink/prerm
#!/bin/sh

/etc/init.d/aglink stop
/etc/init.d/aglink disable
exit 0
endef


$(eval $(call BuildPackage,aglink))
