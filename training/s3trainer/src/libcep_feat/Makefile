include ../../Makefile.global

TARGET = libcep_feat.a
DBG_TARGET = libcep_feat_dbg.a

OBJS  = agc_emax.o \
	agc_max.o \
	r_agc_noise.o \
	agc.o \
	norm.o \
	live_norm.o \
	cmn.o \
	varnorm.o \
	silcomp.o \
	del_sil_seg.o \
	s2_cep.o \
	s2_dcep.o \
	s2_ddcep.o \
	s2_feat.o \
	v1_feat.o \
	v2_feat.o \
	v3_feat.o \
	v4_feat.o \
	v5_feat.o \
	v6_feat.o \
	v7_feat.o \
	v8_feat.o \
	cep_frame.o \
	dcep_frame.o \
	ddcep_frame.o \
	feat.o feat2.o

SRCS  = agc_emax.c \
	agc_max.c \
	r_agc_noise.c \
	agc.c \
	norm.c \
	live_norm.c \
	cmn.c \
	varnorm.c \
	silcomp.c \
	del_sil_seg.c \
	s2_cep.c \
	s2_dcep.c \
	s2_ddcep.c \
	s2_feat.c \
	v1_feat.c \
	v2_feat.c \
	v3_feat.c \
	v4_feat.c \
	v5_feat.c \
	v6_feat.c \
	v7_feat.c \
	v8_feat.c \
	cep_frame.c \
	dcep_frame.c \
	ddcep_frame.c \
	feat.c feat2.c

INSTALLED_HEADERS = \
	$(PKG_ROOT)/include/s3/live_norm.h \
	$(PKG_ROOT)/include/s3/norm.h \
	$(PKG_ROOT)/include/s3/cmn.h \
	$(PKG_ROOT)/include/s3/varnorm.h \
	$(PKG_ROOT)/include/s3/agc_emax.h \
	$(PKG_ROOT)/include/s3/agc_max.h \
	$(PKG_ROOT)/include/s3/r_agc_noise.h \
	$(PKG_ROOT)/include/s3/agc.h \
	$(PKG_ROOT)/include/s3/silcomp.h \
	$(PKG_ROOT)/include/s3/cep_manip.h \
	$(PKG_ROOT)/include/s3/feat.h

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
$(PKG_ROOT)/include/s3/live_norm.h: ../include/s3/live_norm.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/norm.h: ../include/s3/norm.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/cmn.h: ../include/s3/cmn.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/varnorm.h: ../include/s3/varnorm.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/agc_emax.h: ../include/s3/agc_emax.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/agc_max.h: ../include/s3/agc_max.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/r_agc_noise.h: ../include/s3/r_agc_noise.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/agc.h: ../include/s3/agc.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/silcomp.h: ../include/s3/silcomp.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/cep_manip.h: ../include/s3/cep_manip.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/feat.h: ../include/s3/feat.h
	rm -f $@
	cp -p $? $@
