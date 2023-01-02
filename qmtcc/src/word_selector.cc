#include "word_selector.hh"
#include "codec.hh"

#include <qmap.h>

namespace
{

	struct append_form
	{
		typedef QMap<QString,bool> map_type;

		append_form(QTextCodec *codec): codec_(codec) {}

		void operator()(const mt::article& a)
		{
			forms_[conv(codec_,a.orig())]=true;
		}
		operator QString() const
		{
			QString result;
		
			for(map_type::const_iterator it = forms_.begin(); it!=forms_.end();++it)
				result +=it.key()+", ";

			result.truncate(result.length()-2);
			if (forms_.size() > 1)
				return "("+result+")";
			else
				return result;
		}
		QMap<QString,bool> forms_;
		QTextCodec *codec_;
	};
	
	struct append_article
	{
		append_article(QTextCodec *codec):count_(0),codec_(codec) {}
		void operator()(mt::article_set as)
		{
			QString real_name;
			if (as.articles_.empty())
				str_+=" "+conv(codec_,as.untranslated_);
			else
			{
				//collect all possible forms
				QString forms = std::for_each(as.articles_.begin(),
						              as.articles_.end(),append_form(codec_));
				str_+="  <a href=\"";
				str_+= QString("%1").arg(count_);
				str_+="\">"+forms+"</a>";
			}
			++count_;
		}
		operator QString() const { return str_; }
		QString str_;
		int count_;
		QTextCodec *codec_;
	};
}

word_selector::word_selector(QWidget *parent):
	QTextBrowser(parent)
{
	connect(this,SIGNAL(linkClicked(const QString&)),SLOT(emit_clicked(const QString&)));
	setMaximumHeight(30);
}
void word_selector::set(mt::artset_vector& asets)
{
	if (asets.empty()) return;

	QTextCodec *in_c = QTextCodec::codecForName(codeset(asets[0].langin_));
	text_ = std::for_each(asets.begin(),asets.end(),append_article(in_c));
	setText(text_);
}

void word_selector::emit_clicked(const QString& link)
{
	emit go(link.toInt());
	setText(text_);
}
