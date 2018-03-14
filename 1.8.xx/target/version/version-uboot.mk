#
# liyugao: rules to pack final u-boot versions
#

#include $(TOPDIR)/rules.mk
#include $(INCLUDE_DIR)/image.mk

UBOOT_VER?=2.0
UBOOT_PROFILE?=cgw600 cgw800 ta62_51_ram64 ta62_51_ram128 ta30_51 ta62_50

UBOOT_COMMON_SIZE=0x40000
UBOOT_INFO_SIZE=0x400

# Parameters: <profile> <version name> <raw uboot.bin> <Hardware> <PcbCode> <PcbVer> <BomCode>
define UbootProfile
  UBOOT_VERSION_$(1)=$(2)
  UBOOT_DST_BIN_$(1)=$(VERSION_DIR)/$$(UBOOT_VERSION_$(1)).bin
  UBOOT_SRC_BIN_$(1)=$(3)
  UBOOT_SRC_SIZE_$(1)=$(shell echo $$[$(UBOOT_COMMON_SIZE) - $(UBOOT_INFO_SIZE)])
  UBOOT_HARDWARE_$(1)=$(4)
  UBOOT_PCBCODE_$(1)=$(5)
  UBOOT_PCBVER_$(1)=$(6)
  UBOOT_BOMCODE_$(1)=$(7)

endef

# Parameters: <string> <size> <file>
define PadString
	echo -n $(1) | dd bs=$(2) conv=sync 2>/dev/null >> $(3)
endef

# Parameters: <profile>
# bootinfo = version(64Bytes) + Hardware(16Bytes) + PcbCode(16Bytes) + PcbVer(16Bytes) + BomCode(64Bytes)
define BuildUboot
$(UBOOT_DST_BIN_$(1)): $(VERSION_DIR) $(UBOOT_SRC_BIN_$(1)) 
	@if [ `stat -c%s "$(UBOOT_SRC_BIN_$(1))"` -gt "$(UBOOT_SRC_SIZE_$(1))" ]; then	\
	  echo "Error: `basename $(UBOOT_SRC_BIN_$(1))` is too big (Max=$(UBOOT_SRC_SIZE_$(1)) Current=`stat -c%s "$(UBOOT_SRC_BIN_$(1))"`)"; \
	  exit 1;	\
	else	\
	  echo "Generating `basename $$@`...File:`basename $(UBOOT_SRC_BIN_$(1))` Hardware:$(UBOOT_HARDWARE_$(1)) PcbCode:$(UBOOT_PCBCODE_$(1)) PcbVer:$(UBOOT_PCBVER_$(1)) BomCode:$(UBOOT_BOMCODE_$(1))...";	\
	  dd if=$(UBOOT_SRC_BIN_$(1)) bs=$(UBOOT_SRC_SIZE_$(1)) conv=sync 2>/dev/null > $$@;	\
	  $(call PadString,$(UBOOT_VERSION_$(1)),64,$$@);	\
	  $(call PadString,$(UBOOT_HARDWARE_$(1)),16,$$@);\
	  $(call PadString,$(UBOOT_PCBCODE_$(1)),16,$$@);	\
	  $(call PadString,$(UBOOT_PCBVER_$(1)),16,$$@);	\
	  $(call PadString,$(UBOOT_BOMCODE_$(1)),64,$$@);	\
	fi

endef

# Generate Profiles
#modified by xiaowei 2016-2-25 
#$(eval $(call UbootProfile,cgw600,boot_ta37_22_x_m0_$(UBOOT_VER),$(BIN_DIR)/openwrt-ar71xx-ta37-16M-qca-legacy-uboot.bin,TA37,TA37-22,X,M0))
#$(eval $(call UbootProfile,cgw800,boot_ta37_40_x_r0_$(UBOOT_VER),$(BIN_DIR)/openwrt-ar71xx-ta37-16M-qca-legacy-uboot.bin,TA37,TA37-40,X,R0))
#$(eval $(call UbootProfile,ta30_51,boot_ta30_51_x_w0_$(UBOOT_VER),$(BIN_DIR)/openwrt-ar71xx-ta30-51-16M-qca-legacy-uboot.bin,TA30,TA30-51,X,W0))
#$(eval $(call UbootProfile,ta62_51_ram64,boot_ta62_51_x_n0_$(UBOOT_VER),$(BIN_DIR)/openwrt-ar71xx-ta62-16M-qca-legacy-uboot.bin,TA62,TA62-51,X,N0))
#$(eval $(call UbootProfile,ta62_51_ram128,boot_ta62_51_x_n1_$(UBOOT_VER),$(BIN_DIR)/openwrt-ar71xx-ta62-16M-qca-legacy-uboot.bin,TA62,TA62-51,X,N1))
#$(eval $(call UbootProfile,ta62_50,boot_ta62_50_x_z0_$(UBOOT_VER),$(BIN_DIR)/openwrt-ar71xx-ta62-16M-qca-legacy-uboot.bin,TA62,TA62-50,X,Z0))
$(eval $(call UbootProfile,cgw600,boot_ta321_70_x_ac_$(UBOOT_VER),$(BIN_DIR)/openwrt-ar71xx-ta37-16M-qca-legacy-uboot.bin,TA37,TA37-22,X,M0))

# Generate Rules
$(eval $(foreach profile,$(UBOOT_PROFILE),$(call BuildUboot,$(profile))))

VERSION_TARGET_INSTALL+=$(foreach profile,$(UBOOT_PROFILE),$(UBOOT_DST_BIN_$(profile)))

