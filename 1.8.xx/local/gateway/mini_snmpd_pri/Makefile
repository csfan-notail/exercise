# ------------------------------------------------------------------------------
# Copyright (C) 2008 Robert Ernst <robert.ernst@aon.at>
#
# This file may be distributed and/or modified under the terms of the
# GNU General Public License version 2 as published by the Free Software
# Foundation and appearing in the file LICENSE.GPL included in the
# packaging of this file.
#
# This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
# WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
# 
# See COPYING for GPL licensing information.
#



# ------------------------------------------------------------------------------
# Build instructions
#
# For cross-compilation, define CC, e.g. CC=arm-linux-gcc
# For debugging code, add -DDEBUG -g to OFLAGS
# For optimizing code, add -O2 to OFLAGS
# For compiling for IPv4 only, change CFLAGS from -D__IPV6__ to -D__IPV4__
# For compiling for FreeBSD, change CFLAGS from -D__LINUX__ to -D__FREEBSD__
# For compiling the demo extension, add -D__DEMO__ to CFLAGS
# To compile the programm, simply call 'make'
#

obj-app = snmpagentd
obj-dep	= mib.o globals.o utils.o linux.o freebsd.o \
	      interface_app.o \
	  	  interface_route.o \
          snmp_nms.o \
          my_snmp_alarm.o\
          my_snmp_util.o \
          my_snmp_handle.o \
          my_snmp_protocol.o \
          my_snmp_config.o \
          my_snmp_data.o \
          my_snmp_info.o \
          my_snmp_init.o \
          my_snmp_scalar_handler.o \
          my_snmp_table_handler.o \
          my_snmp_trap.o \
          mini_snmpd.o 

ifeq ($(BOARD_TYPE),GW_X86)
obj-dep	+= $(APP_COMMON)/app_interface.o \
  	  $(APP_COMMON)/pal_route.o \
  	  $(APP_COMMON)/pal_portmap.o \
  	  $(APP_COMMON)/network_interface.o \
  	  $(APP_COMMON)/log.o \
  	  $(APP_COMMON)/tool.o \
  	  $(APP_COMMON)/nvram_config.o \
  	  $(APP_COMMON)/restore.o \
  	  $(APP_COMMON)/syslib.o \
  	  $(APP_COMMON)/conversion.o\
  	  $(APP_COMMON)/check_config.o\
  	  $(APP_COMMON)/update_config.o
obj-dep += $(APP_COMMON)/nvram_flash.o 
else ifeq ($(SYSTEM_TYPE),OPENWRT)
obj-dep	+= interface_wireless.o 
else
obj-dep	+= interface_wireless.o \
						$(APP_COMMON)/wireless_interface.o 
endif
	  
VERSION = 1.2b
#VENDOR	= .1.3.6.1.4.1.4881.1.3.1.1.110
VENDOR = .1.3.6.1.4.1.28723
ifeq ($(SYSTEM_TYPE),OPENWRT)
OFLAGS	= -O2 -DSYSLOG -g -pthread -lrt -levent -DGBCOM_ADD -DBIGENDIAN -D__AP_BGSCAN__ $(AP_FLAG) -DMGSOFT 

CFLAGS	= -g -Wall -Wno-unused-variable -DVERSION="\"$(VERSION)\"" -DVENDOR="\"$(VENDOR)\"" \
	  $(OFLAGS) -D__TRAPS__ -D__LINUX__ -D__IPV4__ -DOPENWRT -I$(GW_INCLUDE) -I$(GW_EXT_APP_INCLUDE) -I$(GW_PAL_INCLUDE) -I$(STAGING_DIR)/usr/include/event2
LDFLAGS +=-lpal -lag_share
else

ifeq ($(BOARD_TYPE),GW_X86)
OFLAGS	= -O2 -DSYSLOG -g -static -pthread -lrt -DGBCOM_ADD -DBIGENDIAN -D__AP_BGSCAN__ $(AP_FLAG) -DMGSOFT 
else
OFLAGS	= -O2 -DSYSLOG -g -pthread -lrt -DGBCOM_ADD -DBIGENDIAN -D__AP_BGSCAN__ $(AP_FLAG) -DMGSOFT 
endif
CFLAGS	= -I$(APP_INCLUDE) -I. -I$(KERNELPATH)/include -Wall -Wno-unused-variable -DVERSION="\"$(VERSION)\"" -DVENDOR="\"$(VENDOR)\"" \
	  $(OFLAGS) -D__TRAPS__ -D__LINUX__ -D__IPV4__
ifneq ($(BOARD_TYPE),GW_X86)
CFLAGS += -L$(TOPDIR)/app/common -lcommon -L$(TOPDIR)/wifi/wlan/os/linux/tools -lwifi_pal
LDLIBS = -L$(APP_LIB)
endif

endif

LDFLAGS	+= $(OFLAGS) 

DEST_PATH = $(BIN_PATH)/app/bin
DEST_NAME = $(obj-app)

ifeq ($(SYSTEM_TYPE),OPENWRT)
build: $(obj-dep)
	@echo "BUILD         $(obj-app)"
	@$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -o $(obj-app) $(obj-dep) $(LDLIBS)
	@$(STRIP) -d $(obj-app)

install:
	@echo "INSTALL       $(obj-app)"
	@cp $(obj-app) $(DEST_PATH)/$(DEST_NAME)

#For "find && rm" operation.
export REMOVE_PATTERN= \( -name '*.[oas]' \
                        -o -name '*.ko'  \
                        -o -name 'modules.order' \
                        -o -name 'Module.symvers' \
                        -o -name '*.cmd' \
                        -o -name '*.d'   \
                        -o -name '*.tmp' \
                        -o -name '*.scc' \
                        -o -name '*.tmp_versions' \
                        -o -name '*.depend' \
                        -o -name '*.mod.c' \)
clean:
	@echo "CLEAN         $(obj-app)"
	@find . -follow $(REMOVE_PATTERN) | xargs rm -fr
	@rm -f $(obj-app)
else
ifeq ($(BOARD_TYPE),GW_X86)
CFLAGS += -I$(KERNEL_ROOT)/include -DGW_X86
LDLIBS = -lrt
include $(APPMK)
else
include $(TOPDIR)/build/app.mk
endif
endif
