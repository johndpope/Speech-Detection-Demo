/*********************************************************************
 *
 * $Header: /usr2/eht/s3/src/libs2io/include/s3/RCS/s2_write_cb.h,v 1.2 1995/10/10 13:10:34 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1994 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: s2_write_cb.h
 * 
 * Description: 
 * 
 * Author:
 *	Eric H. Thayer (eht@cs.cmu.edu)
 *
 *********************************************************************/

#ifndef S2_WRITE_CB_H
#define S2_WRITE_CB_H

#include <s3/gauden.h>

int
s2_write_cb(gauden_t *g,
	    const char *out_dir_name,
	    const char **cb_base,
	    const char *mean_ext,
	    const char *var_ext,
	    int do_floor);

#endif /* S2_WRITE_CB_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log: s2_write_cb.h,v $
 * Revision 1.2  1995/10/10  13:10:34  eht
 * Changed to use <s3/prim_type.h>
 *
 * Revision 1.1  1995/09/08  19:13:52  eht
 * Initial revision
 *
 *
 */
