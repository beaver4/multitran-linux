#ifndef QMTCC_WORDSELECTOR_HH__
#define QMTCC_WORDSELECTOR_HH__

#include <qtextbrowser.h>

#include "translation.hh"

class word_selector: public QTextBrowser
{
Q_OBJECT
	public:
		word_selector(QWidget *parent = 0);
		void set(mt::artset_vector& asets);
	signals:
		void go(int);
	protected slots:
		void emit_clicked(const QString&);
	private:
		QString text_;
};

#endif

