/* $Id: Gridbox.c,v 1.2 1998/08/07 03:07:24 falk Rel falk $
 *
 * Gridbox.c - Gridbox composite widget
 *
 * Author: Edward A. Falk
 *	   falk@falconer.vip.best.com
 *
 * Date: August 1998
 *
 *
 * The Gridbox widget aligns its children in a rectangular array
 * of cells.  Child widgets occupy a rectangular region of cells (default
 * size is 1x1).  A typical layout might look like this:
 *
 *	+-------+-------+--+----------------------------+
 *	|	|	|  |				|
 *	|-------+-------|  |				|
 *	|	|	|  |				|
 *	|-------+-------|  |				|
 *	|	|	|  |				|
 *	|-------+-------|  |				|
 *	|	|	|  |				|
 *	|-------+-------+--+----------------------------|
 *	|__________________|____________________________|
 *
 * In addition, child widgets may specify a margin, and weights which
 * determine how they are resized if the parent widget is resized.
 *
 * $Log: Gridbox.c,v $
 * Revision 1.2  1998/08/07 03:07:24  falk
 * string => fill style converter slightly more robust
 *
 * Revision 1.1  1998/08/06 23:27:06  falk
 * Initial revision
 *
 */

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/Xmu/Converters.h>
#include <X11/Xmu/CharSet.h>
#include "GridboxP.h"

#define	DEFAULT_MARGIN	-1

#define Offset(field) XtOffsetOf(GridboxRec, gridbox.field)
static XtResource resources[] = {
    {XtNdefaultDistance, XtCThickness, XtRInt, sizeof(int),
	Offset(defaultDistance), XtRImmediate, (XtPointer)4},
};
#undef Offset

#define Offset(field) XtOffsetOf(GridboxConstraintsRec, gridbox.field)
static XtResource gridboxConstraintResources[] = {
    {XtNgridx, XtCPosition, XtRPosition, sizeof(Position),
	Offset(gridx), XtRImmediate, (XtPointer)0},
    {XtNgridy, XtCPosition, XtRPosition, sizeof(Position),
	Offset(gridy), XtRImmediate, (XtPointer)0},
    {XtNgridWidth, XtCWidth, XtRDimension, sizeof(Dimension),
	Offset(gridWidth), XtRImmediate, (XtPointer)1},
    {XtNgridHeight, XtCHeight, XtRDimension, sizeof(Dimension),
	Offset(gridHeight), XtRImmediate, (XtPointer)1},
    {XtNfill, XtCFill, XtRFillType, sizeof(FillType),
	Offset(fill), XtRImmediate, (XtPointer)FillBoth},
    {XtNgravity, XtCGravity, XtRGravity, sizeof(XtGravity),
	Offset(gravity), XtRImmediate, (XtPointer)CenterGravity},
    {XtNweightx, XtCWeight, XtRInt, sizeof(int),
	Offset(weightx), XtRImmediate, (XtPointer)0},
    {XtNweighty, XtCWeight, XtRInt, sizeof(int),
	Offset(weighty), XtRImmediate, (XtPointer)0},
    {XtNmargin, XtCMargin, XtRInt, sizeof(int),
	Offset(margin), XtRImmediate, (XtPointer)DEFAULT_MARGIN},
};
#undef Offset

#ifdef	__STDC__
static	void	GridboxClassInit() ;
static	void	GridboxInit(Widget request, Widget new, ArgList, Cardinal *) ;
static	void	GridboxExpose(Widget w, XEvent *event, Region region) ;
static	void	GridboxResize(Widget w) ;
static	Boolean	GridboxSetValues(Widget, Widget, Widget, ArgList, Cardinal *) ;
static	void	GridboxDestroy(Widget w) ;
static	XtGeometryResult
	  GridboxQueryGeometry(Widget, XtWidgetGeometry *, XtWidgetGeometry *) ;

static	void	GridboxChangeManaged(Widget w) ;
static	XtGeometryResult
	GridboxGeometryManager(Widget, XtWidgetGeometry *, XtWidgetGeometry *) ;

static	void	GridboxConstraintInit(Widget, Widget, ArgList, Cardinal *) ;
static	Boolean
	GridboxConstraintSetValues(Widget, Widget, Widget, ArgList, Cardinal *);


static	void	getPreferredSizes(GridboxWidget) ;
static	void	computeCellSize(GridboxWidget, GridboxConstraints,
			int *, int *, Dimension *, Dimension *) ;
static	void	freeAll(GridboxWidget) ;
static	void	computeWidHgtInfo(GridboxWidget) ;
static	void	computeWidHgtUtil(int, int, int, int, Dimension *, int *) ;
static	XtGeometryResult	changeGeometry(GridboxWidget, int, int,
			int, XtWidgetGeometry *) ;

static	Boolean	_CvtStringToFillType(Display *, XrmValuePtr, Cardinal *,
			XrmValuePtr, XrmValuePtr, XtPointer *) ;
#else
static	void	GridboxClassInit() ;
static	void	GridboxInit() ;
static	void	GridboxExpose() ;
static	void	GridboxResize() ;
static	Boolean	GridboxSetValues() ;
static	void	GridboxDestroy() ;
static	void	GridboxChangeManaged() ;
static	void	GridboxConstraintInit() ;
static	Boolean	GridboxConstraintSetValues() ;
static	void	getPreferredSizes() ;
static	void	computeCellSize() ;
static	void	freeAll() ;
static	void	computeWidHgtInfo() ;
static	void	computeWidHgtUtil() ;
static	XtGeometryResult	GridboxQueryGeometry() ;
static	XtGeometryResult	GridboxGeometryManager() ;
static	XtGeometryResult	changeGeometry() ;
static	Boolean	_CvtStringToFillType() ;
#endif

#define	XTCALLOC(n,type)	((type *) XtCalloc((n), sizeof(type)))

#ifndef	min
#define	min(a,b)	((a)<(b)?(a):(b))
#define	max(a,b)	((a)>(b)?(a):(b))
#endif

GridboxClassRec	gridboxClassRec = {
  { /* core_class fields */
    /* superclass         */    (WidgetClass) &constraintClassRec,
    /* class_name         */    "Gridbox",
    /* widget_size        */    sizeof(GridboxRec),
    /* class_initialize   */    GridboxClassInit,
    /* class_part_init    */    NULL,
    /* class_inited       */    FALSE,
    /* initialize         */    GridboxInit,
    /* initialize_hook    */    NULL,
    /* realize            */    XtInheritRealize,
    /* actions            */    NULL,
    /* num_actions        */    0,
    /* resources          */    resources,
    /* num_resources      */    XtNumber(resources),
    /* xrm_class          */    NULLQUARK,
    /* compress_motion    */    TRUE,
    /* compress_exposure  */    TRUE,
    /* compress_enterleave*/    TRUE,
    /* visible_interest   */    FALSE,
    /* destroy            */    GridboxDestroy,
    /* resize             */    GridboxResize,
    /* expose             */    GridboxExpose,
    /* set_values         */    GridboxSetValues,
    /* set_values_hook    */    NULL,
    /* set_values_almost  */    XtInheritSetValuesAlmost,
    /* get_values_hook    */    NULL,
    /* accept_focus       */    NULL,
    /* version            */    XtVersion,
    /* callback_private   */    NULL,
    /* tm_table           */    NULL,
    /* query_geometry     */	GridboxQueryGeometry,
    /* display_accelerator*/	XtInheritDisplayAccelerator,
    /* extension          */	NULL
  },
  { /* composite_class fields */
    /* geometry_manager   */   GridboxGeometryManager,
    /* change_managed     */   GridboxChangeManaged,
    /* insert_child       */   XtInheritInsertChild,
    /* delete_child       */   XtInheritDeleteChild,
    /* extension          */   NULL
  },
  { /* constraint_class fields */
    /* subresourses       */   gridboxConstraintResources,
    /* subresource_count  */   XtNumber(gridboxConstraintResources),
    /* constraint_size    */   sizeof(GridboxConstraintsRec),
    /* initialize         */   GridboxConstraintInit,
    /* destroy            */   NULL,
    /* set_values         */   GridboxConstraintSetValues,
    /* extension          */   NULL
  },
  { /* gridbox_class fields */
    /* extension	*/   NULL
  }
};

WidgetClass gridboxWidgetClass = (WidgetClass)&gridboxClassRec;

/****************************************************************
 *
 * Class Procedures
 *
 ****************************************************************/


static	void
GridboxClassInit()
{
    XtAddConverter( XtRString, XtRGravity, XmuCvtStringToGravity, NULL, 0) ;
    XtSetTypeConverter( XtRString, XtRFillType, _CvtStringToFillType,
	NULL, 0, XtCacheNone, (XtDestructor)NULL);
}


/* ARGSUSED */
static	void
GridboxInit(request, new, args, num_args)
    Widget request, new;
    ArgList args;
    Cardinal *num_args;
{
    GridboxWidget gb = (GridboxWidget)new;

    gb->gridbox.nx = gb->gridbox.ny = 0 ;
    gb->gridbox.max_wids = gb->gridbox.max_hgts = NULL ;
    gb->gridbox.max_weightx = gb->gridbox.max_weighty = NULL ;
    gb->gridbox.needs_layout = True ;
    getPreferredSizes(gb) ;

    if( request->core.width == 0 || request->core.height == 0 )
    {
      if( gb->gridbox.max_wids == NULL )
	computeWidHgtInfo(gb) ;

      if( request->core.width == 0 ) new->core.width = gb->gridbox.total_wid;
      if( request->core.height == 0 ) new->core.height = gb->gridbox.total_hgt;
    }

    /* TODO: resize now, or wait until later? */
}



/* No realize function */


/* ARGSUSED */
static	void
GridboxExpose(w, event, region)
	Widget	w ;
	XEvent	*event ;
	Region	region ;
{
	GridboxWidget gb = (GridboxWidget)w ;

	if( gb->gridbox.needs_layout )
	  XtClass(w)->core_class.resize(w) ;
}



static	void
GridboxResize(w)
    Widget w;
{
    GridboxWidget gb = (GridboxWidget)w;
    WidgetList children = gb->composite.children;
    int num_children = gb->composite.num_children;
    Widget	*childP;
    Position	x, y;
    int		width, height;
    int		i,j ;
    int		excess ;
    int		margin ;
    int		weight ;
    Dimension	*wids, *hgts ;
    Position	*xs, *ys ;
    int		mincellsize = gb->gridbox.defaultDistance * 2 + 1 ;

    /* determine how much space the rows & columns need */

    if( gb->gridbox.max_wids == NULL )
      computeWidHgtInfo(gb) ;

    /* find out how much excess there is */
    if( gb->gridbox.nx > 0 )
    {
      wids =  XTCALLOC(gb->gridbox.nx, Dimension) ;
      xs =  XTCALLOC(gb->gridbox.nx, Position) ;
      memcpy(wids, gb->gridbox.max_wids, gb->gridbox.nx * sizeof(Dimension)) ;
      excess = gb->core.width - gb->gridbox.total_wid ;
      weight = gb->gridbox.total_weightx ;

      /* distribute excess to the columns & assign positions */
      for(x=0, i=0; i < gb->gridbox.nx; ++i)
      {
	if( gb->gridbox.max_weightx[i] > 0 )
	{
	  j = wids[i] + gb->gridbox.max_weightx[i]*excess/weight ;
	  wids[i] = max(j,mincellsize) ;
	}
	xs[i] = x ;
	x += wids[i] ;
      }
    }

    if( gb->gridbox.ny > 0 )
    {
      /* Same again, for heights */
      hgts = XTCALLOC(gb->gridbox.ny, Dimension) ;
      ys = XTCALLOC(gb->gridbox.ny, Position) ;
      memcpy(hgts, gb->gridbox.max_hgts, gb->gridbox.ny * sizeof(Dimension)) ;
      excess = gb->core.height - gb->gridbox.total_hgt ;
      weight = gb->gridbox.total_weighty ;

      /* distribute it to the rows */
      for(y=0, i=0; i < gb->gridbox.ny; ++i)
      {
	if( gb->gridbox.max_weighty[i] > 0 )
	{
	  j = hgts[i] + gb->gridbox.max_weighty[i]*excess/weight ;
	  hgts[i] = max(j,mincellsize) ;
	}
	ys[i] = y ;
	y += hgts[i] ;
      }
    }

    /* Finally, loop through children, assign positions and sizes */
    /* Each child is assigned a size which is a function of its position
     * and size in cells.  The child's margin is subtracted from all sides.
     */

    for (childP = children; childP - children < num_children; childP++)
    {
      GridboxConstraints gc = (GridboxConstraints)(*childP)->core.constraints;
      if (!XtIsManaged(*childP)) continue;

      margin = gc->gridbox.margin ;

      computeCellSize(gb, gc, &width,&height, wids,hgts) ;
      x = xs[gc->gridbox.gridx] ;
      y = ys[gc->gridbox.gridy] ;

      /* Correct for preferred fill & alignment */
      if( !(gc->gridbox.fill & FillWidth)  &&
	  (excess = width - gc->gridbox.prefWidth) > 0 )
      {
	switch( gc->gridbox.gravity ) {
	  case CenterGravity: case NorthGravity: case SouthGravity:
	    x += excess/2 ;
	    break ;
	  case EastGravity: case NorthEastGravity: case SouthEastGravity:
	    x += excess ;
	    break ;
	}
	width = gc->gridbox.prefWidth ;
      }

      if( !(gc->gridbox.fill & FillHeight)  &&
	  (excess = height - gc->gridbox.prefHeight) > 0 )
      {
	switch( gc->gridbox.gravity ) {
	  case CenterGravity: case WestGravity: case EastGravity:
	    y += excess/2 ;
	    break ;
	  case SouthGravity: case SouthWestGravity: case SouthEastGravity:
	    y += excess ;
	    break ;
	}
	height = gc->gridbox.prefHeight ;
      }

      x += margin ;
      y += margin ;
      width -= 2 * (*childP)->core.border_width + 2 * margin ;
      height -= 2 * (*childP)->core.border_width + 2 * margin ;
      width = max(width,1) ;
      height = max(height,1) ;

      XtConfigureWidget(*childP,x,y, width, height,
      	(*childP)->core.border_width );
    }
    gb->gridbox.needs_layout = False ;
}


/* ARGSUSED */
static	Boolean
GridboxSetValues(current, request, new, args, num_args)
    Widget current, request, new;
    ArgList args;
    Cardinal *num_args;
{
    /* The only resource is the default margin.  I don't
     * think there's any reason to react to changes therein, so
     * this function does nothing.
     */

    return( FALSE );
}



/* No accept_focus function */


static	void
GridboxDestroy(w)
	Widget	w ;
{
	freeAll( (GridboxWidget)w ) ;
}


static	XtGeometryResult
GridboxQueryGeometry( widget, request, reply  )
    Widget widget;
    XtWidgetGeometry *request, *reply;
{
    GridboxWidget gb = (GridboxWidget)widget;

    /* determine how much space the rows & columns need */

    if( gb->gridbox.max_wids == NULL )
      computeWidHgtInfo(gb) ;

    reply->request_mode = CWWidth | CWHeight;
    reply->width = gb->gridbox.total_wid;
    reply->height = gb->gridbox.total_hgt;

    /* cheerfully accept anything >= our requested size, else offer
     * requested size as a compromise.
     */

    if( request->width == gb->core.width  &&
	request->height == gb->core.height )
      return XtGeometryNo;

    if( (request->request_mode & CWWidth) && request->width < reply->width ||
        (request->request_mode & CWHeight) && request->height < reply->height )
      return XtGeometryAlmost;

    return XtGeometryYes;
}





	/* COMPOSITE WIDGET FUNCTIONS */


static	void
GridboxChangeManaged(w)
    Widget w;
{
    GridboxWidget gb = (GridboxWidget)w;
    XtWidgetGeometry reply;
    int		width, height ;

    getPreferredSizes(gb) ;
    computeWidHgtInfo(gb) ;
    width = gb->gridbox.total_wid ;
    height = gb->gridbox.total_hgt ;

    /* ask to change geometry to accomodate; accept any compromise offered */
    if( changeGeometry(gb, width, height, False, &reply) == XtGeometryAlmost )
      (void) changeGeometry(gb, reply.width, reply.height, False, &reply) ;

    XtClass(w)->core_class.resize((Widget)gb) ;
}




	/* Respond to size change requests from a child.
	 *
	 * Recompute row/column sizes based on child request
	 * and request to change my own size accordingly.
	 *
	 * If parent grants; good.
	 * If parent offers compromise, accept.
	 * If parent refuses, live with it.
	 */

static	XtGeometryResult
GridboxGeometryManager(w, request, reply)
    Widget w;
    XtWidgetGeometry *request;
    XtWidgetGeometry *reply;	/* RETURN */
{
    int			new_width, new_height ;
    int			old_width, old_height ;
    int			cell_width, cell_height ;
    GridboxWidget	gb = (GridboxWidget) XtParent(w);
    GridboxConstraints	gc = (GridboxConstraints) w->core.constraints;
    XtWidgetGeometry	myreply ;
    int			queryOnly = request->request_mode & XtCWQueryOnly ;
    int			margin ;

    /* Position & border requests always denied */

    if( (request->request_mode & CWX) && request->x != w->core.x  ||
        (request->request_mode & CWY) && request->y != w->core.y  ||
        (request->request_mode & CWBorderWidth) &&
			request->border_width != w->core.border_width )
      return XtGeometryNo ;

    /* First, remember how much space we needed before request */
    old_width = gb->gridbox.total_wid ;
    old_height = gb->gridbox.total_hgt ;

    computeCellSize(gb, gc, &cell_width, &cell_height, NULL,NULL) ;

    /* set child's preferred size to the request value */
    margin = 2*w->core.border_width + 2*gc->gridbox.margin ;
    if( request->request_mode & (CWWidth|CWBorderWidth) )
      gc->gridbox.prefWidth = request->width + margin ;
    if( request->request_mode & (CWHeight|CWBorderWidth) )
      gc->gridbox.prefHeight = request->height + margin ;

    /* recompute row & column sizes */
    computeWidHgtInfo(gb) ;
    new_width = gb->gridbox.total_wid ;
    new_height = gb->gridbox.total_hgt ;


    /* resize myself to accomodate request; accept any compromise */
    if( changeGeometry(gb, new_width, new_height, queryOnly, &myreply)
    		== XtGeometryAlmost && !queryOnly )
      (void) changeGeometry(gb, myreply.width, myreply.height, queryOnly,
      								&myreply) ;


    /* If my new size is >= what I need, then accept */
    /* If the current cell size is big enough, use that */

    if( myreply.width >= new_width  &&  myreply.height >= new_height  ||
        cell_width >= gc->gridbox.prefWidth  && 
	cell_height >= gc->gridbox.prefHeight )
    {
      if( !queryOnly ) {
	XtClass((Widget)gb)->core_class.resize((Widget)gb) ;
	return XtGeometryDone ;
      }
      else
	return XtGeometryYes ;
    }

    /* OK, what kind of compromise can we offer? */


    /* If the new size is greater than the old, we can offer the excess
     * to the child.
     */

    if( myreply.width >= old_width  &&  myreply.height >= old_height )
    {
      new_width = w->core.width + myreply.width - old_width ;
      new_height = w->core.height + myreply.height - old_height ;

      if( new_width > request->width ) new_width = request->width ;
      if( new_height > request->height ) new_height = request->height ;

      if( new_width == request->width && new_height == request->height )
      {
	if( !queryOnly ) {
	  XtClass((Widget)gb)->core_class.resize((Widget)gb) ;
	  return XtGeometryDone ;
	}
	else
	  return XtGeometryYes ;
      }

      reply->width = new_width ;
      reply->height = new_height ;
      reply->request_mode = CWWidth | CWHeight ;
      return XtGeometryAlmost ;
    }

    return XtGeometryNo ;
}



/* No delete_child function.  It might make sense to resize the
 * grid again, but for now we'll just leave it alone.
 */




	/* CONSTRAINT WIDGET FUNCTIONS */


/* ARGSUSED */
static	void
GridboxConstraintInit(request, new, args, num_args)
    Widget request, new;
    ArgList args;
    Cardinal *num_args;
{
    GridboxConstraints gc = (GridboxConstraints)new->core.constraints;
    GridboxWidget gb = (GridboxWidget)new->core.parent;

    if( gc->gridbox.margin < 0 )
        gc->gridbox.margin = gb->gridbox.defaultDistance;

    /* TODO: how about resources that cause a child to use all
     * remaining space, or to start a new row?
     */
}



	/* No constraint destroy function */


/* ARGSUSED */
static	Boolean
GridboxConstraintSetValues(current, request, new, args, num_args)
    Widget current, request, new;
    ArgList args;
    Cardinal *num_args;
{
  GridboxConstraints gcCur = (GridboxConstraints) current->core.constraints;
  GridboxConstraints gcNew = (GridboxConstraints) new->core.constraints;
  GridboxWidget	gb = (GridboxWidget) XtParent(new) ;

  if (gcCur->gridbox.gridx	!= gcNew->gridbox.gridx		||
      gcCur->gridbox.gridy	!= gcNew->gridbox.gridy		||
      gcCur->gridbox.gridWidth	!= gcNew->gridbox.gridWidth	||
      gcCur->gridbox.gridHeight	!= gcNew->gridbox.gridHeight )
  {
      freeAll(gb) ;
      gb->gridbox.needs_layout = True ;
  }

  else if( gcCur->gridbox.fill		!= gcNew->gridbox.fill		||
	   gcCur->gridbox.gravity	!= gcNew->gridbox.gravity	||
	   gcCur->gridbox.gravity	!= gcNew->gridbox.gravity	||
	   gcCur->gridbox.gravity	!= gcNew->gridbox.gravity	||
	   gcCur->gridbox.gravity	!= gcNew->gridbox.gravity )
  {
      gb->gridbox.needs_layout = True ;
  }

  return False ;		/* what does this signify? */
}





	/* PRIVATE ROUTINES */



	/* Query all children, find out how much space they want.
	 * Add some for border & margin.
	 * Call this whenever the set of managed children changes.
	 */

static	void
getPreferredSizes(gb)
	GridboxWidget	gb ;
{
	int	i ;
	Widget	*childP;
	int	margin ;
	GridboxConstraints gc ;
	XtWidgetGeometry	preferred ;

	for( i=0, childP = gb->composite.children;
	     i < gb->composite.num_children ;
	     ++i, ++childP )
	{
	  gc = (GridboxConstraints) (*childP)->core.constraints ;

	  (void) XtQueryGeometry(*childP, NULL, &preferred) ;
	  margin = (gc->gridbox.margin + preferred.border_width) * 2 ;
	  gc->gridbox.prefWidth = preferred.width + margin ;
	  gc->gridbox.prefHeight = preferred.height + margin ;
	}
}


	/* Given a gridbox & child constraints, compute the size of
	 * the cell occupied by the child.
	 */

static	void
computeCellSize(gb, gc, wr,hr, wids,hgts)
	GridboxWidget	gb ;
	GridboxConstraints gc ;
	int		*wr, *hr ;
	Dimension	*wids, *hgts ;
{
	Position	x,y ;
	Dimension	w,h ;
	int		i ;

	if( wids == NULL ) wids = gb->gridbox.max_wids ;
	if( hgts == NULL ) hgts = gb->gridbox.max_hgts ;

	x = gc->gridbox.gridx ;
	y = gc->gridbox.gridy ;
	for(w=0, i=0; i < gc->gridbox.gridWidth; ++i)
	  w += wids[x+i] ;
	for(h=0, i=0; i < gc->gridbox.gridHeight; ++i)
	  h += hgts[y+i] ;

	*wr = w ;
	*hr = h ;
}



static	void
freeAll(gb)
    GridboxWidget	gb ;
{
    XtFree((char *)gb->gridbox.max_wids) ; gb->gridbox.max_wids = NULL ;
    XtFree((char *)gb->gridbox.max_hgts) ; gb->gridbox.max_hgts = NULL ;
    XtFree((char *)gb->gridbox.max_weightx) ; gb->gridbox.max_weightx = NULL ;
    XtFree((char *)gb->gridbox.max_weighty) ; gb->gridbox.max_weighty = NULL ;
}




	/* This function is the meat of the gridbox widget.  It performs
	 * the following actions:
	 *
	 * 1) loop through all children, finding their preferred
	 *    sizes.  (This has already been done in getPreferredSizes.)
	 *
	 * 2) Determine how many rows & columns there are in the grid.
	 *
	 * 3) Compute desired sizes for all rows & columns:
	 *   a) for all single-celled children, set the max desired
	 *      size for the corresponding rows & columns.
	 *   b) for all two-column children, set the max desired
	 *      size for both of the corresponding columns
	 *      by distributing the excess proportionally.
	 *   c) repeat for two-row children.
	 *   d) repeat for three-column, children
	 *   e) repeat for three-row children.
	 *   f) etc., until all children have been accounted for.
	 *
	 *   This is a non-deterministic algorithm, i.e. it is not
	 *   guaranteed to find the optimum row & column sizes.
	 *   I will have to give this some more thought.
	 *
	 * 4) Compute sums of row & column sizes.  This is our own
	 *    preferred size.
	 */

static	void
computeWidHgtInfo(gb)
    GridboxWidget	gb ;
{
    Widget	*childP ;
    int		i,j ;
    int		nc=0, nr=0 ;
    Dimension	*wids, *hgts ;
    int		*weightx, *weighty ;
    int		maxgw=0, maxgh=0 ;	/* max size in cells */
    GridboxConstraints	gc ;

    if( gb->composite.num_children <= 0 )
      return ;

    freeAll(gb) ;		/* start with clean slate */

    /* step 2:  Find out how many rows & columns there will be.
     */

    for( i=0, childP = gb->composite.children;
	 i < gb->composite.num_children ;
	 ++i, ++childP )
    {
      gc = (GridboxConstraints) (*childP)->core.constraints ;

      if( gc->gridbox.gridWidth > maxgw )
	maxgw = gc->gridbox.gridWidth ;
      if( gc->gridbox.gridHeight > maxgh )
	maxgh = gc->gridbox.gridHeight ;

      if( gc->gridbox.gridx + gc->gridbox.gridWidth > nc )
	nc = gc->gridbox.gridx + gc->gridbox.gridWidth ;
      if( gc->gridbox.gridy + gc->gridbox.gridHeight > nr )
	nr = gc->gridbox.gridy + gc->gridbox.gridHeight ;
    }

    gb->gridbox.nx = nc ;
    gb->gridbox.ny = nr ;

    gb->gridbox.max_wids	= wids	  = XTCALLOC(nc, Dimension) ;
    gb->gridbox.max_hgts	= hgts	  = XTCALLOC(nr, Dimension) ;
    gb->gridbox.max_weightx	= weightx = XTCALLOC(nc, int) ;
    gb->gridbox.max_weighty	= weighty = XTCALLOC(nr, int) ;


    /* step 3, examine children for the size they need,
     * compute row & column sizes accordingly.
     *
     * This is not coded effeciently, and might benefit from some
     * rethinking if the Gridbox widget is to be used with large grids.
     *
     * This also generates a non-optimum answer if large cells
     * partially overlap.
     */

    /* column widths */
    for(j=1; j<=maxgw; ++j)
    {
      for( i=0, childP = gb->composite.children;
	   i < gb->composite.num_children ;
	   ++i, ++childP )
      {
	gc = (GridboxConstraints) (*childP)->core.constraints ;
	if( gc->gridbox.gridWidth == j )
	  computeWidHgtUtil(gc->gridbox.gridx, gc->gridbox.gridWidth,
		gc->gridbox.prefWidth, gc->gridbox.weightx, wids, weightx) ;
      }
    }

    /* column heights */
    for(j=1; j<=maxgh; ++j)
    {
      for( i=0, childP = gb->composite.children;
	   i < gb->composite.num_children ;
	   ++i, ++childP )
      {
	gc = (GridboxConstraints) (*childP)->core.constraints ;
	if( gc->gridbox.gridHeight == j )
	  computeWidHgtUtil(gc->gridbox.gridy, gc->gridbox.gridHeight,
		gc->gridbox.prefHeight, gc->gridbox.weighty, hgts, weighty) ;
      }
    }


    /* Step 4: compute sums */

    gb->gridbox.total_wid = 0 ;
    gb->gridbox.total_weightx = 0 ;
    for(i=0; i<gb->gridbox.nx; ++i) {
      gb->gridbox.total_wid += wids[i] ;
      gb->gridbox.total_weightx += weightx[i] ;
    }

    gb->gridbox.total_hgt = 0 ;
    gb->gridbox.total_weighty = 0 ;
    for(i=0; i<gb->gridbox.ny; ++i) {
      gb->gridbox.total_hgt += hgts[i] ;
      gb->gridbox.total_weighty += weighty[i] ;
    }
}



static	void
computeWidHgtUtil(idx, ncell, wid, weight, wids, weights)
    int			idx, ncell ;
    int			wid, weight ;
    Dimension		*wids ;
    int			*weights ;
{
    /* 1 set the specified column weight(s) to the max of their current
     *   value and the weight of this widget.
     *
     * 2 find out if the available space in the indicated column(s)
     *   is enough to satisfy this widget.  If not, distribute the
     *   excess size by column weights.
     */

    int		i, cwid = 0 ;
    int		wtot = 0 ;		/* total weight of cells */
    int		excess ;

    for(i=0; i<ncell; ++i)
    {
      if( weights[idx+i] < weight )
	weights[idx+i] = weight ;
      cwid += wids[idx+i] ;
      wtot += weights[idx+i] ;
    }

    if( cwid < wid )
    {
      excess = wid - cwid    + ncell-1 ;	/* round up */
      for(i=0; i<ncell; ++i)
	if( wtot == 0 )
	  wids[idx+i] += excess/ncell ;
	else
	  wids[idx+i] += excess*weights[idx+i]/wtot ;
    }
}



	/* Make size change request.  Always return the resulting size.  */

static	XtGeometryResult
changeGeometry(gb, req_width,req_height, queryOnly, reply)
    GridboxWidget	gb ;
    int			req_width, req_height ;
    int			queryOnly ;
    XtWidgetGeometry	*reply ;
{
    XtGeometryResult	result ;
    Dimension	old_width = gb->core.width, old_height = gb->core.height;

    if( req_width != gb->core.width  ||  req_height != gb->core.height )
    {
      XtWidgetGeometry myrequest ;

      myrequest.width = req_width ;
      myrequest.height = req_height ;
      myrequest.request_mode = CWWidth | CWHeight ;
      if( queryOnly )
	myrequest.request_mode |= XtCWQueryOnly ;

      result = XtMakeGeometryRequest((Widget)gb, &myrequest, reply) ;

      /* BUG.  The Athena box widget (and probably others) will change
       * our dimensions even if this is only a query.  To work around that
       * bug, we restore our dimensions after such a query.
       */
      if( queryOnly ) {
	gb->core.width = old_width ;
	gb->core.height = old_height ;
      }
    }
    else
      result = XtGeometryYes ;

    switch( result ) {
      case XtGeometryYes:
      case XtGeometryDone:
	reply->width = req_width ;
	reply->height = req_height ;
	break ;
      case XtGeometryNo:
	reply->width = old_width ;
	reply->height = old_height ;
	break ;
      case XtGeometryAlmost:
	break ;
    }

    return result ;
}





	/* RESOURCES */

/* ARGSUSED */
static	Boolean
_CvtStringToFillType(dpy, args, num_args, fromVal, toVal, data)
    Display	*dpy ;
    XrmValuePtr args;		/* unused */
    Cardinal    *num_args;      /* unused */
    XrmValuePtr fromVal;
    XrmValuePtr toVal;
    XtPointer	*data ;
{
    String	str = (String)fromVal->addr ;
    static int	fillType;

    if( XmuCompareISOLatin1(str, "none") == 0  ||
        XmuCompareISOLatin1(str, "fillnone") == 0 )
      fillType = FillNone ;
    else if( XmuCompareISOLatin1(str, "width") == 0  ||
	     XmuCompareISOLatin1(str, "fillwidth") == 0  ||
	     XmuCompareISOLatin1(str, "horizontal") == 0  ||
	     XmuCompareISOLatin1(str, "x") == 0 )
      fillType = FillWidth ;
    else if( XmuCompareISOLatin1(str, "height") == 0  ||
	     XmuCompareISOLatin1(str, "fillheight") == 0  ||
	     XmuCompareISOLatin1(str, "vertical") == 0  ||
	     XmuCompareISOLatin1(str, "y") == 0 )
      fillType = FillHeight ;
    else if( XmuCompareISOLatin1(str, "both") == 0  ||
             XmuCompareISOLatin1(str, "fillboth") == 0  ||
             XmuCompareISOLatin1(str, "all") == 0  ||
             XmuCompareISOLatin1(str, "xy") == 0 )
      fillType = FillBoth ;
    else {
      XtStringConversionWarning(fromVal->addr, XtRFillType);
      return False ;
    }

    toVal->size = sizeof(fillType) ;
    if( toVal->addr )
    {
      if( toVal->size < sizeof(fillType) )
	return False ;
      else
	*((int *)toVal->addr) = fillType ;
    }
    else
      toVal->addr = (XPointer) &fillType;
    return True ;
}
