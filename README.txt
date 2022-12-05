	$Id: Gridbox.README 2.1 1999/12/12 07:26:41 falk Exp $

Gridbox 2.0 -- arrange child widgets in a configurable grid.

	Written by Edward A. Falk <falk@falconer.vip.best.com>

    The Gridbox widget manages several children, displaying them in rows
    and columns.  Rows and columns are sized according to the needs of
    their widgets.  Child widgets may occupy multiple cells in the grid.
    Resource can control how rows and columns are resized when the
    Gridbox is resized.

    See Gridbox.man for more information.

    To view the man page:  tbl Gridbox.man | nroff -man



Installation:

    If your system supports Imake, simply give the commands:

	xmkmf
	make

    This will build the demo program as well as the Gridbox widget .o file.

    To use with Motif, compile with -DUSE_MOTIF

    If your system does not have Imake, or you don't wish to use it,
    some sample Makefiles are provided.  Choose the one closest to your
    own system, copy it to Makefile, and type "make".


Copyright (c) 1999	Edward A. Falk

	The X Consortium, and any party obtaining a copy of these files
	from the X Consortium, directly or indirectly, is granted, free of
	charge, a full and unrestricted irrevocable, world-wide, paid up,
	royalty-free, nonexclusive right and license to deal in this
	software and documentation files (the "Software"), including
	without limitation the rights to use, copy, modify, merge, publish,
	distribute, sublicense, and sell copies of the Software, and to
	permit persons who receive copies from any such party to do so.




Internals:

    getPreferredSizes()

	This function queries all child widgets to find out how
	much space they want.  In addition, grid positions
	GRIDBOX_NEXT and GRIDBOX_SAME are translated to real positions.

    computeCellSize()

    	Given a child widget constraints, and arrays of row & columns
	sizes, compute the size of the child's cell(s).

    computeWidHgtInfo()

    	Examine child widgets to see how many rows and columns there
	are.  Then call computeWidHgtMax()

    computeWidHgtMax()

    	Find the maximum child width for each column, and the maxium
	child height for each row.  These form the minimum sizes
	for the rows and columns.  Finally, sum up these row & columns
	sizes to determine our own preferred size.

    layout()

    	Given gridbox size, distribute excess to rows & columns.

    layoutChild()

    	Compute the size and position of one child widget.

    changeGeometry()

    	Make a geometry request; return resulting size.
