#include <algorithm>
#include <functional>
#include <memory>

#include "translator.hh"
#include "langpair_selector.hh"

#include "codec.hh"

#include <qtextedit.h>
#include <qstatusbar.h>


translator::translator(QWidget *parent):
	QMainWindow(parent,"translator")
{
    
	QToolBar * tr_tools = new QToolBar( this, "translation tools" );
	tr_tools->setLabel( "Translation Tools" );

	input_ = new QLineEdit(tr_tools);
	input_->setMinimumWidth(200);

	selector_ = new langpair_selector(tr_tools);

	result_ =new result_viewer(this);
    	result_->setFocus();
    	setCentralWidget( result_ );


    	statusBar()->message( "Ready", 2000 );

	resize(400,300);
	
/*
	mt::phrase ph;
    	ph.push_back("good");
    	ph.push_back("morning");
    	ph.push_back("moscow");
    	ph.push_back("listed");
    	ph.push_back("qwerqwerqwer");

	translation_ = new mt::translation(ph,mt::english,mt::russian);

	result_->set(&(translation_->asets()));
*/
	connect(input_,SIGNAL(returnPressed()),SLOT(do_translate()));
}


void translator::do_translate()
{
	mt::lang_code from = selector_->langin();
	mt::lang_code to = selector_->langout();
	QTextCodec *in_c = QTextCodec::codecForName(codeset(from));
	mt::phrase ph;
	mt::fill_phrase(ph,in_c->fromUnicode(input_->text()).lower().data(),from);
	
	translation_ = new mt::translation(ph,from,to);

	result_->set(&(translation_->asets()));
}
