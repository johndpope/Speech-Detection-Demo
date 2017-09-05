include ../../Makefile.global

ADDL_LDFLAGS = -ygauden_alloc_param

TARGET = printp
LIBS   = \
	-lio \
	-lmodinv \
	-lcep_feat \
	-lcommon \
	-lm
DEP_LIBS = \
	$(LIBDIR)/libio.a \
	$(LIBDIR)/libmodinv.a \
	$(LIBDIR)/libcep_feat.a \
	$(LIBDIR)/libcommon.a

DBG_TARGET = $(TARGET)_dbg
DBG_LIBS = \
	-lio_dbg \
	-lmodinv_dbg \
	-lcep_feat_dbg \
	-lcommon_dbg \
	-lm
DBG_DEP_LIBS = \
	$(LIBDIR)/libio_dbg.a \
	$(LIBDIR)/libmodinv_dbg.a \
	$(LIBDIR)/libcep_feat_dbg.a \
	$(LIBDIR)/libcommon_dbg.a

OBJS  = parse_cmd_ln.o \
	main.o

SRCS  = parse_cmd_ln.c \
	main.c

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
# DO NOT DELETE
