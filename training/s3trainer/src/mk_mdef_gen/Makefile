include ../../Makefile.global

TARGET = mk_mdef_gen
LIBS   = -lio -ls2io -lcommon -lm
DEP_LIBS = $(LIBDIR)/libcommon.a \
	   $(LIBDIR)/libs2io.a \
	   $(LIBDIR)/libio.a

DBG_TARGET = $(TARGET)_dbg
DBG_LIBS   = -lio_dbg -ls2io_dbg -lcommon_dbg -lm
DBG_DEP_LIBS = $(LIBDIR)/libcommon_dbg.a \
	   $(LIBDIR)/libs2io_dbg.a \
	   $(LIBDIR)/libio_dbg.a

OBJS  = mk_mdef_gen.o hash.o heap.o parse_cmd_ln.o main.o
SRCS  = mk_mdef_gen.c hash.c heap.c parse_cmd_ln.c main.c

#SRCS  = count_triphones.c make_ci_heap.c make_ci_heap_cd_hash.c \
#        find_threshold.c make_dict_triphone_list.c hash.c make_mdef.c \
#        heap.c parse_cmd_ln.c main.c make_cd_heap.c read_dict.c
#
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
