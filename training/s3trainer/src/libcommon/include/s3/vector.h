/*********************************************************************
 *
 * $Header: /usr2/eht/s3/src/libcommon/include/s3/RCS/vector.h,v 1.2 1995/10/09 20:55:35 eht Exp $
 *
 * Carnegie Mellon ARPA Speech Group
 *
 * Copyright (c) 1995 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: vector.h
 * 
 * Traceability: 
 * 
 * Description: 
 * 
 * Author: 
 * 	$Author: eht $
 *********************************************************************/

#ifndef VECTOR_H
#define VECTOR_H

#include <s3/prim_type.h>

typedef float *vector_t;

vector_t
vector_alloc(int32 n_dim);

int
vector_free(vector_t v);

void
vector_floor(vector_t v, uint32 dim, float32 f);

void
vector_nz_floor(vector_t v, uint32 dim, float32 f);

int32
vector_normalize(vector_t v, uint32 dim);

void
vector_print(vector_t v, uint32 dim);

#endif /* VECTOR_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log: vector.h,v $
 * Revision 1.2  1995/10/09  20:55:35  eht
 * Changes for prim_type.h
 *
 * Revision 1.1  1995/08/15  13:44:14  eht
 * Initial revision
 *
 *
 */
