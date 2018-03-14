include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=ethernet

PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/packages

include $(INCLUDE_DIR)/package.mk
ifeq ($(DUMP),)
	-include $(LINUX_DIR)/.config
endif

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Configure
endef

define Build/Compile
endef


define KernelPackage/eth-atheros
  SECTION:=local
  CATEGORY:=Local Packages
  SUBMENU:=BSP
  TITLE:=Atheros built-in ethernet mac & phy support
  DEPENDS:=@PCI_SUPPORT
  KCONFIG:=CONFIG_AR80XX_PHY \
    CONFIG_AR8216_PHY \
    CONFIG_NET_VENDOR_ATHEROS   
  FILES:=$(LINUX_DIR)/drivers/net/phy/ar80xx.ko \
    $(LINUX_DIR)/drivers/net/phy/ar8216.ko \
    $(LINUX_DIR)/drivers/net/ethernet/atheros/ag71xx/ag71xx.ko        
  #AUTOLOAD:=$(call AutoLoad,50,eth-atheros)
endef

define KernelPackage/eth-atheros/install
	$(INSTALL_DIR) $(1)/lib/preinit
	$(INSTALL_DATA) ./files/04_load_ethernet $(1)/lib/preinit/
endef


$(eval $(call KernelPackage,eth-atheros))

