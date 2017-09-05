/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libcep_feat/RCS/v4_feat.h,v 1.1 97/07/16 11:38:16 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1997 Carnegie Mellon University.  
 *   All rights reserved.
 *
 *********************************************************************
 *
 * File: v4_feat.h
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

#ifndef V4_FEAT_H
#define V4_FEAT_H


#include <s3/vector.h>

const char *
v4_feat_doc(void);

uint32
v4_feat_id(void);

uint32
v4_feat_n_stream(void);

uint32
v4_feat_blksize( uint32);

const uint32 *
v4_feat_vecsize( uint32);

void
v4_feat_set_in_veclen(uint32 len);

vector_t **
v4_feat_alloc(uint32 n_frames, uint32 veclen);

void
v4_feat_free(vector_t **f);

vector_t **
v4_feat_compute(vector_t *mfcc,
		uint32 *inout_n_frame, uint32 veclen);

void
v4_feat_print(const char *label,
	      vector_t **f,
	      uint32 n_frames, uint32 veclen);

#endif /* V4_FEAT_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	v4_feat.h,v $
 * Revision 1.1  97/07/16  11:38:16  eht
 * Initial revision
 * 
 *
 */
