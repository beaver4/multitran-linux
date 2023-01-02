include ../Makefile.common
LDFLAGS = -Wl,-rpath,$(shell pwd)/../src/ -L../src/
LDLIBS = -lmtquery -lbtree
DEBUG_INCLUDES= -I../../libmtsupport/include/mt/support -I../../libbtree/include/btree -I../../libfacet/include/facet
CXXFLAGS = $(FLAGS) $(DEBUG_INCLUDES) -I../include/mt/query -I/usr/include/btree -I/usr/include/mt/support -I/usr/include/facet

TESTS = test1 test2 test3 test4 test5 test6

TARGETS = $(TESTS)

all: $(TARGETS)

install:

clean:
	rm -rf $(TARGETS)

