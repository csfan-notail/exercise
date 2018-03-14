
EXEC = tr069

ifeq ($(top_dir),)
top_dir = $(shell pwd)
endif
export top_dir

BLDDIR = $(top_dir)/.build

CFLAGS += -Wall -g -I$(TAU_INCLUDE) -I$(top_dir)/include

LDLIBS += -Wl,-rpath,$(BLDDIR)  \
		-L$(TAU_LIBDIR) -lutil -lcrypt -lusb-1.0  -lftp  -lpal  -lcrypto -lssl -lomnet -lreg
		
_sub_dirs_ = lib SOAPParser webproto nanoxml core standard gbcom
sub_dirs = $(patsubst %, $(top_dir)/%, $(_sub_dirs_))
MAKE_OPTS = top_dir=$(top_dir) cflags="$(CFLAGS)"

$(BLDDIR)/$(EXEC) : $(sub_dirs)
	@mkdir -p $(BLDDIR)	
	@for d in $(sub_dirs); do make -C $$d $(MAKE_OPTS); done
	@echo "    [LD]    $(EXEC)"
	$(CC) $(LDFLAGS) -o $(BLDDIR)/$(EXEC)  $(LDLIBS) \
		`find $(patsubst %, %/.build/obj, $(sub_dirs)) -name *.o|xargs`

 	
