include ../../Makefile.global

TARGET   = agg_seg
LIBS     = \
	-lio \
	-lmllr \
	-lio \
	-ls2io \
	-lcep_feat \
	-lcommon  \
	-lm
DEP_LIBS = \
	$(LIBDIR)/libio.a \
	$(LIBDIR)/libmllr.a \
	$(LIBDIR)/libs2io.a \
	$(LIBDIR)/libcep_feat.a \
	$(LIBDIR)/libcommon.a

DBG_TARGET   = $(TARGET)_dbg
DBG_LIBS     = \
	-lio_dbg \
	-lmllr_dbg \
	-lio_dbg \
	-ls2io_dbg \
	-lcep_feat_dbg \
	-lcommon_dbg \
	-lm
DBG_DEP_LIBS = \
	$(LIBDIR)/libio_dbg.a \
	$(LIBDIR)/libmllr_dbg.a \
	$(LIBDIR)/libcep_feat_dbg.a \
	$(LIBDIR)/libs2io_dbg.a \
	$(LIBDIR)/libcommon_dbg.a

OBJS  = main.o \
\
	cnt_st_seg.o \
	cnt_phn_seg.o \
\
	agg_phn_seg.o \
	agg_st_seg.o \
	agg_all_seg.o \
\
	mk_seg.o \
	parse_cmd_ln.o

SRCS  = main.c \
\
	cnt_st_seg.c \
	cnt_phn_seg.c \
\
	agg_phn_seg.c \
	agg_st_seg.c \
	agg_all_seg.c \
\
	mk_seg.c \
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
