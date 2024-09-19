#include <qtextstream.h>

#include "art_viewer.hh"
#include "codec.hh"

#include "file_map.hh"
#include "singleton.hh"
#include "path.hh"
#include "str.hh"
#include "lgk.hh"

typedef mt::singleton_array<mt::file_map> txtdb_type;

namespace
{
	QString spart(QTextCodec *codec,const mt::article& a)
	{
		mt::file_map& spart = txtdb_type::instance(mt::datapath+mt::path_separator()+"speechparts.txt");
		return QString("<td>")+
			"<b>"+conv(codec,a.orig())+"</b>,"+
			"<font color=\"blue\">"+spart.any_name(mt::to_string<int>(mt::speech_part(a.lgk())))+"</font>"+
			"</td>\n";
	}

	QString subj(const mt::article& a)
	{
		mt::file_map& subj = txtdb_type::instance(mt::datapath+mt::path_separator()+"subjects.txt");
		return QString("<td><font color=\"red\">")+subj.any_name(a.subject())+"</font></td>\n";
	}

	int compare_articles(const mt::article& a1,const mt::article& a2)
	{
	    if (a1.lgk() != a2.lgk())
		return a2.lgk() > a1.lgk();
	    else
		return a2.subject() > a1.subject();
	}
	
}

art_viewer::art_viewer(QWidget *parent):
	QTextBrowser(parent)
{
}



void art_viewer::clear()
{
	setText("no translation are available");
}

void art_viewer::set(mt::article_set as)
{
    QString result;
    QTextOStream os(&result);

    QTextCodec *in_c = QTextCodec::codecForName(codeset(as.langin_));
    QTextCodec *out_c = QTextCodec::codecForName(codeset(as.langout_));

    if (as.articles_.empty())
    {
    	setText("<b>translation for this word not found</b>");
    }
    else
    {
	os<<"<table border=\"0\" width=\"100%\">\n";
        os<<"<tr><td><b>original</b></td><td><b>subject</b></td><td><b>translation</b>";

	std::sort(as.articles_.begin(),as.articles_.end(),compare_articles);
    
	int prev_lgk = -1;
	std::string prev_subject = "x";
	for(size_t i=0;i<as.articles_.size();++i)
	{
	    const mt::article& a = as.articles_[i];
	    if (prev_lgk != a.lgk())
	    {
	    	os<<"</td>\n</tr>\n";//close previous last coll and row
		os<<"<tr>\n";//open new row
	
		os<<spart(in_c,a);//first coll: speech part
		os<<subj(a);//second coll: subject
		
		//open new third call
		os<<"<td>"<<conv(out_c,a.translated());
		
		prev_lgk = a.lgk();
		prev_subject = a.subject();
	    }
	    else if (prev_subject != a.subject())
	    {
	    	os<<"</td>\n</tr>\n";//close previous last coll and row
		os<<"<tr>\n";//open new row

		os<<"<td></td>\n";//first empty coll
		os<<subj(a);//second coll: subject

		//open new third call
		os<<"<td>"<<conv(out_c,a.translated());
		
		prev_subject = a.subject();
	    }
	    else
	    	os<<", "<<conv(out_c,a.translated());//append to third call
	}
	 os<<"</td>\n</tr>\n</table>\n";
    }
    setText(result);
}

