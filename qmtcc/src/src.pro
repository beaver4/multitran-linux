TEMPLATE = app
LANGUAGE = C++
TARGET=qmtcc

CONFIG	+= qt warn_on release
unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

target.path = /usr/bin
INSTALLS += target

DEPENDPATH	= ../include
INCPATH += ../include /usr/include/mt/query /usr/include/mt/support /usr/include/facet /usr/include/btree
LIBS += -lmtquery


HEADERS	= xkeyboard.hh langpair_selector.hh qmtcc.hh single.hh word_selector.hh \
art_viewer.hh codec.hh translator.hh result_viewer.hh
SOURCES	+= main.cc langpair_selector.cc qmtcc.cc xkeyboard.cc word_selector.cc \
art_viewer.cc codec.cc translator.cc result_viewer.cc
#FORMS	 = options.ui
