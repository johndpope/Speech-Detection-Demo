/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libclust/include/s3/RCS/merge_den.h,v 1.1 97/07/16 11:39:10 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1997 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: merge_den.h
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

#ifndef MERGE_DEN_H
#define MERGE_DEN_H


#include <s3/prim_type.h>

void
merge_d(float32 ***opdf,
	uint32 *i2o,

	float32 ***ipdf,
	uint32 n_ipdf,

	uint32 n_stream,
	uint32 n_cw);

void
interpolate_d(float32 ***opdf,
	      uint32 n_opdf,
	      float32 ***ipdf,
	      uint32 n_ipdf,
	      float32 *wt,
	      uint32 n_wt,
	      uint32 *i2wt,
	      uint32 *i2o,
	      uint32 n_stream,
	      uint32 n_codeword);


#endif /* MERGE_DEN_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	merge_den.h,v $
 * Revision 1.1  97/07/16  11:39:10  eht
 * Initial revision
 * 
 *
 */
