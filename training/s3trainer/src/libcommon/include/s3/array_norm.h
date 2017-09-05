/*********************************************************************
 *
 * $Header: /usr2/eht/s3/src/libcommon/include/s3/RCS/array_norm.h,v 1.2 1995/10/09 20:55:35 eht Exp $
 *
 * Carnegie Mellon ARPA Speech Group
 *
 * Copyright (c) 1995 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: array_norm.h
 * 
 * Traceability: 
 * 
 * Description: 
 * 
 * Author: 
 * 	$Author: eht $
 *********************************************************************/

#ifndef ARRAY_NORM_H
#define ARRAY_NORM_H

#include <s3/prim_type.h>

void
norm_3d_float32_array(float32 ***arr,
		      uint32 d1,
		      uint32 d2,
		      uint32 d3);

#endif /* ARRAY_NORM_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log: array_norm.h,v $
 * Revision 1.2  1995/10/09  20:55:35  eht
 * Changes needed for prim_type.h
 *
 * Revision 1.1  1995/08/15  13:44:14  eht
 * Initial revision
 *
 *
 */
