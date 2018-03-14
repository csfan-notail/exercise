

include $(TOPDIR)/config.mk

LIB	= libcustom.a

AOBJS	=

COBJS	= board.o ddr_test.o bootinfo.o bootargs.o nvram_flash.o ping_restore.o \
          update_flashimage.o watchdog.o

OBJS	= $(AOBJS) $(COBJS)

$(LIB):	.depend $(OBJS)
	$(AR) crv $@ $(OBJS)

#########################################################################

.depend:	Makefile $(AOBJS:.o=.S) $(COBJS:.o=.c)
		$(CC) -M $(CFLAGS) $(AOBJS:.o=.S) $(COBJS:.o=.c) > $@

sinclude .depend

#########################################################################
