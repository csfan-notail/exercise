##
## openwrt package Makefile example, refer to openwrt_documents for details.
##


include $(TOPDIR)/rules.mk

##kernel.mk only necessary when building kernel modules
##include $(INCLUDE_DIR)/kernel.mk

:
##PKG_NAME 
##  The name of the package, as seen via menuconfig and ipkg
##PKG_VERSION 
##  The upstream version number that we are downloading
##PKG_RELEASE 
##  The version of this package Makefile
##PKG_SOURCE 
##  The filename of the original sources
##PKG_SOURCE_URL 
##  Where to download the sources from (no trailing slash), you can add multiple 
##  download sources by separating them with a and a carriage return.
##PKG_MD5SUM 
##  A checksum to validate the download
##PKG_CAT 
##  How to decompress the sources (zcat, bzcat, unzip)
##PKG_BUILD_DIR 
##  Where to compile the package
PKG_NAME:=pal
PKG_VERSION:=2015-11-06
PKG_RELEASE:=

#PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)_$(ARCH).tar.bz2
#PKG_SOURCE_URL:=http://downloads.openwrt.org/sources
PKG_SOURCE_LOCAL:=$(TOPDIR)/local/gateway/$(PKG_NAME)

#PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)$(if $(PKG_VERSION),-$(PKG_VERSION))
#PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)$(if $(PKG_VERSION),-$(PKG_VERSION))
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk
define Package/pal/Default
  SECTION:=local
  CATEGORY:=Local Packages
  DEPENDS:=
  SUBMENU:=GateWay
  SUBMENUDEP:=
endef

##Package/<name>: 
##<name> matches the argument passed to buildroot, this describes the package 
##the menuconfig and ipkg entries. Within Package/<name> you can define the 
##following variables:
##SECTION 
##  The type of package (currently unused)
##CATEGORY 
##  Which menu it appears in menuconfig: Network, Sound, Utilities, Multimedia ...
##TITLE 
##  A short description of the package
##URL 
##  Where to find the original software
##MAINTAINER (optional) 
##  Who to contact concerning the package
##DEPENDS (optional) 
##  Which packages must be built/installed before this package. To reference a 
##  dependency defined in the same Makefile, use <dependency name>. If defined 
##  as an external package, use +<dependency name>. For a kernel version dependency 
##  use: @LINUX_2_<minor version>
##  *** As defined in package-default.mk,PKG_FIXUP_DEPENDS will add 
##  PKG_DEFAULT_DEPENDS = +libc +USE_EGLIBC:librt +USE_EGLIBC:libpthread
##  to DEPENDS we defined except kernelpackage. So we should add dependency fo libc 
##  manully if our kmod package contains user space program binaries. ***
##BUILDONLY (optional) 
##  Set this option to 1 if you do NOT want your package to appear in menuconfig. 
##  This is useful for packages which are only used as build dependencies.
define Package/pal
  $(call Package/pal/Default)
  TITLE:=a share lib for common gateway api
endef

##TITLE 
##  The name of the module as seen via menuconfig
##DESCRIPTION 
##  The description as seen via help in menuconfig
##SUBMENU 
##  The sub menu under which this package will be seen
##KCONFIG 
##  Kernel configuration option dependency. For external modules, remove it.
##FILES 
##  Files you want to inlude to this kernel module package, separate with spaces.
##AUTOLOAD 
##  Modules that will be loaded automatically on boot, the order you write them is 
##  the order they would be loaded.
define Package/pal/description
 This package is pal for control nf_portal
endef

## Build/Prepare (optional): A set of commands to unpack and patch the sources. 
## For local source: we make link from source dir to build dir

## Build/Configure (optional): 
## You can leave this undefined if the source doesn¡¯t use configure or 
## has a normal config script, otherwise you can put your own commands here or 
## use "$(call Build/Configure/Default,<first list of arguments, second list>)" 
## as above to pass in additional arguments for a standard configure script. 
#define Build/Configure 
#endef 

## Build/InstallDev (optional): 
## If your package provides a library that needs to be made available to other packages, 
## you can use the Build/InstallDev template to copy it into the staging directory 
## which is used to collect all files that other packages might depend on at build time.
define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/*.so $(1)/usr/lib/
endef

## Build/Install
## Build/Dist
## Build/DistCheck

##Package/<name>/install: 
##A set of commands to copy files out of the compiled source and into the 
##ipkg which is represented by the $(1) directory. Note that there are 
##currently 4 defined install macros:
##INSTALL_DIR 
##  install -d -m0755
##INSTALL_BIN 
##  install -m0755
##INSTALL_DATA 
##  install -m0644
##INSTALL_CONF 
##  install -m0600
##
##
define Package/pal/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/*.so $(1)/usr/lib/
endef


## create instantiation for makefile rules
$(eval $(call BuildPackage,pal))
