include $(TOPDIR)/rules.mk

PKG_NAME:=bash_vrrp
PKG_VERSION:=2016-08-02
PKG_RELEASE:=

#PKG_SOURCE_LOCAL:=$(TOPDIR)/local/$(PKG_NAME)

#PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)$(if $(PKG_VERSION),-$(PKG_VERSION))
#PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)$(if $(PKG_VERSION),-$(PKG_VERSION))
PKG_BUILD_DIR:=$(TOPDIR)/local/tau_modules/$(PKG_NAME)

#include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

define Package/bash_vrrp
  SECTION:=TAU modules
  CATEGORY:=Local Packages
  SUBMENU:=App
  TITLE:=Package for VRRP
endef

define Package/bash_vrrp/description
 This package is daemon used to bash_vrrp services and resources.
endef


define Build/Prepare
	#rm -fr $(PKG_BUILD_DIR);
	#cp -fr src $(PKG_BUILD_DIR);
endef

define Build/Compile
endef

#define Build/Clean
#endef


define Package/bash_vrrp/install

	$(INSTALL_DIR) $(1)/sbin/
	$(INSTALL_BIN) files/to_master.sh $(1)/sbin/
	$(INSTALL_BIN) files/to_backup.sh $(1)/sbin/
	$(INSTALL_BIN) files/to_fault.sh $(1)/sbin/

	if [ ! -d $(TARGET_DIR)/sbin/ ]; then \
		(mkdir -p $(TARGET_DIR)/sbin/ ); \
	fi
	
#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ipkg-ar71xx/bash_vrrp/sbin/to_master $(TARGET_DIR)/sbin/to_master.sh
#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ipkg-ar71xx/bash_vrrp/sbin/to_backup $(TARGET_DIR)/sbin/to_backup.sh
#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ipkg-ar71xx/bash_vrrp/sbin/to_fault $(TARGET_DIR)/sbin/to_fault.sh
	
endef

$(eval $(call BuildPackage,bash_vrrp))
