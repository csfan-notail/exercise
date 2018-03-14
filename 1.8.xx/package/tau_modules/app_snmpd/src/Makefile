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

obj-app = mini_snmpd
obj-dep	= mib.o globals.o utils.o linux.o \
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
          mini_snmpd.o \
	  	  log.o 
		  
VERSION = 1.2b
VENDOR	= .1.3.6.1.4.1.171.10.132.1.1

OFLAGS	= -O2 -DSYSLOG -g -pthread -levent -DGBCOM_ADD -DBIGENDIAN -D__AP_BGSCAN__ $(AP_FLAG) -DMGSOFT  -DAP9344_OAM

LDLIBS += -L$(LTE_INCLUDE) -L$(TAU_LIBDIR) -L$(LIB_PATH) -lutil -lftp -lpal -lomnet -lreg

CFLAGS	+= -g -Wall -Wno-unused-variable -DVERSION="\"$(VERSION)\"" -DVENDOR="\"$(VENDOR)\"" \
	  $(OFLAGS) -D__TRAPS__ -D__LINUX__ -D__IPV4__ -DOPENWRT -I$(GW_INCLUDE) -I$(GW_EXT_APP_INCLUDE) -I$(GW_PAL_INCLUDE) -I$(STAGING_DIR)/usr/include/event2  
CFLAGS	+= -I$(TOPDIR)/app/ui/rmtMgr/mini_snmpd/include -I$(LTE_INCLUDE) -I$(TAU_INCLUDE)
DEST_PATH = $(BIN_PATH)/app/bin
DEST_NAME = $(obj-app)

build: $(obj-dep)
	@echo "BUILD         $(obj-app)"
	@$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -o $(obj-app) $(obj-dep)
	@$(STRIP) -d $(obj-app)


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
	
	
