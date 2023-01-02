include ../Makefile.common
LDFLAGS = -Wl,-rpath,$(shell pwd)/../src/ -Wl,-rpath,$(shell pwd)/../../libmtsupport/src -L../src/ -L../../libmtsupport/src/
LDLIBS =  -lfacet
CXXFLAGS = $(FLAGS) -I../include/facet

TESTS = test1

TARGETS = $(TESTS)

all: $(TARGETS)

install:

clean:
	rm -rf $(TARGETS)

