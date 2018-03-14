


include $(TOPDIR)/rules.mk

PKG_NAME:=miscutils
PKG_VERSION:=2015-05-11
PKG_RELEASE:=

#PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)_$(ARCH).tar.bz2
#PKG_SOURCE_URL:=http://downloads.openwrt.org/sources
PKG_SOURCE_LOCAL:=$(TOPDIR)/local/$(PKG_NAME)

#PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)$(if $(PKG_VERSION),-$(PKG_VERSION))
#PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)$(if $(PKG_VERSION),-$(PKG_VERSION))
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

define Package/miscutils/Default
  SECTION:=local
  CATEGORY:=Local Packages
  DEPENDS:=
  SUBMENU:=
  SUBMENUDEP:=
endef

define Package/miscutils
  $(call Package/miscutils/Default)
  TITLE:=Miscellaneous Utils
endef

define Package/miscutils/description
 This package contains miscellaneous utils and scripts.
endef

#define Build/Configure 
#endef 

define Build/Compile
  $(call Build/Compile/Default)
endef

#define Build/InstallDev
#endef

## Build/Install
## Build/Dist
## Build/DistCheck

define Package/miscutils/install
	$(INSTALL_DIR) $(1)/usr/sbin
	for f in `ls -d $(PKG_BUILD_DIR)/scripts/*;ls -d $(PKG_BUILD_DIR)/install_bin/*;`;do \
	  if [ -L $$$$f ];then \
	    cp -fP $$$$f $(1)/usr/sbin/; \
	  else \
	    $(INSTALL_BIN) $$$$f $(1)/usr/sbin/; \
	  fi; \
	done
endef

$(eval $(call BuildPackage,miscutils))
