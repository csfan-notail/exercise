/* lib/version.h.  Generated from version.h.in by configure.
 *
 * Quagga version
 * Copyright (C) 1997, 1999 Kunihiro Ishiguro
 * 
 * This file is part of GNU Zebra.
 *
 * GNU Zebra is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * GNU Zebra is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Zebra; see the file COPYING.  If not, write to the Free
 * Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.  
 */

#ifndef _ZEBRA_VERSION_H
#define _ZEBRA_VERSION_H

#ifdef GIT_VERSION
#include "gitversion.h"
#endif

#ifndef GIT_SUFFIX
#define GIT_SUFFIX ""
#endif
#ifndef GIT_INFO
#define GIT_INFO ""
#endif

#define QUAGGA_PROGNAME   "Quagga"

#define QUAGGA_VERSION     "1.0.20160315" GIT_SUFFIX

#define ZEBRA_BUG_ADDRESS "https://bugzilla.quagga.net"

#define QUAGGA_URL "http://www.quagga.net"

#define QUAGGA_COPYRIGHT "Copyright 1996-2005 Kunihiro Ishiguro, et al."

#define QUAGGA_CONFIG_ARGS "--target=mips-openwrt-linux-uclibc --host=mips-openwrt-linux-uclibc --build=x86_64-linux-gnu --program-prefix= --program-suffix= --prefix=/usr --exec-prefix=/usr --bindir=/usr/bin --sbindir=/usr/sbin --libexecdir=/usr/lib --sysconfdir=/etc --datadir=/usr/share --localstatedir=/var --mandir=/usr/man --infodir=/usr/info --disable-nls --localstatedir=/var/run/quagga --sysconfdir=/etc/quagga/ --enable-shared --disable-static --enable-user=network --enable-group=network --enable-pie=no --enable-multipath=8 --disable-ospfclient --disable-capabilities --disable-doc --enable-zebra --enable-ospfd --disable-bgpd --disable-isisd --disable-ospf6d --enable-ripd --disable-ripngd --disable-pimd --enable-vtysh"

pid_t pid_output (const char *);

#ifndef HAVE_DAEMON
int daemon(int, int);
#endif

#endif /* _ZEBRA_VERSION_H */
