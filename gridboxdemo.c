static	char	rcsid[] = "$Id: gridboxdemo.c,v 1.1 1998/08/06 23:28:17 falk Exp falk $" ;

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
#include <X11/Xaw/AsciiText.h>

#include "Gridbox.h"

	/* X11 stuff */

static	Display	*dpy ;
static	Atom	wm_protocols ;
static	Atom	wm_delete_window ;

	/* Xt stuff */

static	XtAppContext	app_ctx ;

static	Widget	topLevel ;
static	Widget	gridbox ;

static String fallback[] = {
	"Gridboxdemo.baseTranslations: <Message>WM_PROTOCOLS: WMProtocols()",
	"*gridbox.defaultDistance: 0",
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

static	void	WMProtocols(Widget, XEvent *, String *, Cardinal *) ;


	/* define global actions table for the application.  This is done
	 * to catch messages.  This could also be done through the main
	 * event loop if we wrote one.  It would be nice if Xt had a way
	 * of regestering a callback proc for specific kinds of events.
	 */

static	XtActionsRec	gridbox_actions[] = {
    {"WMProtocols", WMProtocols},
} ;


static	void	GridboxDemo() ;


main(int argc, char **argv)
{
	topLevel = XtAppInitialize(&app_ctx, "Gridboxdemo", NULL,0,
		&argc,argv, fallback, NULL,0) ;
	dpy = XtDisplay(topLevel) ;

	XtAppAddActions(app_ctx, gridbox_actions, XtNumber(gridbox_actions));

	gridbox = XtVaCreateManagedWidget("gridbox",
		gridboxWidgetClass, topLevel,
		0) ;

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
	(void) XtVaCreateManagedWidget("b0", commandWidgetClass, gridbox, 0) ;
	(void) XtVaCreateManagedWidget("b1", commandWidgetClass, gridbox, 0) ;
	(void) XtVaCreateManagedWidget("b2", commandWidgetClass, gridbox, 0) ;
	(void) XtVaCreateManagedWidget("b3", commandWidgetClass, gridbox, 0) ;
	(void) XtVaCreateManagedWidget("b4", commandWidgetClass, gridbox, 0) ;
	(void) XtVaCreateManagedWidget("b5", commandWidgetClass, gridbox, 0) ;
	(void) XtVaCreateManagedWidget("b6", commandWidgetClass, gridbox, 0) ;
	(void) XtVaCreateManagedWidget("b7", commandWidgetClass, gridbox, 0) ;
	(void) XtVaCreateManagedWidget("b8", commandWidgetClass, gridbox, 0) ;
	(void) XtVaCreateManagedWidget("b9", commandWidgetClass, gridbox, 0) ;
	(void) XtVaCreateManagedWidget("b10", commandWidgetClass, gridbox, 0) ;
	(void) XtVaCreateManagedWidget("b11", commandWidgetClass, gridbox, 0) ;
	(void) XtVaCreateManagedWidget("b12", commandWidgetClass, gridbox, 0) ;
	(void) XtVaCreateManagedWidget("l1", labelWidgetClass, gridbox, 0) ;
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
