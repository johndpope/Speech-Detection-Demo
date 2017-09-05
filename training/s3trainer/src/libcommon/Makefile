include ../../Makefile.global

TARGET = libcommon.a

DBG_TARGET = libcommon_dbg.a

OBJS  = \
	heap.o \
	enum_subset.o \
	cmd_ln.o \
	ckd_alloc.o \
	prefix_upto.o \
	vector.o \
	hash.o \
	n_words.o \
	mk_wordlist.o \
	remap.o \
	lexicon.o \
	itree.o \
	acmod_set.o \
	err.o \
	prefetch.o \
	matrix.o \
	get_time.o \
	get_cpu_time.o \
	profile.o \
	mk_phone_list.o \
	cvt2triphone.o \
	was_added.o \
	ck_seg.o \
	mk_sseq.o \
	mk_trans_seq.o \
	quest.o \
	btree.o \
	mk_ts2ci.o \
	state_seq.o \
	get_host_name.o \
	mk_phone_seq.o \
	ts2cb.o \
	dtree.o \
	best_q.o \
	two_class.o

SRCS  = \
	heap.c \
	enum_subset.c \
	cmd_ln.c \
	ckd_alloc.c \
	prefix_upto.c \
	vector.c \
	hash.c \
	n_words.c \
	mk_wordlist.c \
	remap.c \
	lexicon.c \
	itree.c \
	acmod_set.c \
	err.c \
	prefetch.c \
	matrix.c \
	get_time.c \
	get_cpu_time.c \
	profile.c \
	mk_phone_list.c \
	cvt2triphone.c \
	was_added.c \
	ck_seg.c \
	mk_sseq.c \
	mk_trans_seq.c \
	quest.c \
	btree.c \
	mk_ts2ci.c \
	state_seq.c \
	get_host_name.c \
	mk_phone_seq.c \
	ts2cb.c \
	dtree.c \
	best_q.c \
	two_class.c

INSTALLED_HEADERS = \
	$(PKG_ROOT)/include/s3/heap.h \
	$(PKG_ROOT)/include/s3/enum_subset.h \
	$(PKG_ROOT)/include/s3/cmd_ln.h \
	$(PKG_ROOT)/include/s3/ckd_alloc.h \
	$(PKG_ROOT)/include/s3/prefix_upto.h \
	$(PKG_ROOT)/include/s3/vector.h \
	$(PKG_ROOT)/include/s3/hash.h \
	$(PKG_ROOT)/include/s3/n_words.h \
	$(PKG_ROOT)/include/s3/mk_wordlist.h \
	$(PKG_ROOT)/include/s3/remap.h \
	$(PKG_ROOT)/include/s3/acmod_set.h \
	$(PKG_ROOT)/include/s3/acmod_set_ds.h \
	$(PKG_ROOT)/include/s3/lexicon.h \
	$(PKG_ROOT)/include/s3/common.h \
	$(PKG_ROOT)/include/s3/itree.h \
	$(PKG_ROOT)/include/s3/err.h \
	$(PKG_ROOT)/include/s3/prefetch.h \
	$(PKG_ROOT)/include/s3/matrix.h \
	$(PKG_ROOT)/include/s3/get_time.h \
	$(PKG_ROOT)/include/s3/get_cpu_time.h \
	$(PKG_ROOT)/include/s3/profile.h \
	$(PKG_ROOT)/include/s3/mk_phone_list.h \
	$(PKG_ROOT)/include/s3/cvt2triphone.h \
	$(PKG_ROOT)/include/s3/was_added.h \
	$(PKG_ROOT)/include/s3/ck_seg.h \
	$(PKG_ROOT)/include/s3/mk_sseq.h \
	$(PKG_ROOT)/include/s3/mk_trans_seq.h \
	$(PKG_ROOT)/include/s3/quest.h \
	$(PKG_ROOT)/include/s3/btree.h \
	$(PKG_ROOT)/include/s3/mk_ts2ci.h \
	$(PKG_ROOT)/include/s3/state_seq.h \
	$(PKG_ROOT)/include/s3/get_host_name.h \
	$(PKG_ROOT)/include/s3/mk_phone_seq.h \
	$(PKG_ROOT)/include/s3/ts2cb.h \
	$(PKG_ROOT)/include/s3/dtree.h \
	$(PKG_ROOT)/include/s3/best_q.h \
	$(PKG_ROOT)/include/s3/two_class.h \
	$(PKG_ROOT)/include/s3/state.h \
	$(PKG_ROOT)/include/s3/prim_type.h \
	$(PKG_ROOT)/include/s3/s2_param.h \
	$(PKG_ROOT)/include/s3/s3.h

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
# Update installed header files if necessary
#
$(PKG_ROOT)/include/s3/heap.h: ../include/s3/heap.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/enum_subset.h: ../include/s3/enum_subset.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/cmd_ln.h: ../include/s3/cmd_ln.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/ckd_alloc.h: ../include/s3/ckd_alloc.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/prefix_upto.h: ../include/s3/prefix_upto.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/vector.h: ../include/s3/vector.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/hash.h: ../include/s3/hash.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/n_words.h: ../include/s3/n_words.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/mk_wordlist.h: ../include/s3/mk_wordlist.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/remap.h: ../include/s3/remap.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/acmod_set.h: ../include/s3/acmod_set.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/acmod_set_ds.h: ../include/s3/acmod_set_ds.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/lexicon.h: ../include/s3/lexicon.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/common.h: ../include/s3/common.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/itree.h: ../include/s3/itree.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/err.h: ../include/s3/err.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/prefetch.h: ../include/s3/prefetch.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/matrix.h: ../include/s3/matrix.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/get_time.h: ../include/s3/get_time.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/get_cpu_time.h: ../include/s3/get_cpu_time.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/profile.h: ../include/s3/profile.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/mk_phone_list.h: ../include/s3/mk_phone_list.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/cvt2triphone.h: ../include/s3/cvt2triphone.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/was_added.h: ../include/s3/was_added.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/ck_seg.h: ../include/s3/ck_seg.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/mk_sseq.h: ../include/s3/mk_sseq.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/mk_trans_seq.h: ../include/s3/mk_trans_seq.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/quest.h: ../include/s3/quest.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/btree.h: ../include/s3/btree.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/mk_ts2ci.h: ../include/s3/mk_ts2ci.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/state_seq.h: ../include/s3/state_seq.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/get_host_name.h: ../include/s3/get_host_name.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/mk_phone_seq.h: ../include/s3/mk_phone_seq.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/ts2cb.h: ../include/s3/ts2cb.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/dtree.h: ../include/s3/dtree.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/best_q.h: ../include/s3/best_q.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/two_class.h: ../include/s3/two_class.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/state.h: ../include/s3/state.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/prim_type.h: ../include/s3/prim_type.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/s3.h: ../include/s3/s3.h
	rm -f $@
	cp -p $? $@
$(PKG_ROOT)/include/s3/s2_param.h: ../include/s3/s2_param.h
	rm -f $@
	cp -p $? $@
