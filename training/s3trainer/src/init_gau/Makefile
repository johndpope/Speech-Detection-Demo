include ../../Makefile.global

TARGET = init_gau
LIBS   = -lio -lmllr -lmodinv -lcommon -ls2io -lcep_feat -lm
DEP_LIBS = \
	$(PKG_ROOT)/lib.$(MACHINE)/libio.a \
	$(PKG_ROOT)/lib.$(MACHINE)/libmllr.a \
	$(PKG_ROOT)/lib.$(MACHINE)/libmodinv.a \
	$(PKG_ROOT)/lib.$(MACHINE)/libcommon.a \
	$(PKG_ROOT)/lib.$(MACHINE)/libs2io.a \
	$(PKG_ROOT)/lib.$(MACHINE)/libcep_feat.a

DBG_TARGET = $(TARGET)_dbg
DBG_LIBS   = -lio_dbg -lmllr_dbg -lmodinv_dbg -lcommon_dbg -ls2io_dbg -lcep_feat_dbg -lm
DBG_DEP_LIBS =	\
	$(PKG_ROOT)/lib.$(MACHINE)/libio_dbg.a \
	$(PKG_ROOT)/lib.$(MACHINE)/libmllr_dbg.a \
	$(PKG_ROOT)/lib.$(MACHINE)/libmodinv_dbg.a \
	$(PKG_ROOT)/lib.$(MACHINE)/libcommon_dbg.a \
	$(PKG_ROOT)/lib.$(MACHINE)/libs2io_dbg.a \
	$(PKG_ROOT)/lib.$(MACHINE)/libcep_feat_dbg.a

OBJS  = main.o \
	init_gau.o \
	accum.o \
	parse_cmd_ln.o

SRCS  = main.c \
	init_gau.c \
	accum.c \
	parse_cmd_ln.c

#
# Following is copied from Makefile.binrules
#
#
# Common rules for binary files (3-Jul-96)
#

default:
	(cd $(MACHINE); $(MAKE) ${$(MACHINE)_MFLAGS} -f ../Makefile VPATH=.. $(TARGET))

debug:
	(cd $(MACHINE)_dbg; $(MAKE) ADDL_CFLAGS=-g ${$(MACHINE)_MFLAGS} -f ../Makefile VPATH=.. $(DBG_TARGET))

clean:
	(cd $(MACHINE); rm -f $(OBJS))

relink:
	(cd $(MACHINE); rm -f $(TARGET))

depend:
	mkdir -p $(MACHINE)
	mkdir -p $(MACHINE)_dbg
	makedepend -f Makefile.depend.$(MACHINE) $(DEPEND_HPATHS) $(SRCS)
	sed -e 's+\./+\.\./+g' Makefile.depend.$(MACHINE) > tmp.$(MACHINE)
	mv -f tmp.$(MACHINE) Makefile.depend.$(MACHINE)

include Makefile.depend.$(MACHINE)

$(TARGET): $(OBJS) $(DEP_LIBS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS) $(LIBS)

$(DBG_TARGET): $(OBJS) $(DBG_DEP_LIBS)
	$(PURIFY) $(CC) $(CFLAGS) -o $(DBG_TARGET) $(OBJS) $(LDFLAGS) $(DBG_LIBS)
	cp -p $(DBG_TARGET) $(BINDIR)

# perform installation if necessary
install:
	( cd $(MACHINE); $(MAKE) ${$(MACHINE)_MFLAGS} -f ../Makefile VPATH=.. $(BINDIR)/$(TARGET) )

$(BINDIR)/$(TARGET): $(TARGET)
	-@mv $(BINDIR)/$(TARGET) $(BINDIR)/$(TARGET).old
	-@rm $(BINDIR)/$(TARGET).old
	cp -p $(TARGET) $(BINDIR)
