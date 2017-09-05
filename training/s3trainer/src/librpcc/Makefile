include ../../Makefile.global

TARGET = librpcc.a

DBG_TARGET = librpcc_dbg.a

OBJS  = rpcc.o

SRCS  = rpcc.c

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
install:
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


