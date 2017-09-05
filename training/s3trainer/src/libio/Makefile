include ../../Makefile.global

TARGET = libio.a

DBG_TARGET = libio_dbg.a

OBJS  = \
	read_line.o \
	model_def_io.o \
	swap.o \
	topo_read.o \
	fread_retry.o \
	corpus.o \
	share_ctl.o \
	s3_open.o \
	segdmp.o \
	s3io.o \
	s3mixw_io.o \
	s3tmat_io.o \
	s3gau_io.o \
	s3map_io.o \
	s3ts2cb_io.o \
	s3cb2mllr_io.o \
	s3lamb_io.o \
	s3regmat_io.o \
	fgets_wo_nl.o \
	uttfile.o \
	fp_cache.o

SRCS  = \
	read_line.c \
	model_def_io.c \
	swap.c \
	topo_read.c \
	fread_retry.c \
	corpus.c \
	share_ctl.c \
	s3_open.c \
	segdmp.c \
	s3io.c \
	s3mixw_io.c \
	s3tmat_io.c \
	s3gau_io.c \
	s3map_io.c \
	s3ts2cb_io.c \
	s3cb2mllr_io.c \
	s3lamb_io.c \
	s3regmat_io.c \
	fgets_wo_nl.c \
	uttfile.c \
	fp_cache.c

INSTALLED_HEADERS = \
	$(PKG_ROOT)/include/s3/array_io.h \
	$(PKG_ROOT)/include/s3/bcomment_io.h \
	$(PKG_ROOT)/include/s3/corpus.h \
	$(PKG_ROOT)/include/s3/floatbuf_io.h \
	$(PKG_ROOT)/include/s3/intbuf_io.h \
	$(PKG_ROOT)/include/s3/fread_retry.h \
	$(PKG_ROOT)/include/s3/gauden_io.h \
	$(PKG_ROOT)/include/s3/mixw_param_io.h \
	$(PKG_ROOT)/include/s3/model_def.h \
	$(PKG_ROOT)/include/s3/model_def_io.h \
	$(PKG_ROOT)/include/s3/read_line.h \
	$(PKG_ROOT)/include/s3/swap.h \
	$(PKG_ROOT)/include/s3/tmat_param_io.h \
	$(PKG_ROOT)/include/s3/topo_read.h \
	$(PKG_ROOT)/include/s3/bo_magic.h \
	$(PKG_ROOT)/include/s3/s3_open.h \
	$(PKG_ROOT)/include/s3/state_param_def_io.h \
	$(PKG_ROOT)/include/s3/segdmp.h \
	$(PKG_ROOT)/include/s3/lambda_io.h \
	$(PKG_ROOT)/include/s3/regmat_io.h \
	$(PKG_ROOT)/include/s3/mllr_class_io.h \
	$(PKG_ROOT)/include/s3/s3io.h \
	$(PKG_ROOT)/include/s3/s3mixw_io.h \
	$(PKG_ROOT)/include/s3/s3tmat_io.h \
	$(PKG_ROOT)/include/s3/s3gau_io.h \
	$(PKG_ROOT)/include/s3/s3map_io.h \
	$(PKG_ROOT)/include/s3/s3ts2cb_io.h \
	$(PKG_ROOT)/include/s3/s3cb2mllr_io.h \
	$(PKG_ROOT)/include/s3/s3lamb_io.h \
	$(PKG_ROOT)/include/s3/s3regmat_io.h \
	$(PKG_ROOT)/include/s3/fgets_wo_nl.h \
	$(PKG_ROOT)/include/s3/uttfile.h \
	$(PKG_ROOT)/include/s3/fp_cache.h

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

#
# Copy header files to installed location if necessary
#

$(PKG_ROOT)/include/s3/array_io.h: ../include/s3/array_io.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/bcomment_io.h: ../include/s3/bcomment_io.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/corpus.h: ../include/s3/corpus.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/floatbuf_io.h: ../include/s3/floatbuf_io.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/intbuf_io.h: ../include/s3/intbuf_io.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/fread_retry.h: ../include/s3/fread_retry.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/gauden_io.h: ../include/s3/gauden_io.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/mixw_param_io.h: ../include/s3/mixw_param_io.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/model_def.h: ../include/s3/model_def.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/model_def_io.h: ../include/s3/model_def_io.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/read_line.h: ../include/s3/read_line.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/swap.h: ../include/s3/swap.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/tmat_param_io.h: ../include/s3/tmat_param_io.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/topo_read.h: ../include/s3/topo_read.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/bo_magic.h: ../include/s3/bo_magic.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/s3_open.h: ../include/s3/s3_open.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/state_param_def_io.h: ../include/s3/state_param_def_io.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/segdmp.h: ../include/s3/segdmp.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/lambda_io.h: ../include/s3/lambda_io.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/regmat_io.h: ../include/s3/regmat_io.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/mllr_class_io.h: ../include/s3/mllr_class_io.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/s3io.h: ../include/s3/s3io.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/s3mixw_io.h: ../include/s3/s3mixw_io.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/s3tmat_io.h: ../include/s3/s3tmat_io.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/s3gau_io.h: ../include/s3/s3gau_io.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/s3map_io.h: ../include/s3/s3map_io.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/s3ts2cb_io.h: ../include/s3/s3ts2cb_io.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/s3cb2mllr_io.h: ../include/s3/s3cb2mllr_io.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/s3lamb_io.h: ../include/s3/s3lamb_io.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/s3regmat_io.h: ../include/s3/s3regmat_io.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/fgets_wo_nl.h: ../include/s3/fgets_wo_nl.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/uttfile.h: ../include/s3/uttfile.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/fp_cache.h: ../include/s3/fp_cache.h
	rm -f $@
	cp -p $? $@
