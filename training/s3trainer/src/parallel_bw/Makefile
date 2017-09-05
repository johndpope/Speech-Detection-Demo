include ../../Makefile.global

TARGET   = parallel_bw
LIBS     = -lmodinv \
	   -lcep_feat \
	   -lio \
	   -lmllr \
	   -ls2io \
	   -lcommon \
	   -lrpcc \
	   -lm
DEP_LIBS = $(LIBDIR)/libmodinv.a \
	   $(LIBDIR)/libio.a \
	   $(LIBDIR)/libmllr.a \
	   $(LIBDIR)/libs2io.a \
	   $(LIBDIR)/libcommon.a \
	   $(LIBDIR)/libcep_feat.a \
	   $(LIBDIR)/librpcc.a

DBG_TARGET   =	$(TARGET)_dbg
DBG_LIBS     =	-lmodinv_dbg \
		-lcep_feat_dbg \
		-lio_dbg \
		-lmllr_dbg \
		-ls2io_dbg \
		-lcommon_dbg \
		-lrpcc \
		-lm
DBG_DEP_LIBS =	$(LIBDIR)/libmodinv_dbg.a \
		$(LIBDIR)/libio_dbg.a \
		$(LIBDIR)/libmllr_dbg.a \
		$(LIBDIR)/libs2io_dbg.a \
		$(LIBDIR)/libcommon_dbg.a \
		$(LIBDIR)/libcep_feat_dbg.a \
		$(LIBDIR)/librpcc.a

OBJS  = accum.o \
	backward.o \
	baum_welch.o \
	viterbi.o \
	forward.o \
	main.o \
	next_utt_states.o \
	train_cmd_ln.o

SRCS  = accum.c \
	backward.c \
	baum_welch.c \
	viterbi.c \
	forward.c \
	main.c \
	next_utt_states.c \
	train_cmd_ln.c

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
