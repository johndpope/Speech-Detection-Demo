/*********************************************************************
 *
 * $Header: /usr2/eht/s3/src/libs2io/include/s3/RCS/s2_read_cb.h,v 1.4 1995/11/10 20:13:42 eht Exp $
 *
 * Carnegie Mellon ARPA Speech Group
 *
 * Copyright (c) 1995 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * file: s2_read_cb.h
 * 
 * traceability: 
 * 
 * description: 
 * 
 * author: 
 * 
 *********************************************************************/

#ifndef S2_READ_CB_H
#define S2_READ_CB_H

#include <s3/gauden.h>
#include <s3/prim_type.h>

int
s2_read_cb(gauden_t *g,
	   const char *in_dir_name,
	   const char **cb_base,
	   const char *mean_ext,
	   const char *var_ext,
	   int do_floor,
	   const int32 *do_fixvar);

#endif /* S2_READ_CB_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log: s2_read_cb.h,v $
 * Revision 1.4  1995/11/10  20:13:42  eht
 * Add a do_fixvar argument
 *
 * Revision 1.3  1995/11/10  20:10:40  eht
 * get rid of extra d
 *
 * Revision 1.2  1995/11/10  20:09:27  eht
 * add do_floor argument
 *
 * Revision 1.1  1995/08/15  13:46:15  eht
 * Initial revision
 *
 *
 */
