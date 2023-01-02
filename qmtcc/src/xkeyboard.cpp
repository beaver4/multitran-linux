/***************************************************************************
                          xkeyboard.cpp  -  description
                             -------------------
    begin                : Sun Jul 8 2001
    copyright            : (C) 2001 by Leonid Zeitlin
    email                : lz@europe.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "xkeyboard.h"

#include <qwindowdefs.h>
#include <qstringlist.h>

#include <kdebug.h>
#include <klocale.h>

XKeyboard *XKeyboard::m_self = 0;

XKeyboard::XKeyboard()
{
  Display *display = qt_xdisplay();
#ifdef HAVE_LIBXKLAVIER
//  XklSetDebugLevel(0);
  XklSetLogAppender(XklLogAppender);
  XklInit(display);
  XklRegisterStateCallback(XklStateCallback, this);
  XklRegisterConfigCallback(XklConfigCallback, this);
#else
  int opcode, errorBase, major = XkbMajorVersion, minor = XkbMinorVersion;

  // check the library version
  if (!XkbLibraryVersion(&major, &minor)) {
    kdWarning() << i18n("This program was built against XKB extension library\n"
      "version %1.%2, but is run with the library version %3.%4.\n"
      "This may cause various problems and even result in a complete\n"
      "failure to function\n").arg(XkbMajorVersion).arg(XkbMinorVersion).arg(major).arg(minor);
  }

  // initialize the extension
  m_xkb_available = XkbQueryExtension(display, &opcode, &m_event_code, &errorBase, &major, &minor);
  if (!m_xkb_available) {
    kdError() << i18n("The X Server does not support a compatible XKB extension.\n"
      "Either the server is not XKB-capable or the extension was disabled.\n"
      "This program would not work with this server, so it will exit now\n");
  }
  else {
    // register for XKB events
    //// group state change, i.e. the current group changed:
    XkbSelectEventDetails(display, XkbUseCoreKbd, XkbStateNotify,
      XkbAllStateComponentsMask, XkbGroupStateMask);
    //// keyboard mapping change:
    XkbSelectEventDetails(display, XkbUseCoreKbd, XkbMapNotify,
      XkbAllMapComponentsMask, XkbKeySymsMask);
    //// group names change:
    XkbSelectEventDetails(display, XkbUseCoreKbd, XkbNamesNotify,
      XkbAllNamesMask, XkbGroupNamesMask);
    //// new keyboard:
    XkbSelectEventDetails(display, XkbUseCoreKbd, XkbNewKeyboardNotify,
      XkbAllNewKeyboardEventsMask, XkbAllNewKeyboardEventsMask);
    // retrieve the number of keyboard groups
    retrieveNumKbdGroups();
  }
#endif
  m_self = this;
}

XKeyboard::~XKeyboard(){
#ifdef HAVE_LIBXKLAVIER
  XklStopListen();
  XklTerm();
#endif  
}

/** Determine if the given XEvent e is an XKB event with type XkbStateNotify.
  * If so, newgroupno will return the new keyboard group #.
  * In other words, return value of true means that this XEvent tells us
  * that the user just switched the keyboard group to the new value
  * newgroupno */
/*bool XKeyboard::isXkbStateNotifyEvent(XEvent *e, int *newgroupno){
  bool ret = false;
  if (e->type == m_event_code) {
    XkbEvent *kb_ev = (XkbEvent *) e;
    if (kb_ev->any.xkb_type == XkbStateNotify) {
      ret = true;
      *newgroupno = kb_ev->state.group;
    }
  }
  return ret;
}*/

/** Set the current keyboard group to the given groupno */
void XKeyboard::setGroupNo(int groupno){
#ifdef HAVE_LIBXKLAVIER
  XklLockGroup(groupno);
#else
  XkbLockGroup(qt_xdisplay(), XkbUseCoreKbd, groupno);
#endif  
}

#ifndef HAVE_LIBXKLAVIER
extern "C" {
  static int IgnoreXError(Display *, XErrorEvent *) {
    return 0;
  }
}
#endif

/** Get the names of the currently configured keyboard groups */
void XKeyboard::getGroupNames(QStringList &list){
#ifdef HAVE_LIBXKLAVIER
  const char** groupnames = XklGetGroupNames();
  int numgroups = XklGetNumGroups();
  for (int i = 0; i < numgroups; i++) 
    list.append(groupnames[i]);
#else
  XkbDescRec xkb;
  Display *display = qt_xdisplay();
  char *names[XkbNumKbdGroups];

  memset(&xkb, 0, sizeof(xkb));
  xkb.device_spec = XkbUseCoreKbd;
  XkbGetNames(display, XkbGroupNamesMask, &xkb);
  memset(names, 0, sizeof(char *) * XkbNumKbdGroups);
  // XGetAtomNames below may generate BadAtom error, which is not a problem.
  // (it may happen if the name for a group was not defined)
  // Thus we temporarily ignore X errors
  XErrorHandler old_handler = XSetErrorHandler(IgnoreXError);
  XGetAtomNames(display, xkb.names->groups, m_numgroups, names);
  // resume normal X error processing
  XSetErrorHandler(old_handler);
  for (int i = 0; i < m_numgroups; i++) {
    if (names[i]) {
      list.append(names[i]);
      XFree(names[i]);
    }
    else list.append(QString::null);
  }
  XkbFreeNames(&xkb, XkbGroupNamesMask, 1);
#endif  
}

XKeyboard * XKeyboard::self()
{
  return m_self;
}

/** return the current keyboard group index */
int XKeyboard::getGroupNo(){
#ifdef HAVE_LIBXKLAVIER
  return XklGetCurrentState()->group;
#else
  XkbStateRec rec;
  XkbGetState(qt_xdisplay(), XkbUseCoreKbd, &rec);
  return (int) rec.group;
#endif  
}

/** Returns if the given event notifies us of a keyboard layout change that requires a
  * reconfiguration
  * (e.g. new group added, group names changed, etc.) */
/*bool XKeyboard::isLayoutChangeEvent(XEvent *e){
  if (e->type == m_event_code) {
    XkbEvent *xkb_ev = (XkbEvent *) e;
    if ((xkb_ev->any.xkb_type == XkbMapNotify) && (xkb_ev->map.changed & XkbKeySymsMask)
      || (xkb_ev->any.xkb_type == XkbNamesNotify) && (xkb_ev->names.changed & XkbGroupNamesMask)
      || (xkb_ev->any.xkb_type == XkbNewKeyboardNotify)) {
      retrieveNumKbdGroups();
      return true;
    }
  }
  return false;
}*/

#ifndef HAVE_LIBXKLAVIER
/** No descriptions */
void XKeyboard::retrieveNumKbdGroups(){
  XkbDescRec xkb;

  memset(&xkb, 0, sizeof(xkb));
  /* Interestingly, in RedHat 6.0 (XFree86 3.3.3.1) the XkbGetControls call
  below works even if xkb.device_spec is not set. But in RedHat 7.1 (XFree86 4.0.3)
  it returns BadImplementation status code, and you have to specify
  xkb.device_spec = XkbUseCoreKbd. */
  xkb.device_spec = XkbUseCoreKbd;
  XkbGetControls(qt_xdisplay(), XkbGroupsWrapMask, &xkb);
  m_numgroups = xkb.ctrls->num_groups;
  XkbFreeControls(&xkb, XkbGroupsWrapMask, 1);
}
#endif

/** Examines an X Event passed to it and takes actions if the event is of
  * interest to XKeyboard */
void XKeyboard::processEvent(XEvent *ev) {
#ifdef HAVE_LIBXKLAVIER
  XklFilterEvents(ev);
#else
  if (ev->type == m_event_code) {
    // This an XKB event
    XkbEvent *xkb_ev = (XkbEvent *) ev;
    if (xkb_ev->any.xkb_type == XkbStateNotify) {
      // state notify event, the current group has changed
      emit groupChanged(xkb_ev->state.group);
    }
    else if ((xkb_ev->any.xkb_type == XkbMapNotify) && (xkb_ev->map.changed & XkbKeySymsMask)
      || (xkb_ev->any.xkb_type == XkbNamesNotify) && (xkb_ev->names.changed & XkbGroupNamesMask)
      || (xkb_ev->any.xkb_type == XkbNewKeyboardNotify)) {
      // keyboard layout has changed
      retrieveNumKbdGroups();
      emit layoutChanged();
    }
  }
#endif  
}

#ifdef HAVE_LIBXKLAVIER
void XKeyboard::XklStateCallback(XklStateChange changeType, int group,
  Bool /*restore*/, void */*userData*/)
{
  if (changeType == GROUP_CHANGED)
    emit XKeyboard::self()->groupChanged(group);
}

void XKeyboard::XklConfigCallback(void */*userData*/)
{
  emit XKeyboard::self()->layoutChanged();
}

void XKeyboard::XklLogAppender(const char file[], const char function[],
    int level, const char format[], va_list args)
{
  int size = vsnprintf(NULL, 0, format, args);
  char *str = new char[size + 1];
  vsnprintf(str, size, format, args);
  kdDebug() << file << "/" << function << ": " << str << endl;
  delete[] str;
}    
#endif
