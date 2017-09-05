/*********************************************************************
 *
 * $Header: /usr2/eht/s3/src/libcep_feat/include/s3/RCS/norm.h,v 1.2 1995/10/10 12:35:52 eht Exp $
 *
 * Carnegie Mellon ARPA Speech Group
 *
 * Copyright (c) 1995 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: norm.h
 * 
 * Traceability: 
 * 
 * Description: 
 * 
 * Author: 
 * 	$Author: eht $
 *********************************************************************/

#ifndef NORM_H
#define NORM_H

#include <s3/prim_type.h>

void norm_mean (float32 *vec,		/* the data */
		uint32 nvec,	/* number of vectors (frames) */
		uint32 veclen);	/* number of components per vector */

#endif /* NORM_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log: norm.h,v $
 * Revision 1.2  1995/10/10  12:35:52  eht
 * Changed to use <s3/prim_type.h>
 *
 * Revision 1.1  1995/08/15  13:45:04  eht
 * Initial revision
 *
 *
 */
