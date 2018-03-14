#
# Copyright (C) 2006,2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=wifidog_opt
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

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

define Package/wifidog_opt
  SUBMENU:=GateWay
  SECTION:=local
  CATEGORY:=Local Packages
  DEPENDS:=+iptables-mod-extra +iptables-mod-ipopt +kmod-ipt-nat +iptables-mod-nat-extra +libpthread +aglink +libuci +librt +pal
  TITLE:=An opt wireless captive portal solution 
  URL:=http://www.wifidog.com
endef

define Package/wifidog_opt/description
	The Wifidog project is a complete and embeddable captive
	portal solution for wireless community groups or individuals
	who wish to open a free Hotspot while still preventing abuse
	of their Internet connection.
endef

define Package/wifidog_opt/conffiles
/etc/wifidog/wifidog.conf
endef

define Package/wifidog_opt/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/wifidog $(1)/usr/sbin/wifidog
	
	$(INSTALL_BIN) ./files/free_auth.sh $(1)/usr/sbin/free_auth.sh
	
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/gctl $(1)/usr/sbin/gctl
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/api/libhttpd/libhttpd.a $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/etc/wifidog
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/src/config/wifidog.conf $(1)/etc/wifidog
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/src/config/wifidog-msg.html $(1)/etc/wifidog
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/src/config/apple-success.html $(1)/etc/wifidog
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/src/config/apple-network.html $(1)/etc/wifidog
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/src/config/maxuser.html $(1)/etc/wifidog
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/src/config/offline.html $(1)/etc/wifidog
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/src/config/style.css $(1)/etc/wifidog
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/src/config/first_page.html $(1)/etc/wifidog
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/src/config/auth_ok.html $(1)/etc/wifidog
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/src/config/get_mac.html $(1)/etc/wifidog
	
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/wifidog.init $(1)/etc/init.d/wifidog
	
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_BIN) ./files/gateway.default $(1)/etc/config/gateway
	
	$(INSTALL_DIR) $(1)/etc/uci-defaults
	$(INSTALL_BIN) ./files/gateway.default $(1)/etc/uci-defaults/
	
	$(INSTALL_DIR) $(1)/usr/lib/lua/luci/model/cbi/wifidog
	$(INSTALL_BIN) ./files/wifidogmodel.lua $(1)/usr/lib/lua/luci/model/cbi/wifidog/wifidog.lua

	$(INSTALL_BIN) ./files/gateway.lua $(1)/usr/lib/lua/luci/model/cbi/wifidog/gateway.lua

	$(INSTALL_BIN) ./files/auth.lua $(1)/usr/lib/lua/luci/model/cbi/wifidog/auth.lua
	
	$(INSTALL_BIN) ./files/snmp.lua $(1)/usr/lib/lua/luci/model/cbi/wifidog/snmp.lua

	$(INSTALL_DIR) $(1)/usr/lib/lua/luci/controller
	$(INSTALL_BIN) ./files/wifidog.lua $(1)/usr/lib/lua/luci/controller/wifidog.lua
endef


define Package/wifidog_opt/postinst
#!/bin/sh

if [ -z "$$IPKG_INSTROOT" ]; then
	( . /etc/uci-defaults/gateway.default )
	rm -f /etc/uci-defaults/gateway.default 
fi

/etc/init.d/wifidog enable
chmod +x /etc/init.d/wifidog

/etc/init.d/wifidog stop
/etc/init.d/wifidog start
exit 0
endef

define Package/wifidog_opt/prerm
#!/bin/sh

/etc/init.d/wifidog stop
/etc/init.d/wifidog disable
exit 0
endef

$(eval $(call BuildPackage,wifidog_opt))
