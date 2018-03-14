#CC=/home/apenv/ar9344/build/gcc-4.3.3/build_mips/staging_dir/usr/bin/mips-linux-uclibc-gcc
#LD=/home/apenv/ar9344/build/gcc-4.3.3/build_mips/staging_dir/usr/bin/mips-linux-uclibc-ld
#AR=/home/apenv/ar9344/build/gcc-4.3.3/build_mips/staging_dir/usr/bin/mips-linux-uclibc-ar

lib-app = libhttpd.a
lib-obj = protocol.o
lib-obj += api.o
lib-obj += version.o
lib-obj += ip_acl.o

CFLAGS = -DSO_REUSEADDR
ifneq ($(SYSTEM_TYPE),OPENWRT)
		  
libhttpd: $(lib-obj)
	$(AR) crus $(lib-app) $(lib-obj)
	cp ./$(lib-app) ../../lib/$(lib-app)
else
libhttpd:build install

build: $(lib-obj)
	$(AR) crus $(lib-app) $(lib-obj)
install:
	cp $(lib-app) ../../lib/$(lib-app)
endif
clean:
	rm $(lib-obj) $(lib-app) -f
