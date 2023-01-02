#ifndef QMTCC_XKEYBOARD_HH__
#define QTMCC_XKEYBOARD_HH__

#include <qobject.h>
#include <qstringlist.h>

#include <X11/Xlib.h>
#include <X11/XKBlib.h>

#include "single.hh"

/**
 * wrapper over XKeyboard Extension
 * this code was derived from kkbswitch (autor Leonid Zeitlin)
 */
class xkeyboard: public QObject
{
    Q_OBJECT
    static const int MaxNumKbdGroups = XkbNumKbdGroups;
    public:
	xkeyboard();
	virtual ~xkeyboard() {}
//	bool failed(); /**< are XKEYBOARD extention is available and valid */

	void switch_to(const QString&) const; /**< set current locked keyboard group */
	int current() const; /**< current group number */

	QString name(int groupno) const; /**< get name of the group */
	int amount() const; /**< return number of the groups */
    
	void processEvent(XEvent *ev);
    signals:
	void group_changed(int groupno); /**< new keyboard group was selected */
	void layout_changed(); /**< keyboard layout has changed and thus we need to reconfigure */
    private:
    	int find_index(const QString& name);
	void read_layout();

	int event_code_;
	int amount_;//total number of xkb groups
	bool failed_;
	QStringList names_;
};

typedef singleton_holder<xkeyboard> xkb;

#endif
