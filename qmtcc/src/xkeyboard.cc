#include "xkeyboard.hh"

static
int IgnoreXError(Display *, XErrorEvent *)
{
	return 0;
}

xkeyboard::xkeyboard():
	amount_(0)
{
	Display *display = qt_xdisplay();
	int opcode, errorBase, major = XkbMajorVersion, minor = XkbMinorVersion;

	if (!XkbLibraryVersion(&major, &minor))
    		qWarning("this program was compiled with different version of Xkb extension");

	// initialize the extension
	failed_ = (XkbQueryExtension(display, &opcode, &event_code_, &errorBase, &major, &minor) == False);
  	if (failed_)
     		qWarning("X Server doesn't support a compatible XKB extension");
  	else
  	{
    		XkbSelectEventDetails(display, XkbUseCoreKbd, XkbStateNotify,
    			  	      XkbAllStateComponentsMask, XkbGroupStateMask);// group state change
    		XkbSelectEventDetails(display, XkbUseCoreKbd, XkbMapNotify,
    			  	      XkbAllMapComponentsMask, XkbKeySymsMask);// keyboard mapping change
    		XkbSelectEventDetails(display, XkbUseCoreKbd, XkbNamesNotify,
    			  	      XkbAllNamesMask, XkbGroupNamesMask);// group names change
    		XkbSelectEventDetails(display, XkbUseCoreKbd, XkbNewKeyboardNotify,
    			  	      XkbAllNewKeyboardEventsMask, XkbAllNewKeyboardEventsMask);// new keyboard
  	}
  	read_layout();
}

void xkeyboard::processEvent(XEvent *ev)
{
	if (failed_) return;

	if (ev->type == event_code_)
    	{
		XkbEvent *xkb_ev = (XkbEvent *) ev;
		if (xkb_ev->any.xkb_type == XkbStateNotify)
		{
    	   		emit group_changed(xkb_ev->state.group);//the current group has changed
		}
		else if ((xkb_ev->any.xkb_type == XkbMapNotify) && (xkb_ev->map.changed & XkbKeySymsMask) ||
    			 (xkb_ev->any.xkb_type == XkbNamesNotify) && (xkb_ev->names.changed & XkbGroupNamesMask) ||
    			 (xkb_ev->any.xkb_type == XkbNewKeyboardNotify))
		{//keyboard layout has changed
    	    		qWarning("big layout change");
    	    		read_layout();
    	    		emit layout_changed();
		}
    	}
}

QString xkeyboard::name(int groupno) const
{
	//qWarning("names_ = %s",names[groupno]);
	return names_[groupno];
}

int xkeyboard::current() const
{
	XkbStateRec rec;
	XkbGetState(qt_xdisplay(), XkbUseCoreKbd, &rec);
	return static_cast<int>(rec.group);
}

int xkeyboard::amount() const
{
	return amount_;
}

void xkeyboard::switch_to(const QString& group) const
{
	qWarning("switch to group %d",names_.findIndex(group));
	XkbLockGroup(qt_xdisplay(), XkbUseCoreKbd, names_.findIndex(group));
}

QString create_short_name(const QString& str)
{
	QString res = str;
	res.replace(".","").replace(" ","");
	return res.left(2).upper();
}

void xkeyboard::read_layout()
{
	//get number of the group
	XkbDescRec xkb_desc;
	memset(&xkb_desc, 0, sizeof(xkb_desc));
	xkb_desc.device_spec = XkbUseCoreKbd;
	XkbGetControls(qt_xdisplay(), XkbGroupsWrapMask, &xkb_desc);
	amount_ = xkb_desc.ctrls->num_groups;
	XkbFreeControls(&xkb_desc, XkbGroupsWrapMask, 1);

	qWarning("total amount of the groups are:%d",amount_);
  
	//also get all names
	char *names[XkbNumKbdGroups];

	memset(&xkb_desc, 0, sizeof(xkb_desc));
	xkb_desc.device_spec = XkbUseCoreKbd;
	XkbGetNames(qt_xdisplay(), XkbGroupNamesMask, &xkb_desc);
	memset(names, 0, sizeof(char *) * XkbNumKbdGroups);
    
	XErrorHandler old_handler = XSetErrorHandler(IgnoreXError);
	XGetAtomNames(qt_xdisplay(), xkb_desc.names->groups, amount_, names);
	XSetErrorHandler(old_handler);
    
	for (int i = 0; i < amount_; ++i)
	{
		if (names[i])
		{
    	    		qWarning("group name is %s",names[i]);
    	    		names_.append(create_short_name(names[i]));
    	    		XFree(names[i]);
		}
		else
	    		names_.append(QString::null);
    	}
    	XkbFreeNames(&xkb_desc, XkbGroupNamesMask, 1);
}
