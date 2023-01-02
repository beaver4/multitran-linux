include ../Makefile.common
LDFLAGS = -Wl,-rpath,$(shell pwd)/../src/ -L../src/
LDLIBS = -lmtsupport
CXXFLAGS = $(FLAGS) -I../include/mt/support

TESTS = test1

TARGETS = $(TESTS)

all: $(TARGETS)

install:

clean:
	rm -rf $(TARGETS)

