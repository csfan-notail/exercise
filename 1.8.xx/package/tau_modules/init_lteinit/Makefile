include $(TOPDIR)/rules.mk

PKG_NAME:=lteinit
PKG_VERSION:=2015-08-24
PKG_RELEASE:=

#PKG_SOURCE_LOCAL:=$(TOPDIR)/local/$(PKG_NAME)

#PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)$(if $(PKG_VERSION),-$(PKG_VERSION))
#PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)$(if $(PKG_VERSION),-$(PKG_VERSION))
PKG_BUILD_DIR:=$(TOPDIR)/local/tau_modules/$(PKG_NAME)

#include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

define Package/lteinit
  SECTION:=TAU modules
  CATEGORY:=Local Packages
  DEPENDS:=busybox
  SUBMENU:=App
  TITLE:=Package for lteinit
endef

define Package/lteinit/description
 This package is daemon used to lteinit services and resources.
endef


define Build/Prepare
	#rm -fr $(PKG_BUILD_DIR);
	#cp -fr src $(PKG_BUILD_DIR);
endef

define Build/Compile
endef

#define Build/Clean
#endef


define Package/lteinit/install

	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) files/lteinit $(1)/etc/init.d/
	if [ ! -d $(TARGET_DIR)/etc/init.d/ ]; then \
		(mkdir -p $(TARGET_DIR)/etc/init.d/ ); \
	fi
	
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ipkg-ar71xx/lteinit/etc/init.d/lteinit $(TARGET_DIR)/etc/init.d/lteinit
	
endef

$(eval $(call BuildPackage,lteinit))
