include ../Makefile.common
LDFLAGS = -Wl,-rpath,$(shell pwd)/../src/ -L../src/
LDLIBS = -lbtree
CXXFLAGS = $(FLAGS) -I../include/btree

TESTS = test1 test2 test3 test4

TARGETS = $(TESTS)

all: $(TARGETS)

install:

clean:
	rm -rf $(TARGETS) *.cdb

