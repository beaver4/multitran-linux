#ifndef QMTCC_HH__
#define QMTCC_HH__

#include <qapplication.h>
#include "xkeyboard.hh"

class qmtcc: public QApplication
{
	public:
		qmtcc(int argc,char **argv);
	protected:	
		bool x11EventFilter(XEvent *e);
		xkeyboard &xkb_;
};

#endif
