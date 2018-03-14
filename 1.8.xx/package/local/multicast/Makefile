
include $(TOPDIR)/rules.mk

PKG_NAME:=multicast
PKG_VERSION:=2015-05-26
PKG_RELEASE:=

PKG_SOURCE_LOCAL:=$(TOPDIR)/local/mtest/$(PKG_NAME)

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

define Package/multicast/Default
  SECTION:=local
  CATEGORY:=Local Packages
  DEPENDS:=+libpthread
  SUBMENU:=mtest
  SUBMENUDEP:=
endef

define Package/multicast
  $(call Package/multicast/Default)
  TITLE:=Package for multicast program
endef

define Package/multicast/description
 This package is for multicast program compiling.
endef

define submake_cmd
	+$(MAKE_VARS) \
	$(MAKE) $(PKG_JOBS) -C $(PKG_BUILD_DIR)/$(MAKE_PATH) \
		-f Makefile.qsdk \
		$(MAKE_FLAGS) \
		$(1);
endef

define Build/Compile
	$(call submake_cmd,$$(1))
endef

define Build/Clean
	$(call submake_cmd,clean)
endef

define Package/multicast/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/multicast $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/led_blink.sh $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/multi_cast $(1)/etc/init.d/
endef

$(eval $(call BuildPackage,multicast))
