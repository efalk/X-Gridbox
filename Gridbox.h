/* $Id$
 *
 * This widget manages multiple child widgets, arranging them in a
 * rectangular grid.  Child widgets may occupy multiple grid cells.
 */



#ifndef _Gridbox_h
#define _Gridbox_h

#include <X11/Constraint.h>

/***********************************************************************
 *
 * Gridbox Widget -- loosely based on Java Gridbag layout
 *
 ***********************************************************************/

/* Parameters:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 background	     Background		Pixel		XtDefaultBackground
 border		     BorderColor	Pixel		XtDefaultForeground
 borderWidth	     BorderWidth	Dimension	1
 defaultDistance     Thickness		int		4
 destroyCallback     Callback		Pointer		NULL
 width		     Width		Dimension	computed at realize
 height		     Height		Dimension	computed at realize
 mappedWhenManaged   MappedWhenManaged	Boolean		True
 x		     Position		Position	0
 y		     Position		Position	0

*/

/* Constraint parameters:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 gridx		     Position		Position	GRID_RELATIVE
 gridy		     Position		Position	GRID_RELATIVE
 gridWidth	     Width		Dimension	1
 gridHeight	     Height		Dimension	1
 fill		     Fill		FillType	NONE
 gravity	     Gravity		int		Center
 weightx	     Weight		int		0
 weighty	     Weight		int		0
 margin		     Margin		int		defaultDistance

*/


#ifndef	XtNgridx
#define	XtNgridx	"gridx"
#define	XtNgridy	"gridy"
#define	XtNgridWidth	"gridWidth"
#define	XtNgridHeight	"gridHeight"
#define	XtNfill		"fill"
#define	XtNmargin	"margin"
#define	XtNweightx	"weightx"
#define	XtNweighty	"weighty"
#define	XtCFill		"Fill"
#define	XtCWeight	"Weight"
#define	XtRFillType	"FillType"
#endif

#ifndef	XtNdefaultDistance
#define	XtNdefaultDistance	"defaultDistance"
#endif

#ifndef	XtNgravity
#define	XtNgravity	"gravity"
#define	XtCGravity	"Gravity"
#endif


typedef	unsigned int	FillType ;
#define	FillNone	0
#define	FillWidth	1
#define	FillHeight	2
#define	FillBoth	(FillWidth|FillHeight)

typedef	struct _GridboxClassRec	*GridboxWidgetClass ;
typedef	struct _GridboxRec	*GridboxWidget ;

extern	WidgetClass	gridboxWidgetClass ;

_XFUNCPROTOBEGIN

_XFUNCPROTOEND
 
#endif /* _Gridbox_h */
