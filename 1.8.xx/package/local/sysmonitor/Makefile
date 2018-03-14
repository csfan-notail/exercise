include $(TOPDIR)/rules.mk

PKG_NAME:=sysmonitor
PKG_VERSION:=2015-08-24
PKG_RELEASE:=

#PKG_SOURCE_LOCAL:=$(TOPDIR)/local/$(PKG_NAME)

#PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)$(if $(PKG_VERSION),-$(PKG_VERSION))
#PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)$(if $(PKG_VERSION),-$(PKG_VERSION))
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

#include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

define Package/sysmonitor
  SECTION:=local
  CATEGORY:=Local Packages
  DEPENDS:=busybox
  SUBMENU:=App
  TITLE:=Package for sysmonitor
endef

define Package/sysmonitor/description
 This package is daemon used to monitor services and resources.
endef


define Build/Prepare
	rm -fr $(PKG_BUILD_DIR);
	cp -fr src $(PKG_BUILD_DIR);
endef

#define Build/Compile
#endef

#define Build/Clean
#endef


define Package/sysmonitor/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/sysmonitor $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) files/sysmonitor $(1)/etc/init.d/
endef

$(eval $(call BuildPackage,sysmonitor))
