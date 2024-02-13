include ../Makefile.common

LIBRARY = lib$(NAME)
MAJOR = 0

SHAREDLIB = $(LIBRARY).so
SONAME = $(SHAREDLIB).$(MAJOR)
STATICLIB = $(LIBRARY).a

INSTALL = install

LIBDIR = $(libdir)
INCLUDEDIR = $(includedir)/facet

INCLUDES = -I../../libmtsupport/include/mt/support -I/usr/include/mt/support -I../include/facet
CXXFLAGS =$(FLAGS) $(INCLUDES) $(RPM_OPT_FLAGS)
DEBUG_LDFLAGS=-L$(shell pwd)/../../libmtsupport/src -Wl,-rpath,$(shell pwd)/../../libmtsupport/src
LDLIBS = -lmtsupport
LINK.o = $(CXX) $(LDFLAGS) $(FLAGS) $(TARGET_ARCH)

LIB_SRC = alphabet.cc identification.cc typographic.cc

LIB_OBJ = $(LIB_SRC:%.cc=%.o)
LIB_SOBJ = $(LIB_SRC:%.cc=%.so)
LIB_DEP = $(LIB_SRC:%.cc=%.d)

CXXFLAGS += -fPIC

TARGETS = $(SHAREDLIB) $(STATICLIB)

.PHONY: all install clean

all: $(TARGETS)

%.so: %.cc
	$(COMPILE.cpp) $(CPPFLAGS) -fPIC $< $(OUTPUT_OPTION)

$(SHAREDLIB): $(LIB_SOBJ)
	$(LINK.o) $(DEBUG_LDFLAGS) -shared -Wl,-soname,$(SONAME) $+ $(LDLIBS) $(OUTPUT_OPTION)
	ln -sf $(SHAREDLIB) $(SONAME)

$(STATICLIB): $(LIB_OBJ)
	$(AR) $(ARFLAGS) $@ $+
	-ranlib $@

install: all
	$(INSTALL) -d -m755 $(INCLUDEDIR)
	@for i in ../include/facet/*.hh; do $(INSTALL) $$i -pm644 $(INCLUDEDIR) ;done
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
