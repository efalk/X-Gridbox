static	char	rcsid[] = "$Id: gridboxdemo.c,v 1.5 1999/12/12 07:26:14 falk Exp falk $" ;

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
static	Widget	b0,b1,b2,b3 ;

static String fallback[] = {
	"Gridboxdemo.baseTranslations: <Message>WM_PROTOCOLS: WMProtocols()",
	"*mainGrid.defaultDistance: 0",
	"*b0.label: button 0",
	"*b0.gridx: 0",
	"*b0.gridy: 0",
	"*b0.weighty: 1",
	"*b0.gridWidth: 2",
	"*b1.label: button 1",
	"*b1.gridx: 2",
	"*b1.gridy: 0",
	"*b2.label: .",
	"*b2.gridx: 3",
	"*b2.gridy: 0",
	"*b3.label: button 3",
	"*b3.gridx: 4",
	"*b3.gridy: 0",
	NULL
};

#ifdef	__STDC__
static	void	WMProtocols(Widget, XEvent *, String *, Cardinal *) ;
#else
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
	topLevel = XtAppInitialize(&app_ctx, "Test", NULL,0,
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
  Widget	gridbox ;

  gridbox = XtCreateManagedWidget("mainGrid",
	gridboxWidgetClass, topLevel, NULL, 0) ;

  /* fill the grid box */
  b0 = XtVaCreateManagedWidget("b0", commandWidgetClass, gridbox, 0) ;
  b1 = XtVaCreateManagedWidget("b1", commandWidgetClass, gridbox, 0) ;
  b2 = XtVaCreateManagedWidget("b2", commandWidgetClass, gridbox, 0) ;
  b3 = XtVaCreateManagedWidget("b3", commandWidgetClass, gridbox, 0) ;
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
