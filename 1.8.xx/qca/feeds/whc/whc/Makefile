include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/package.mk

PKG_NAME:=whc
PKG_RELEASE:=1

COMMON_PKGS += +qca-whc-lbd +luci-whc +luci-whc-advanced +luci-whc-diaglog

define Package/$(PKG_NAME)
  SECTION:=QCA
  CATEGORY:=QCA Proprietary software
  URL:=http://www.qca.qualcomm.com
  MAINTAINER:=Qualcomm Atheros
  TITLE:=Whole Home Coverage - selects all Whole Home Coverage components for build
  DEPENDS:=$(COMMON_PKGS)
  MAINTAINER:=Qualcomm Atheros
endef

define Package/$(PKG_NAME)/description
  Meta package which selects all Whole Home Coverage components required for build
endef

define Build/Compile
endef

define Package/$(PKG_NAME)/install
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
