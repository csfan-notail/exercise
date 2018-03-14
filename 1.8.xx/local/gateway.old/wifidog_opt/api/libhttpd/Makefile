
lib-app = libhttpd.a
lib-obj = protocol.o
lib-obj += api.o
lib-obj += version.o
lib-obj += ip_acl.o

CFLAGS = -DSO_REUSEADDR
libhttpd:build install

build: $(lib-obj)
	$(AR) crus $(lib-app) $(lib-obj)
install:
	cp $(lib-app) ../../lib/$(lib-app)

clean:
	rm $(lib-obj) $(lib-app) -f
