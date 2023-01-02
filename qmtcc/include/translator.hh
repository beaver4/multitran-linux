#ifndef QMTCC_TRANSLATOR_HH__
#define QMTCC_TRANSLATOR_HH__

#include <qmainwindow.h>
#include <qlineedit.h>

#include "langpair_selector.hh"
#include "result_viewer.hh"

#include "translation.hh"
#include "shared_ptr.hh"

/**
 * main application window
 */
class translator:public QMainWindow
{
	typedef mt::shared_ptr<mt::translation> trans_ptr;
Q_OBJECT
	public:
		translator(QWidget *parent = 0);
	protected slots:
		void do_translate();
	private:
		QLineEdit	  *input_;
		langpair_selector *selector_;
		result_viewer	  *result_;
		trans_ptr translation_;
};

#endif
