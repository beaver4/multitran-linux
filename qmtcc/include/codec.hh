#ifndef QMTCC_CODEC_HH__
#define QMTCC_CODEC_HH__

#include <qstring.h>
#include <qtextcodec.h>
#include "lang_info.hh"

QString codeset(mt::lang_code langnum);
QString conv(QTextCodec* codec,const std::string& str);

#endif
