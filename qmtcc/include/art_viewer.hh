#ifndef QMTCC_ARTVIEWER_HH__
#define QMTCC_ARTVIEWER_HH__

#include <qtextbrowser.h>

#include "translation.hh"

class art_viewer: public QTextBrowser
{
Q_OBJECT
	public:
		art_viewer(QWidget *parent = 0);
		void set(mt::article_set aset);
		void clear();
	private:
		QString text_;
};



#endif

