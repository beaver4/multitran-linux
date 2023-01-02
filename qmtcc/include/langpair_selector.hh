#ifndef QMTCC_LANG_PAIR_SELECTOR_HH__
#define QMTCC_LANG_PAIR_SELECTOR_HH__

#include <qwidget.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qmap.h>

#include "lang_info.hh"

/**
 * widget to select a translation direction: from lang --> to lang
 */
class langpair_selector: public QWidget
{
Q_OBJECT
	public: 
		langpair_selector(QWidget *parent=0);
		mt::lang_code langin() const;
		mt::lang_code langout() const;
	protected slots:
		void on_group_change(int);
		void on_from_change(int);
		void on_to_change(int);
	private:
		void correct_pair(int,int);
		void assign_pair(int,int);

	QHBoxLayout		layout_;
	QComboBox		from_,to_;
	QMap<QString,int>	names_; /**< mapping: xkb name --> lang num */
	QMap<QString,int>	nums_; /**< mapping: lang name --> lang num */
};

#endif
