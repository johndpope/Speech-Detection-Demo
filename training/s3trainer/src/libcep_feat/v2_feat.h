/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libcep_feat/RCS/v2_feat.h,v 1.3 97/07/16 11:38:16 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1994 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: v2_feat.h
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

#ifndef V2_FEAT_H
#define V2_FEAT_H


#include <s3/vector.h>

const char *
v2_feat_doc(void);

uint32
v2_feat_id(void);

uint32
v2_feat_n_stream(void);

uint32
v2_feat_blksize( uint32);

const uint32 *
v2_feat_vecsize( uint32);

void
v2_feat_set_in_veclen(uint32 len);

vector_t **
v2_feat_alloc(uint32 n_frames, uint32 veclen);

void
v2_feat_free(vector_t **f);

void
deltacep_frame(vector_t dcep,
               vector_t mfcc);

void
doubledelta_frame(vector_t dcep,
                  vector_t mfcc);


vector_t **
v2_feat_compute(vector_t *mfcc,
		uint32 *inout_n_frame,
		uint32 veclen);

void
v2_feat_print(const char *label,
	      vector_t **f,
	      uint32 n_frames,
		uint32 veclen);

#endif /* V2_FEAT_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	v2_feat.h,v $
 * Revision 1.3  97/07/16  11:38:16  eht
 * *** empty log message ***
 * 
 * Revision 1.2  1996/03/25  15:36:54  eht
 * Allow for settable input feature length
 *
 * Revision 1.1  1996/01/30  17:09:59  eht
 * Initial revision
 *
 *
 */
