#
# @@-COPYRIGHT-START-@@
#
# Copyright (c) 2014 Qualcomm Atheros, Inc.
# All rights reserved.
# Qualcomm Atheros Confidential and Proprietary.
#
# @@-COPYRIGHT-END-@@
#

include $(TOPDIR)/rules.mk

PKG_NAME:=whc-ui
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

define Package/luci-whc/Default
  SECTION:=QCA
  CATEGORY:=QCA Proprietary software
  URL:=http://www.qca.qualcomm.com
  MAINTAINER:=Qualcomm Atheros
  SUBMENU:=Whole Home Coverage features
endef

define Package/luci-whc
  $(call Package/luci-whc/Default)
  DEPENDS:=+luci +luci-mod-admin-core +luci-mod-admin-full
  TITLE:=WHC Web UI
endef

define Package/luci-whc/description
  Luci WebUI for Whole Home Coverage
endef

define Package/luci-whc-advanced
  $(call Package/luci-whc/Default)
  DEPENDS:=+luci +luci-mod-admin-core +luci-mod-admin-full +luci-whc
  TITLE:=WHC Web UI - Advanced
endef

define Package/luci-whc-advanced/description
  Luci WebUI for Whole Home Coverage - Advanced
endef

define Package/luci-whc-diaglog
  $(call Package/luci-whc/Default)
  DEPENDS:=+luci +luci-mod-admin-core +luci-mod-admin-full +luci-whc
  TITLE:=WHC Web UI - Diagnostic Logging
endef

define Package/luci-whc-diaglog/description
  Luci WebUI for Whole Home Coverage - Diagnostic Logging
endef

define Build/Compile
	true
endef

define Package/luci-whc/install
	$(INSTALL_DIR) $(1)/usr/lib/lua/luci/model/cbi/whc
	$(CP) files/luci-whc/luasrc/model/cbi/whc/whc.lua /$(1)/usr/lib/lua/luci/model/cbi/whc/
	$(INSTALL_DIR) $(1)/usr/lib/lua/luci/view/whc
	$(CP) files/luci-whc/luasrc/view/whc/dview_js.htm /$(1)/usr/lib/lua/luci/view/whc/
	$(INSTALL_DIR) $(1)/usr/lib/lua/luci/controller
	$(CP) files/luci-whc/luasrc/controller/whc.lua /$(1)/usr/lib/lua/luci/controller/
endef

define Package/luci-whc-advanced/install
	$(INSTALL_DIR) $(1)/usr/lib/lua/luci/model/cbi/whc
	$(CP) files/luci-whc/luasrc/model/cbi/whc/advanced.lua /$(1)/usr/lib/lua/luci/model/cbi/whc/
	$(INSTALL_DIR) $(1)/usr/lib/lua/luci/view/whc
	$(CP) files/luci-whc/luasrc/view/whc/btn_adv.htm /$(1)/usr/lib/lua/luci/view/whc/
endef

define Package/luci-whc-diaglog/install
	$(INSTALL_DIR) $(1)/usr/lib/lua/luci/model/cbi/whc
	$(CP) files/luci-whc/luasrc/model/cbi/whc/diaglog.lua /$(1)/usr/lib/lua/luci/model/cbi/whc/
	$(INSTALL_DIR) $(1)/usr/lib/lua/luci/view/whc
	$(CP) files/luci-whc/luasrc/view/whc/btn_diag.htm /$(1)/usr/lib/lua/luci/view/whc/
endef

$(eval $(call BuildPackage,luci-whc))
$(eval $(call BuildPackage,luci-whc-advanced))
$(eval $(call BuildPackage,luci-whc-diaglog))

