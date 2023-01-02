include Makefile.common

CVSROOT=:ext:ievlev@cvs.sourceforge.net:/cvsroot/multitran

INDENT_FILES=$(shell find . -name '*.cc' -o -name '*.hh')

.PHONY: all clean indent

SUBDIRS = src testsuite

all:
	@for i in $(SUBDIRS); do $(MAKE) -C $$i all || exit 1; done

clean:
	@for i in $(SUBDIRS); do $(MAKE) -C $$i clean || exit 1; done

install: all
	@for i in $(SUBDIRS); do $(MAKE) libdir=$(libdir) includedir=$(includedir) -C  $$i install || exit 1; done

indent:
	@(for i in $(INDENT_FILES) ;do \
	astyle --mode=c++ --style=ansi -N -T8 -Oo --pad=oper $$i;\
	done)
	@find . -type f -name '*.orig' -print0 | xargs -r0 rm -f

distrib:
	@cvs commit -m ' - Auto commit'
	@mkdir -p lib$(NAME)-$(VERSION)
	@CVSROOT=$(CVSROOT) cvs co lib$(NAME)
	@mv lib$(NAME)/* lib$(NAME)-$(VERSION)
	@./cvs2cl.pl -b --fsf -f lib$(NAME)-$(VERSION)/ChangeLog
	@rmdir lib$(NAME)
	@find lib$(NAME)-$(VERSION) -type d -name 'CVS'|xargs rm -rf
	@tar -cjpf lib$(NAME)-$(VERSION).tar.bz2 lib$(NAME)-$(VERSION)
	@rm -rf lib$(NAME)-$(VERSION)
