

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk


PKG_NAME:=blackbox
PKG_VERSION:=2015-06-09
PKG_RELEASE:=

#PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)_$(ARCH).tar.bz2
#PKG_SOURCE_URL:=http://downloads.openwrt.org/sources
PKG_SOURCE_LOCAL:=$(TOPDIR)/local/bsp/$(PKG_NAME)

#PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)$(if $(PKG_VERSION),-$(PKG_VERSION))
#PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)$(if $(PKG_VERSION),-$(PKG_VERSION))
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

define KernelPackage/${PKG_NAME}
  SECTION:=local
  CATEGORY:=Local Packages
  SUBMENU:=BSP
  SUBMENUDEP:=
  DEPENDS:=$(PKG_DEFAULT_DEPENDS) busybox
  TITLE:=Kernel driver for BlackBox
  FILES:=$(PKG_BUILD_DIR)/bb.$(LINUX_KMOD_SUFFIX)
  #AUTOLOAD:=$(call AutoLoad,99,bb)
endef


define KernelPackage/${PKG_NAME}/description
 Black Box to Record kernel printk messages
endef


#define Build/Configure 
#endef 

define Build/Compile
	+$(MAKE_VARS) \
	$(MAKE) $(PKG_JOBS) -C $(LINUX_DIR) \
		$(MAKE_FLAGS) \
		SUBDIRS=$(PKG_BUILD_DIR) modules;
	+$(MAKE_VARS) \
	$(MAKE) $(PKG_JOBS) -C $(PKG_BUILD_DIR)/$(MAKE_PATH) \
		$(MAKE_FLAGS) \
		$$(1);		
endef

#define Build/InstallDev
#endef

## Build/Install
## Build/Dist
## Build/DistCheck

define KernelPackage/${PKG_NAME}/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/showreboot $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/reboot $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/rebootrecordtimer $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/lib/preinit
	cp -f $(PKG_BUILD_DIR)/04_load_blackbox $(1)/lib/preinit/
endef


define Package/kmod-${PKG_NAME}/preinst
#!/bin/sh

busybox_reboot="$${PKG_ROOT}/sbin/reboot"
if [ -L "$${busybox_reboot}" ] && \
   [ "`readlink $${busybox_reboot}`" = "../bin/busybox" ];then
  mv $${busybox_reboot} $${busybox_reboot}-busybox;
  cd `dirname $${busybox_reboot}` && ln -sn /usr/sbin/reboot
fi

endef


define Package/kmod-${PKG_NAME}/prerm
#!/bin/sh

busybox_reboot="$${PKG_ROOT}/sbin/reboot"
[ -e $${busybox_reboot}-busybox ] && mv $${busybox_reboot}-busybox $${busybox_reboot};

endef


$(eval $(call KernelPackage,${PKG_NAME}))
