include ../../Makefile.global

TARGET = libs2io.a

DBG_TARGET = libs2io_dbg.a

OBJS  = \
	s2_read_map.o \
	s2_read_seno.o \
	s2_read_tmat.o \
	s2_read_cb.o \
	s2_write_cb.o \
	s2_write_seno.o \
	s2_write_hmm.o \
	s2_mixing_weights.o \
	areadfloat.o \
	awritefloat.o \
	areadshort.o \
	awriteshort.o \
	areadint.o \
	areadchar.o \
	awriteint.o \
	add-table.o \
	pset_io.o \
	int32_io.o

SRCS  = \
	s2_read_map.c \
	s2_read_seno.c \
	s2_read_tmat.c \
	s2_read_cb.c \
	s2_write_cb.c \
	s2_write_seno.c \
	s2_write_hmm.c \
	s2_mixing_weights.c \
	areadfloat.c \
	areadint.c \
	areadshort.c \
	awriteshort.c \
	areadchar.c \
	awritefloat.c \
	awriteint.c \
	add-table.c \
	pset_io.c \
	int32_io.c

INSTALLED_HEADERS = \
	$(PKG_ROOT)/include/s3/s2_mixing_weights.h \
	$(PKG_ROOT)/include/s3/s2_read_cb.h \
	$(PKG_ROOT)/include/s3/s2_read_map.h \
	$(PKG_ROOT)/include/s3/s2_read_seno.h \
	$(PKG_ROOT)/include/s3/s2_read_tmat.h \
	$(PKG_ROOT)/include/s3/s2_write_cb.h \
	$(PKG_ROOT)/include/s3/s2_write_hmm.h \
	$(PKG_ROOT)/include/s3/s2_write_seno.h \
	$(PKG_ROOT)/include/s3/int32_io.h \
	$(PKG_ROOT)/include/s3/s2io.h \
	$(PKG_ROOT)/include/s3/pset_io.h \
	$(PKG_ROOT)/include/s3/read_seno_dtree.h

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
$(PKG_ROOT)/include/s3/s2_mixing_weights.h: ../include/s3/s2_mixing_weights.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/s2_read_cb.h: ../include/s3/s2_read_cb.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/s2_read_map.h: ../include/s3/s2_read_map.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/s2_read_seno.h: ../include/s3/s2_read_seno.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/s2_read_tmat.h: ../include/s3/s2_read_tmat.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/s2_write_cb.h: ../include/s3/s2_write_cb.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/s2_write_hmm.h: ../include/s3/s2_write_hmm.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/s2_write_seno.h: ../include/s3/s2_write_seno.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/int32_io.h: ../include/s3/int32_io.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/s2io.h: ../include/s3/s2io.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/pset_io.h: ../include/s3/pset_io.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/read_seno_dtree.h: ../include/s3/read_seno_dtree.h
	rm -f $@
	cp -p $? $@


