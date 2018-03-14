#
# liyugao: rules to pack final rootfs versions
#

#include $(TOPDIR)/rules.mk
#include $(INCLUDE_DIR)/image.mk

wc_c=cat $(1) | wc -c

VERSION_TAIL_HEADID:=0001
VERSION_TAIL_HEAD:=$(VERSION_TAIL_HEADID)-$(shell $(SH_FUNC) echo "headid=$(VERSION_TAIL_HEADID)" | md5s)
#VERSION_TAIL_HEAD=${VERSION_TAIL_HEADID}-`echo "headid=${VERSION_TAIL_HEADID}" | md5sum | awk '{print $1}'`

VER?=0.0.2.9
VERSION_TARGET?=ta321
VERSION_ROOTFS_TYPE=squashfs-64k

VERSION_CTRL_DIR=version_ctrl

# Parameters: <board> <rootfs type>
define VersionDefault
#modified by xiaowei 2016-2-25 
  #VERSION_NAME_$(1)=gw-$(1)-$(VER)
  VERSION_NAME_$(1)=h3000-$(VER)
  VERSION_DST_$(1)=$(VERSION_DIR)/$$(VERSION_NAME_$(1)).bin
  VERSION_DST_MAXSIZE_$(1)=6225920
  VERSION_SRC_ROOTFS_$(1):=$(KDIR)/root.$(if $(2),(2),$(VERSION_ROOTFS_TYPE))
  VERSION_SRC_ROOTFS_SIZE_$(1)=$$(shell $(call wc_c,$$(VERSION_SRC_ROOTFS_$(1))))
  VERSION_SRC_ROOTFS_MD5_$(1)=$$(shell $(SH_FUNC) md5s $$(VERSION_SRC_ROOTFS_$(1)))
  VERSION_SRC_CTRL_$(1)=default_ctrl
  VERSION_SRC_CTRL_TAR_$(1)=$(VERSION_DIR)/version_ctrl_$(1).tar.bz2
  
endef


# Parameters: <board>
# version = rootfs + tail + version_ctrl.tar.bz2
# tail = <tail head> <version name> <rootfs size> <rootfs md5> <version_ctrl.tar.bz2 size> 
define BuildVersion
$(VERSION_SRC_CTRL_TAR_$(1)): $(VERSION_DIR) FORCE
	@#echo Packing `basename $$@`...;
	@-rm -fr $$@;
	@-rm -fr $(VERSION_CTRL_DIR);
	@[ -d $(VERSION_SRC_CTRL_$(1)) ] && cp -fr $(VERSION_SRC_CTRL_$(1)) $(VERSION_CTRL_DIR);
	@tar -jcf $$@ $(VERSION_CTRL_DIR);
	@-rm -fr $(VERSION_CTRL_DIR);

$(VERSION_DST_$(1)): $(VERSION_DIR) $(VERSION_SRC_ROOTFS_$(1)) $(VERSION_SRC_CTRL_TAR_$(1))
	@echo Generating `basename $$@`...;
	@-rm -fr $$@;	
	@cp $(VERSION_SRC_ROOTFS_$(1)) $(VERSION_DST_$(1));	
	@echo -e "\n$(VERSION_TAIL_HEAD) $(VERSION_NAME_$(1)) $(VERSION_SRC_ROOTFS_SIZE_$(1)) $(VERSION_SRC_ROOTFS_MD5_$(1)) `$(call wc_c,$(VERSION_SRC_CTRL_TAR_$(1)))`" >> $(VERSION_DST_$(1));
	@cat $(VERSION_SRC_CTRL_TAR_$(1)) >>  $(VERSION_DST_$(1));		
	@-rm -fr $(VERSION_SRC_CTRL_TAR_$(1));
	@if [ `$(call wc_c,$$@)` -gt $(VERSION_DST_MAXSIZE_$(1)) ];then	\
	  echo "Error, `basename $$@` is too big (Max=$(VERSION_DST_MAXSIZE_$(1)) Current=`$(call wc_c,$$@)` ).";	\
	  rm $$@ && false;	\
	fi

endef

$(eval $(foreach board,$(VERSION_TARGET),$(call VersionDefault,$(board))))

# override default variables here
# do nothing for now
#

$(eval $(foreach board,$(VERSION_TARGET),$(call BuildVersion,$(board))))

VERSION_TARGET_INSTALL+=$(foreach board,$(VERSION_TARGET),$(VERSION_DST_$(board)))




