static	char	rcsid[] = "$Id: gridboxdemo.c,v 1.5 1999/12/12 07:26:14 falk Exp $" ;

#include <stdio.h>
#include <stdlib.h>

#include <X11/StringDefs.h>
#include <X11/Intrinsic.h>
#include <X11/Core.h>
#include <X11/Object.h>
#include <X11/Shell.h>

#include <X11/Xaw/Box.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Scrollbar.h>

#include "Gridbox.h"

	/* X11 stuff */

static	Display	*dpy ;
static	Atom	wm_protocols ;
static	Atom	wm_delete_window ;

	/* Xt stuff */

static	XtAppContext	app_ctx ;

static	Widget	topLevel ;
static	Widget	toolBar, l1, b0,b1,b2,b3 ;

static String fallback[] = {
	"Gridboxdemo.baseTranslations: <Message>WM_PROTOCOLS: WMProtocols()",
	"*mainGrid.defaultDistance: 0",
	"*menuBar.?.borderWidth: 0",
	"*menuBar.?.shadowWidth: 0",
	"*menuBar.gridy: 0",
	"*menuBar.fill: Width",
	"*menuBar.weightx: 1",
	"*m0.label: File",
	"*m0.gridx: 0",
	"*m1.label: Edit",
	"*m1.gridx: 1",
	"*m2.label: Help",
	"*m2.gridx: 2",
	"*m2.weightx: 1",
	"*m2.fill: None",
	"*m2.gravity: East",
	"*toolBar.gridy: 1",
	"*toolBar.fill: Width",
	"*toolBar.weightx: 1",
	"*t0.label: Rd",
	"*t0.gridx: 0",
	"*t1.label: Sv",
	"*t1.gridx: 1",
	"*t2.label: New",
	"*t2.gridx: 2",
	"*t3.label: <<",
	"*t3.gridx: 3",
	"*t4.label: X",
	"*t4.gridx: 4",
	"*t5.label: >>",
	"*t5.gridx: 5",
	"*scrollGrid.gridy: 2",
	"*scrollGrid.fill: Both",
	"*scrollGrid.weightx: 1",
	"*scrollGrid.weighty: 1",
	"*scrollGrid.defaultDistance: 0",
	"*scrollGrid.?.borderWidth: 0",
	"*s1.orientation: Vertical",
	"*s1.gridx: 0",
	"*s1.gridy: 0",
	"*s1.fill: Both",
	"*s1.weighty: 1",
	"*s2.orientation: Horizontal",
	"*s2.gridx: 1",
	"*s2.gridy: 1",
	"*s2.fill: Both",
	"*s2.weightx: 1",
	"*sl.gridx: 1",
	"*sl.gridy: 0",
	"*sl.fill: Both",
	"*sl.weightx: 1",
	"*sl.weighty: 1",
	"*sl.bitmap: /usr/X11/include/X11/bitmaps/xlogo64",
	"*gridbox.defaultDistance: 0",
	"*gridbox.gridy: 3",
	"*gridbox.fill: Width",
	"*toolBar.weightx: 1",
	"*b0.label: button 0",
	"*b0.gridx: 0",
	"*b0.gridy: 0",
	"*b0.weighty: 1",
	"*b0.gridWidth: 2",
	"*b1.label: button 1",
	"*b1.gridx: 2",
	"*b1.gridy: 0",
	"*b2.label: button 2",
	"*b2.gridx: 3",
	"*b2.gridy: 0",
	"*b3.label: button 3",
	"*b3.gridx: 4",
	"*b3.gridy: 0",
	"*b4.label: button 4",
	"*b4.gridx: 0",
	"*b4.gridy: 1",
	"*b4.gridHeight: 2",
	"*b5.label: button 5",
	"*b5.gridx: 1",
	"*b5.gridy: 1",
	"*b6.label: button 6",
	"*b6.gridx: 2",
	"*b6.gridy: 1",
	"*b6.gridWidth: 3",
	"*b7.label: button 7",
	"*b7.gridx: 1",
	"*b7.gridy: 2",
	"*b7.gridHeight: 3",
	"*b7.margin: 3",
	"*b8.label: button 8",
	"*b8.gridx: 2",
	"*b8.gridy: 2",
	"*b8.weightx: 1",
	"*b9.label: button 9",
	"*b9.gridx: 3",
	"*b9.gridy: 2",
	"*b10.label: button 10",
	"*b10.gridx: 4",
	"*b10.gridy: 2",
	"*b11.label: button 11",
	"*b11.gridx: 0",
	"*b11.gridy: 3",
	"*b12.label: button 12",
	"*b12.gridx: 2",
	"*b12.gridy: 3",
	"*b13.label: button 13",
	"*b13.gridx: 3",
	"*b13.gridy: 3",
	"*b14.label: button 14",
	"*b14.gridx: 4",
	"*b14.gridy: 3",
	"*b15.label: button 15",
	"*b15.gridx: 0",
	"*b15.gridy: 4",
	"*b16.label: button 16",
	"*b16.gridx: 2",
	"*b16.gridy: 4",
	"*b17.label: button 17",
	"*b17.gridx: 3",
	"*b17.gridy: 4",
	"*b18.label: button 18",
	"*b18.gridx: 4",
	"*b18.gridy: 4",
	"*b19.label: button 19",
	"*b19.gridx: 0",
	"*b19.gridy: 5",
	"*b19.weighty: 1",
	"*b20.label: button 20",
	"*b20.gridx: 1",
	"*b20.gridy: 5",
	"*b21.label: button 21",
	"*b21.gridx: 2",
	"*b21.gridy: 5",
	"*b22.label: button 22",
	"*b22.gridx: 3",
	"*b22.gridy: 5",
	"*b23.label: button 23",
	"*b23.gridx: 4",
	"*b23.gridy: 5",
	"*l1.bitmap: /usr/X11/include/X11/bitmaps/xlogo32",
	"*l1.borderWidth: 0",
	"*l1.gridx: 3",
	"*l1.gridy: 3",
	"*l1.gridWidth: 2",
	"*l1.gridHeight: 2",
	NULL
};

#ifdef	__STDC__
static	void	mcb0(Widget, XtPointer, XtPointer) ;
static	void	mcb1(Widget, XtPointer, XtPointer) ;
static	void	mcb2(Widget, XtPointer, XtPointer) ;
static	void	mcb3(Widget, XtPointer, XtPointer) ;
static	void	WMProtocols(Widget, XEvent *, String *, Cardinal *) ;
#else
static	void	mcb0() ;
static	void	mcb1() ;
static	void	mcb2() ;
static	void	mcb3() ;
static	void	WMProtocols() ;
#endif


	/* define global actions table for the application.  This is done
	 * to catch messages.  This could also be done through the main
	 * event loop if we wrote one.  It would be nice if Xt had a way
	 * of regestering a callback proc for specific kinds of events.
	 */

static	XtActionsRec	gridbox_actions[] = {
    {"WMProtocols", WMProtocols},
} ;


static	void	GridboxDemo() ;


int
main(int argc, char **argv)
{
	topLevel = XtAppInitialize(&app_ctx, "Gridboxdemo", NULL,0,
		&argc,argv, fallback, NULL,0) ;
	dpy = XtDisplay(topLevel) ;

	XtAppAddActions(app_ctx, gridbox_actions, XtNumber(gridbox_actions));

	GridboxDemo() ;

	XtRealizeWidget(topLevel) ;

	/* tell window system we're willing to handle window-delete messages */
	wm_protocols = XInternAtom(dpy, "WM_PROTOCOLS", False);
	wm_delete_window = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
	(void) XSetWMProtocols(dpy, XtWindow(topLevel), &wm_delete_window,1);

	XtAppMainLoop(app_ctx) ;

	exit(0) ;
	/* NOTREACHED */
}



static	void
GridboxDemo()
{
  Widget	mainGrid, menuBar, scrollGrid, gridbox ;

  mainGrid = XtCreateManagedWidget("mainGrid",
	gridboxWidgetClass, topLevel, NULL, 0) ;

  menuBar = XtCreateManagedWidget("menuBar",
	gridboxWidgetClass, mainGrid, NULL, 0) ;

  toolBar = XtCreateWidget("toolBar",
	gridboxWidgetClass, mainGrid, NULL, 0) ;

  scrollGrid = XtCreateManagedWidget("scrollGrid",
	gridboxWidgetClass, mainGrid, NULL, 0) ;

  gridbox = XtCreateManagedWidget("gridbox",
	gridboxWidgetClass, mainGrid, NULL, 0) ;

  /* fill the menu bar */
  (void) XtVaCreateManagedWidget("m0", commandWidgetClass, menuBar, 0) ;
  (void) XtVaCreateManagedWidget("m1", commandWidgetClass, menuBar, 0) ;
  (void) XtVaCreateManagedWidget("m2", commandWidgetClass, menuBar, 0) ;

  /* fill the tool bar */
  (void) XtVaCreateManagedWidget("t0", commandWidgetClass, toolBar, 0) ;
  (void) XtVaCreateManagedWidget("t1", commandWidgetClass, toolBar, 0) ;
  (void) XtVaCreateManagedWidget("t2", commandWidgetClass, toolBar, 0) ;
  (void) XtVaCreateManagedWidget("t3", commandWidgetClass, toolBar, 0) ;
  (void) XtVaCreateManagedWidget("t4", commandWidgetClass, toolBar, 0) ;
  (void) XtVaCreateManagedWidget("t5", commandWidgetClass, toolBar, 0) ;

  /* Fill the scroll window */
  (void) XtVaCreateManagedWidget("s1", scrollbarWidgetClass, scrollGrid, 0) ;
  (void) XtVaCreateManagedWidget("s2", scrollbarWidgetClass, scrollGrid, 0) ;
  (void) XtVaCreateManagedWidget("sl", labelWidgetClass, scrollGrid, 0) ;

  /* fill the grid box */
  b0 = XtVaCreateManagedWidget("b0", commandWidgetClass, gridbox, 0) ;
  XtAddCallback(b0, XtNcallback, mcb0, NULL) ;
  b1 = XtVaCreateManagedWidget("b1", commandWidgetClass, gridbox, 0) ;
  XtAddCallback(b1, XtNcallback, mcb1, NULL) ;
  b2 = XtVaCreateManagedWidget("b2", commandWidgetClass, gridbox, 0) ;
  XtAddCallback(b2, XtNcallback, mcb2, NULL) ;
  b3 = XtVaCreateManagedWidget("b3", commandWidgetClass, gridbox, 0) ;
  XtAddCallback(b3, XtNcallback, mcb3, NULL) ;
  (void) XtVaCreateManagedWidget("b4", commandWidgetClass, gridbox, 0) ;
  (void) XtVaCreateManagedWidget("b5", commandWidgetClass, gridbox, 0) ;
  (void) XtVaCreateManagedWidget("b6", commandWidgetClass, gridbox, 0) ;
  (void) XtVaCreateManagedWidget("b7", commandWidgetClass, gridbox, 0) ;
  (void) XtVaCreateManagedWidget("b8", commandWidgetClass, gridbox, 0) ;
  (void) XtVaCreateManagedWidget("b9", commandWidgetClass, gridbox, 0) ;
  (void) XtVaCreateManagedWidget("b10", commandWidgetClass, gridbox, 0) ;
  (void) XtVaCreateManagedWidget("b11", commandWidgetClass, gridbox, 0) ;
  (void) XtVaCreateManagedWidget("b12", commandWidgetClass, gridbox, 0) ;
  l1 = XtVaCreateWidget("l1", labelWidgetClass, gridbox, 0) ;
  (void) XtVaCreateManagedWidget("b15", commandWidgetClass, gridbox, 0) ;
  (void) XtVaCreateManagedWidget("b16", commandWidgetClass, gridbox, 0) ;
  (void) XtVaCreateManagedWidget("b19", commandWidgetClass, gridbox, 0) ;
  (void) XtVaCreateManagedWidget("b20", commandWidgetClass, gridbox, 0) ;
  (void) XtVaCreateManagedWidget("b21", commandWidgetClass, gridbox, 0) ;
  (void) XtVaCreateManagedWidget("b22", commandWidgetClass, gridbox, 0) ;
  (void) XtVaCreateManagedWidget("b23", commandWidgetClass, gridbox, 0) ;
}





/* ARGSUSED */

static	void
mcb0(w, client, data)
    Widget	w ;
    XtPointer	client ;
    XtPointer	data ;
{
    XtManageChild(toolBar) ;
    XtSetSensitive(b0,False) ;
    XtSetSensitive(b2,True) ;
}

static	void
mcb1(w, client, data)
    Widget	w ;
    XtPointer	client ;
    XtPointer	data ;
{
    XtManageChild(l1) ;
    XtSetSensitive(b1,False) ;
    XtSetSensitive(b3,True) ;
}

static	void
mcb2(w, client, data)
    Widget	w ;
    XtPointer	client ;
    XtPointer	data ;
{
    XtUnmanageChild(toolBar) ;
    XtSetSensitive(b0,True) ;
    XtSetSensitive(b2,False) ;
}

static	void
mcb3(w, client, data)
    Widget	w ;
    XtPointer	client ;
    XtPointer	data ;
{
    XtUnmanageChild(l1) ;
    XtSetSensitive(b1,True) ;
    XtSetSensitive(b3,False) ;
}

/* ARGSUSED */

static	void
WMProtocols(w, ev, params, nparams)
	Widget	w ;
	XEvent	*ev ;
	String	*params ;
	Cardinal *nparams ;
{
	if( ev->type == ClientMessage  &&
	    ev->xclient.message_type == wm_protocols  &&
	    ev->xclient.data.l[0] == wm_delete_window )
	{
	  if( w == topLevel )
	    exit(0) ;
	}
}
