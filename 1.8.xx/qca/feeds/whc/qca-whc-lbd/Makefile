include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=qca-whc-lbd
PKG_SOURCE_PROTO:=git
PKG_BRANCH:=whc/banana
PKG_RELEASE:=1

include $(INCLUDE_DIR)/local-development.mk
ifeq ($(DUMP)$(PKG_VERSION),)
  PKG_VERSION:=$(shell git ls-remote $(PKG_SOURCE_URL) $(PKG_BRANCH) | cut -b -7)
endif

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_VERSION)

WIFI_10_4:=PACKAGE_kmod-qca-wifi-10.4-akronite-perf
WIFI_10_2_MIPS:=PACKAGE_kmod-qca-wifi-perf
WIFI_10_2_ARM:=PACKAGE_kmod-qca-wifi-akronite-perf
WIFI_PKGS:=$(WIFI_10_4)||$(WIFI_10_2_MIPS)||$(WIFI_10_2_ARM)

PKG_BUILD_DEPENDS:=$(WIFI_10_4):qca-wifi-10.4 $(WIFI_10_2_MIPS):qca-wifi $(WIFI_10_2_ARM):qca-wifi libhyficommon

QCAWHCLBD_MAKE_OPTS:= \
        CROSS=$(TARGET_CROSS) \
        GWLIB=$(STAGING_DIR)/usr/lib \
        LBTOP=$(PKG_BUILD_DIR) \
        GWINCLUDE=$(STAGING_DIR)/usr/include/hyficommon \
        ATHDIR=$(STAGING_DIR)/usr/include \
        INSTALL_ROOT=$(PKG_BUILD_DIR)/install \
        STAGING_DIR=$(STAGING_DIR)

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

define Package/qca-whc-lbd/config
  $(call Package/$(PKG_NAME)/override_version,$(PKG_NAME))
  $(call Package/$(PKG_NAME)/override_source_path,$(PKG_NAME))
endef

define Package/qca-whc-lbd
  SECTION:=QCA
  CATEGORY:=QCA Proprietary software
  URL:=http://www.qca.qualcomm.com
  MAINTAINER:=Qualcomm Atheros
  DEPENDS:=+libhyficommon +libpthread +monit +@BUSYBOX_CONFIG_STAT \
	   +@BUSYBOX_CONFIG_FEATURE_STAT_FORMAT \
	   @$(WIFI_PKGS)
  TITLE:=Whole Home Coverage Load Balancing daemon
  SUBMENU:=Whole Home Coverage features
endef

define Package/qca-whc-lbd/description
  This package installs the Whole Home Coverage Load Balancing daemon
endef

define Build/Compile
	mkdir -p $(PKG_BUILD_DIR)/install/lib
	mkdir -p $(PKG_BUILD_DIR)/install/include
	mkdir -p $(PKG_BUILD_DIR)/install/sbin
	$(MAKE) -C $(PKG_BUILD_DIR) $(strip $(QCAWHCLBD_MAKE_OPTS)) subdirs
endef

define Package/qca-whc-lbd/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_DIR) $(1)/lib/functions/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/install/sbin/lbd $(1)/usr/sbin
	$(INSTALL_BIN) ./files/lbt $(1)/usr/sbin/lbt
	$(INSTALL_CONF) ./files/lbd.config $(1)/etc/config/lbd
	$(INSTALL_BIN) ./files/lbd.init $(1)/etc/init.d/lbd
	$(INSTALL_DIR) $(1)/etc/hotplug.d/net
	$(INSTALL_BIN) ./files/lbd.hotplug $(1)/etc/hotplug.d/net/31-lbd
	$(CP) ./files/whc-debug.sh $(1)/lib/functions/
	$(CP) ./files/whc-iface.sh $(1)/lib/functions/
endef

$(eval $(call BuildPackage,qca-whc-lbd))
