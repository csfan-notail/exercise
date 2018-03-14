.PHONY: all clean distclean

ifneq ($(cc),)
CC = $(cc)
endif
ifneq ($(cflags),)
CFLAGS = $(cflags)
endif

SRCDIR = $(shell pwd)
BLDDIR = $(SRCDIR)/.build
OBJDIR = $(BLDDIR)/obj

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

CFLAGS += -I$(top_dir) -I$(top_dir)/include
ifeq ($(top_dir),)
$(warning cannot find $$(top_dir))
endif

all: $(OBJS)

$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	@echo "    [CC]    $(notdir $@)"
	@$(CC) -c -o $@ $(CFLAGS) $<

clean:
	-rm -rf $(OBJDIR)

distclean:
	-rm -rf $(BLDDIR)
