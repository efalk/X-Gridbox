/* $Id$
 *
 * GridboxP.h - Private definitions for Gridbox widget
 */

#ifndef _GridboxP_h
#define _GridboxP_h

#include "Gridbox.h"


typedef struct {
    XtPointer	extension ;
} GridboxClassPart;

typedef struct _GridboxClassRec {
    CoreClassPart	core_class;
    CompositeClassPart	composite_class;
    ConstraintClassPart	constraint_class;
    GridboxClassPart	gridbox_class;
} GridboxClassRec;

extern GridboxClassRec gridboxClassRec;

typedef struct _GridboxPart {
    /* resources */
    int		defaultDistance;	/* default distance between children */

    /* private state */
    int		nx, ny ;
    Dimension	*max_wids, *max_hgts ;
    int		*max_weightx, *max_weighty ;
    Dimension	total_wid, total_hgt ;
    int		total_weightx, total_weighty ;
    Bool	needs_layout ;
} GridboxPart;

typedef struct _GridboxRec {
    CorePart		core;
    CompositePart	composite;
    ConstraintPart	constraint;
    GridboxPart		gridbox;
} GridboxRec;

typedef struct _GridboxConstraintsPart {
/*
 * Constraint Resources.
 */
    Position	gridx, gridy ;	/* position in the grid			*/
    Dimension	gridWidth,	/* size in cells			*/
    		gridHeight ;
    FillType	fill ;		/* how to expand to fill cell		*/
    int		gravity ;	/* position within cell			*/
    int		weightx,	/* how much excess space to absorb	*/
    		weighty ;
    int		margin ;
/*
 * Private contstraint resources.
 */
    Dimension	prefWidth, prefHeight ;	/* what the child wants to be	*/
} GridboxConstraintsPart;

typedef struct _GridboxConstraintsRec {
    GridboxConstraintsPart	gridbox;
} GridboxConstraintsRec, *GridboxConstraints;

#endif /* _GridboxP_h */
