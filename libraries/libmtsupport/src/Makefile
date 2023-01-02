include ../Makefile.common

LIBRARY = lib$(NAME)
MAJOR = 0

SHAREDLIB = $(LIBRARY).so
SONAME = $(SHAREDLIB).$(MAJOR)
STATICLIB = $(LIBRARY).a

INSTALL = install

LIBDIR = $(libdir)
INCLUDEDIR = $(includedir)/mt/support

INCLUDES= -I../include/mt/support
CXXFLAGS=$(FLAGS) $(INCLUDES) $(RPM_OPT_FLAGS)
LDFLAGS = 
LINK.o = $(CXX) $(LDFLAGS) $(FLAGS) $(TARGET_ARCH)

LIB_SRC = str.cc path.cc

LIB_OBJ = $(LIB_SRC:%.cc=%.o)
LIB_SOBJ = $(LIB_SRC:%.cc=%.so)
LIB_DEP = $(LIB_SRC:%.cc=%.d)

TARGETS = $(SHAREDLIB) $(STATICLIB)

.PHONY: all install clean

all: $(TARGETS)

%.so: %.cc
	$(COMPILE.cpp) $(CPPFLAGS) -fpic $< $(OUTPUT_OPTION)

$(SHAREDLIB): $(LIB_SOBJ)
	$(LINK.o) -shared -Wl,-soname,$(SONAME) -lc $+ $(OUTPUT_OPTION)
	ln -sf $(SHAREDLIB) $(SONAME)

$(STATICLIB): $(LIB_OBJ)
	$(AR) $(ARFLAGS) $@ $+
	-ranlib $@

install: all
	$(INSTALL) -d -m755 $(INCLUDEDIR)
	@for i in ../include/mt/support/*.hh; do $(INSTALL) $$i -pm644 $(INCLUDEDIR) ;done
	$(INSTALL) -pD -m755 $(SHAREDLIB) $(LIBDIR)/$(SHAREDLIB).$(VERSION)
	$(INSTALL) -pD -m644 $(STATICLIB) $(LIBDIR)/$(STATICLIB)
	@ln -sf $(SHAREDLIB).$(VERSION) $(LIBDIR)/$(SONAME)
	@ln -sf $(SONAME) $(LIBDIR)/$(SHAREDLIB)

clean:
	$(RM) $(TARGETS) $(LIB_OBJ) $(LIB_SOBJ) $(LIB_DEP) $(SONAME) core *~ test

# We need dependencies only if goal isn't "clean"
ifneq ($(MAKECMDGOALS),clean)

%.d:	%.cc
	@echo Making dependences for $<
	@$(SHELL) -ec "$(CC) -MM $(CXXFLAGS) $< | sed 's|\($*\)\.o[ :]*|\1.o $@ : |g' > $@; [ -s $@ ] || $(RM) $@"

ifneq ($(LIB_DEP),)
-include $(LIB_DEP)
endif
endif
