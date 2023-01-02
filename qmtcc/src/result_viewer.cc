#include "result_viewer.hh"

result_viewer::result_viewer(QWidget *parent):
	QWidget(parent),
	box_( this ),
	words_(this),
	article_(this),
	curr_asets_(0)
{
	box_.addWidget(&words_);
	box_.addWidget(&article_);

	words_.setText("words");
	article_.setText("article");
	connect(&words_,SIGNAL(go(int)),SLOT(change(int)));
}

void result_viewer::set(mt::artset_vector* asets)
{
	curr_asets_ = asets;

	words_.set(*curr_asets_);
	if (curr_asets_->empty())
		article_.setText("nothing to translate...");
	else
		article_.set(curr_asets_->at(0));
}

void result_viewer::change(int num)
{
	if (!curr_asets_ || curr_asets_->size() < num) return;
	article_.set(curr_asets_->at(num));
}
