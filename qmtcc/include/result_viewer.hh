#ifndef QMTCC_RESULTVIEWER_HH__
#define QMTCC_RESULTVIEWER_HH__

#include <qwidget.h>
#include <qlayout.h>

#include "art_viewer.hh"
#include "word_selector.hh"

class result_viewer: public QWidget
{
Q_OBJECT
	public:
		result_viewer(QWidget *parent = 0);
		void set(mt::artset_vector *asets);
	protected slots:
		void change(int);
	private:
		QVBoxLayout	box_;
		word_selector   words_;
		art_viewer	article_;
		mt::artset_vector *curr_asets_;//current translation
};



#endif

