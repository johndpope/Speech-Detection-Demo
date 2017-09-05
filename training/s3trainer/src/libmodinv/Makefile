include ../../Makefile.global

TARGET = libmodinv.a

DBG_TARGET = libmodinv_dbg.a

OBJS   = mod_inv.o \
	 gauden.o

SRCS   = mod_inv.c \
	 gauden.c

INSTALLED_HEADERS = \
	$(PKG_ROOT)/include/s3/model_inventory.h \
	$(PKG_ROOT)/include/s3/gauden.h

#
# Rules updated 3-Jul-96 (eht)
#

default:
	(cd $(MACHINE); $(MAKE) ${$(MACHINE)_MFLAGS} -f ../Makefile VPATH=.. $(TARGET))

debug:
	(cd $(MACHINE)_dbg; $(MAKE) ADDL_CFLAGS=-g ${$(MACHINE)_MFLAGS} -f ../Makefile VPATH=.. $(DBG_TARGET))

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

# check installed headers and copy if out of date
$(PKG_ROOT)/include/s3/gauden.h: ../include/s3/gauden.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/model_inventory.h: ../include/s3/model_inventory.h
	rm -f $@
	cp -p $? $@

# DO NOT DELETE
