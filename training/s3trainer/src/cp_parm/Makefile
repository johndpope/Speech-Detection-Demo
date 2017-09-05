include ../../Makefile.global

TARGET   = cp_parm
LIBS     = \
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

DBG_TARGET   =	$(TARGET)_dbg
DBG_LIBS     = \
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

OBJS  = main.o \
	cmd_ln.o

SRCS  = main.c \
	cmd_ln.c

default:
	(cd $(MACHINE); $(MAKE) ${$(MACHINE)_MFLAGS} -f ../Makefile VPATH=.. $(TARGET))

debug:	clean
	(cd $(MACHINE); $(MAKE) ADDL_CFLAGS=-g ${$(MACHINE)_MFLAGS} -f ../Makefile VPATH=.. $(DBG_TARGET))

clean:
	(cd $(MACHINE); rm -f $(OBJS))

relink:
	(cd $(MACHINE); rm -f $(TARGET))

# Do some sed munging to convert paths starting with ./ to ones with ../
depend:
	mkdir -p $(MACHINE)
	mkdir -p $(MACHINE)_dbg
	cat /dev/null > Makefile.depend.$(MACHINE); makedepend -f Makefile.depend.$(MACHINE) $(DEPEND_HPATHS) $(SRCS)
	sed -e 's+\./+\.\./+g' Makefile.depend.$(MACHINE) > tmp.$(MACHINE)
	mv tmp.$(MACHINE) Makefile.depend.$(MACHINE)

include Makefile.depend.$(MACHINE)

$(TARGET): $(OBJS) $(DEP_LIBS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS) $(LIBS)

$(DBG_TARGET): $(OBJS) $(DBG_DEP_LIBS)
	$(CC) $(CFLAGS) -o $(DBG_TARGET) $(OBJS) $(LDFLAGS) $(DBG_LIBS)
	cp -p $(DBG_TARGET) $(PKG_ROOT)/bin.$(MACHINE)

# perform installation if necessary
install:
	(cd $(MACHINE); $(MAKE) ${$(MACHINE)_MFLAGS} -f ../Makefile VPATH=.. install.mdep)

$(PKG_ROOT)/bin.$(MACHINE)/$(TARGET): $(TARGET)
	-mv -f $(PKG_ROOT)/bin.$(MACHINE)/$(TARGET) $(PKG_ROOT)/bin.$(MACHINE)/$(TARGET).old
	-rm $(PKG_ROOT)/bin.$(MACHINE)/$(TARGET).old
	cp -p $(TARGET) $(PKG_ROOT)/bin.$(MACHINE)

install.mdep: $(PKG_ROOT)/bin.$(MACHINE)/$(TARGET)
