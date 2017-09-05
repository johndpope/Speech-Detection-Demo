include ../../Makefile.global

TARGET = libmllr.a

DBG_TARGET = libmllr_dbg.a

OBJS  = \
	mllr.o \
	mllr_io.o

SRCS  = \
	mllr.c \
	mllr_io.c

INSTALLED_HEADERS = \
	$(PKG_ROOT)/include/s3/mllr.h \
	$(PKG_ROOT)/include/s3/mllr_io.h

#
# Rules updated 3-Jul-96 (eht)
#

default:
	(cd $(MACHINE); $(MAKE) ${$(MACHINE)_MFLAGS} -f ../Makefile VPATH=.. $(TARGET))

debug:
	(cd $(MACHINE)_dbg; $(MAKE) ${$(MACHINE)_MFLAGS} ADDL_CFLAGS=-g -f ../Makefile VPATH=.. $(DBG_TARGET))

clean:
	(cd $(MACHINE); $(RM) -f *.a $(OBJS))

# perform installation if necessary
install:	install.hdr
	( cd $(MACHINE); $(MAKE) ${$(MACHINE)_MFLAGS} -f ../Makefile VPATH=.. $(LIBDIR)/$(TARGET) )

# Do the stupid sed hack to convert paths starting with ./ to ones with ../
depend:
	mkdir -p $(MACHINE)
	mkdir -p $(MACHINE)_dbg
	makedepend -f Makefile.depend.$(MACHINE) $(DEPEND_HPATHS) $(SRCS)
	sed -e 's+\./+\.\./+g' Makefile.depend.$(MACHINE) > tmp.$(MACHINE)
	mv tmp.$(MACHINE) Makefile.depend.$(MACHINE)

include Makefile.depend.$(MACHINE)

$(TARGET): $(OBJS)
	$(AR) crv $@ $?
	$(RANLIB) $@

$(DBG_TARGET): $(OBJS)
	$(AR) crv $@ $?
	$(RANLIB) $@
	cp -p $@ $(LIBDIR)

# if installing library, rebuild if out of date
$(LIBDIR)/$(TARGET): $(TARGET)
	cp -p $(TARGET) $(PKG_ROOT)/lib.$(MACHINE)

install.hdr:
	( cd $(MACHINE); $(MAKE) ${$(MACHINE)_MFLAGS} -f ../Makefile VPATH=.. $(INSTALLED_HEADERS) )

#
# Update installed header files if necessary
#

$(PKG_ROOT)/include/s3/mllr.h: ../include/s3/mllr.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/mllr_io.h: ../include/s3/mllr_io.h
	rm -f $@
	cp -p $? $@

# DO NOT DELETE
