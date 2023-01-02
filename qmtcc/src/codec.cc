#include "codec.hh"

QString codeset(mt::lang_code langnum)
{
	return 
		std::use_facet<mt::identification>(
	 	*mt::get_locale(langnum)).codeset().c_str();
}

QString conv(QTextCodec* codec,const std::string& str)
{
	return codec->toUnicode(str.c_str());
}
