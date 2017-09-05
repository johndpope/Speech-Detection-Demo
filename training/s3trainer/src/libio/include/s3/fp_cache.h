/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libio/include/s3/RCS/fp_cache.h,v 1.1 97/07/16 11:39:10 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1997 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: fp_cache.h
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

#ifndef FP_CACHE_H
#define FP_CACHE_H

#include <s3/s3.h>
#include <s3/prim_type.h>

#include <stdio.h>

int
fpc_setmaxsz(uint32 sz);

int
fpc_setclose(void (*fn)(FILE *fp));

int
fpc_setopen(FILE * (*fn)(uint32 id));

int
fpc_n_id(uint32 n);

FILE *
fpc_get(uint32 id);

int
fpc_flush(void);

#endif /* FP_CACHE_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	fp_cache.h,v $
 * Revision 1.1  97/07/16  11:39:10  eht
 * Initial revision
 * 
 *
 */
