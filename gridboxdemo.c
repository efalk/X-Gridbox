static	char	rcsid[] = "$Id$" ;

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
	"*background:		gray",
	"*demoPanel.width:	500",
	"*demoPanel.height:	300",
	"*sampleBox.height:	100",
	"*sampleBox.border:	black",
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
	/* this could all be done with a single call to XtAppInitialize(),
	 * but I'm spelling it out for illustration purposes.
	 */

	XtToolkitInitialize() ;
	app_ctx = XtCreateApplicationContext() ;
	XtAppSetFallbackResources(app_ctx, fallback) ;
	dpy = XtOpenDisplay(app_ctx, NULL, "gridboxdemo", "Gridboxdemo",
		NULL, 0, &argc, argv) ;
	topLevel = XtVaAppCreateShell("gridboxdemo", "Gridboxdemo",
		applicationShellWidgetClass, dpy,
		0) ;

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
#ifdef	COMMENT
	(void) XtVaCreateManagedWidget("b13", commandWidgetClass, gridbox, 0) ;
	(void) XtVaCreateManagedWidget("b14", commandWidgetClass, gridbox, 0) ;
#endif	/* COMMENT */
	(void) XtVaCreateManagedWidget("b15", commandWidgetClass, gridbox, 0) ;
	(void) XtVaCreateManagedWidget("b16", commandWidgetClass, gridbox, 0) ;
#ifdef	COMMENT
	(void) XtVaCreateManagedWidget("b17", commandWidgetClass, gridbox, 0) ;
	(void) XtVaCreateManagedWidget("b18", commandWidgetClass, gridbox, 0) ;
#endif	/* COMMENT */
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
