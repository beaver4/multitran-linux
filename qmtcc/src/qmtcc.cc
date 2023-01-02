#include "qmtcc.hh"

qmtcc::qmtcc(int argc,char **argv):
    QApplication(argc,argv),
    xkb_(xkb::instance())
{}

bool qmtcc::x11EventFilter(XEvent *e)
{
    xkb_.processEvent(e);
    return QApplication::x11EventFilter(e);
}
