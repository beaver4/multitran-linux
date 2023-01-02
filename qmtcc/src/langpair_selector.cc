#include "langpair_selector.hh"
#include "linguas.hh"
#include "xkeyboard.hh"

namespace
{
	struct collect_langs
    	{
		collect_langs(QMap<QString,int>& total): total_(total) {}
		void operator()(const mt::lang_pair& it)
		{
	    		const QString lfrom = mt::lang_name(it.first).c_str();
	    		const QString lto = mt::lang_name(it.second).c_str();

	    		from_[lfrom] = it.first;
	    		to_[lto] = it.second;

	    		total_[lfrom] = it.first;
	    		total_[lto] = it.second;
		}
	
		QMap<QString,int> from_,to_;
		QMap<QString,int>& total_;
    	};

    	struct compare_names
    	{
		compare_names(int from,int to):
	    		from_(from),
	    		to_(to)
		{}
		bool operator()(const mt::lang_pair& lng1,const mt::lang_pair& lng2)
		{
	    		return distance(lng1) < distance(lng2);
		}
		int distance(const mt::lang_pair& lng)
		{
	    		return ((lng.first == from_) + (lng.second == to_));
		}
		int from_,to_;
    	};

	QString find_name(const QMap<QString,int>& map,int num)
	{
		for(QMap<QString,int>::const_iterator it = map.begin();it!=map.end();++it)
    			if (it.data() == num) return it.key();
		return "";
	}

	void set_name(QComboBox& box,const QString& name)
	{
		for(int i=0;i<box.count();++i)
			if (box.text(i) == name)
			{
				box.setCurrentItem(i);
				return;
			}
	}

}


langpair_selector::langpair_selector(QWidget *parent):
	QWidget(parent,"lang_pair_selector"),
	layout_(this),
	from_(this),
	to_(this)
{
	layout_.addWidget(&from_);
	layout_.addWidget(&to_);

	names_["US"]=mt::english;
	names_["RU"]=mt::russian;

	//see all possible language pairs and made to lists of unique names
	const mt::linguas avail_langs;
	collect_langs l = std::for_each(avail_langs.begin(),
                		 	avail_langs.end(),
				 	collect_langs(nums_));

	for(QMap<QString,int>::const_iterator it = l.from_.begin();it!=l.from_.end();++it)
		from_.insertItem(it.key());

	for(QMap<QString,int>::const_iterator it = l.to_.begin();it!=l.to_.end();++it)
		to_.insertItem(it.key());

	xkeyboard& xkey = xkb::instance();

	connect(&xkey,SIGNAL(group_changed(int)),SLOT(on_group_change(int)));
	connect(&from_,SIGNAL(activated(int)),SLOT(on_from_change(int)));
	connect(&to_,SIGNAL(activated(int)),SLOT(on_to_change(int)));
    
	//assign current language state
	qWarning("current group is = %d",xkey.current());
	correct_pair(names_[xkey.name(xkey.current())],0);
}

void langpair_selector::on_group_change(int num)
{
	qWarning("group was changed to %d",num);
	correct_pair(names_[xkb::instance().name(num)],0);
}

void langpair_selector::on_from_change(int num)
{
	qWarning("from was changed to %d",num);
	correct_pair(nums_[from_.text(num)],0);
	xkb::instance().switch_to(find_name(names_,nums_[from_.currentText()]));
}

void langpair_selector::on_to_change(int num)
{
	qWarning("to was changed to %d",num);
	correct_pair(0,nums_[to_.text(num)]);
	xkb::instance().switch_to(find_name(names_,nums_[from_.currentText()]));
}

void langpair_selector::assign_pair(int fromlang,int tolang)
{
	if (fromlang) set_name(from_,find_name(nums_,fromlang));
	if (tolang) set_name(to_,find_name(nums_,tolang));
}

void langpair_selector::correct_pair(int fromlang,int tolang)
{
	qWarning("correct_pair: fromlang=%d tolang=%d",fromlang,tolang);

	mt::linguas avail_langs;
	//first best pair
	mt::linguas::iterator lang = std::max_element(avail_langs.begin(),
		                                      avail_langs.end(),
						      compare_names(fromlang,tolang));

	//check second element and fix it if we need it
	if (!fromlang)
	{//correct "from"
		int cur_fromlang = nums_[from_.currentText()];
		qWarning("correct from: current from is: %d",cur_fromlang);
		qWarning("correct from: current to is: %d",tolang);

		//try to find current pair as a valid
		mt::linguas::iterator cur = std::find(avail_langs.begin(),
					      	      avail_langs.end(),
					              mt::lang_pair(static_cast<mt::lang_code>(cur_fromlang),
					                            static_cast<mt::lang_code>(tolang)));
		if (cur == avail_langs.end())
		{
	    		qWarning("assign best pair: correct from and to");
	    		assign_pair(lang->first,lang->second);
		}
		else
		{
	    		qWarning("current values are normal, correct from only");
	    		assign_pair(0,tolang);
		}
		qWarning("correct from: current from is: %d",cur_fromlang);
    	}
	else if (!tolang)
    	{//correct "to"
		int cur_tolang = nums_[to_.currentText()];
		qWarning("correct to: current from is: %d",fromlang);
		qWarning("correct to: current to is: %d",cur_tolang);

		//try to find current pair as a valid
		mt::linguas::iterator cur = std::find(avail_langs.begin(),
					      	      avail_langs.end(),
					              mt::lang_pair(static_cast<mt::lang_code>(fromlang),
					                            static_cast<mt::lang_code>(cur_tolang)));
		if (cur == avail_langs.end())
		{
	    		qWarning("assign best pair: correct from and to");
	    		assign_pair(lang->first,lang->second);
		}
		else
		{
	    		qWarning("current values are normal, correct from only");
	    		assign_pair(fromlang,0);
		}
    	}
}

mt::lang_code langpair_selector::langin() const
{
	return static_cast<mt::lang_code>(nums_[from_.currentText()]);
}

mt::lang_code langpair_selector::langout() const
{
	return static_cast<mt::lang_code>(nums_[to_.currentText()]);
}

