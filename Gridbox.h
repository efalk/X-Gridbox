/* $Id: Gridbox.h,v 1.4 1999/12/12 06:07:17 falk Exp falk $
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


  defaultDistance	specifies the default margin around child widgets.

  All other resources are the same as for Constraint.

*/

/* Constraint parameters:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 gridx		     Position		Position	0
 gridy		     Position		Position	0
 gridWidth	     Width		Dimension	1
 gridHeight	     Height		Dimension	1
 fill		     Fill		FillType	FillBoth
 gravity	     Gravity		Gravity		CenterGravity
 weightx	     Weight		int		0
 weighty	     Weight		int		0
 margin		     Margin		int		defaultDistance
 allowResize	     AllowResize	Boolean		True


  gridx, gridy		position of child in grid.  Upper-left cell is 0,0
  			May be specified as GRIDBOX_NEXT to place after
			the previous child or GRIDBOX_SAME to place in
			the same row/column.
  gridWidth,gridHeight	size of child in cells.
  fill			"none", "width", "height" or "both"
  			(also: "fillnone", "fillwidth", "horizontal", "x",
			"fillheight", "vertical", "y", "fillboth", "all", "xy")
  gravity		position of child within cell when cell is larger.
  			See <X11/X.h> for list.
  weightx,weighty	effects how excess space is allocated to cells
  margin		margin around child within cell
  allowResize		child widget resize requests may be granted

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
#define	XtRGridPosition	"GridPosition"
#endif

#ifndef	XtNdefaultDistance
#define	XtNdefaultDistance	"defaultDistance"
#endif

#ifndef	XtNgravity
#define	XtNgravity	"gravity"
#define	XtCGravity	"Gravity"
#endif

#ifndef	XtNallowResize
#define	XtNallowResize	"allowResize"
#define	XtCAllowResize	"AllowResize"
#endif


typedef	unsigned int	FillType ;
#define	FillNone	0
#define	FillWidth	1
#define	FillHeight	2
#define	FillBoth	(FillWidth|FillHeight)

#define	GRIDBOX_NEXT	-1
#define	GRIDBOX_SAME	-2

typedef	struct _GridboxClassRec	*GridboxWidgetClass ;
typedef	struct _GridboxRec	*GridboxWidget ;

extern	WidgetClass	gridboxWidgetClass ;

_XFUNCPROTOBEGIN

_XFUNCPROTOEND
 
#endif /* _Gridbox_h */
