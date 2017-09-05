/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libcep_feat/RCS/v4_feat.c,v 1.1 97/07/16 11:36:22 eht Exp $
 *
 * Carnegie Mellon ARPA Speech Group
 *
 * Copyright (c) 1995 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: v4_feat.c
 * 
 * Description: 
 *
 * Author: 
 * 	Eric H. Thayer (eht@cs.cmu.edu)
 *********************************************************************/

static char rcsid[] = "@(#)$Id: v4_feat.c,v 1.1 97/07/16 11:36:22 eht Exp $";

#include "v4_feat.h"

#include <s3/feat.h>

#include "cep_frame.h"
#include "dcep_frame.h"
#include "ddcep_frame.h"

#include <s3/ckd_alloc.h>
#include <s3/cmd_ln.h>
#include <s3/s3.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>

#define N_FEAT		1

static uint32 n_feat = N_FEAT;
static uint32 vecsize[1];
static uint32 mfcc_len;

const char *
v4_feat_doc()
{
    return "1 stream :== < input feature vector >";
}

uint32
v4_feat_id()
{
    return FEAT_ID_V4;
}

uint32
v4_feat_n_stream()
{
    return n_feat;
}

void
v4_feat_set_in_veclen(uint32 veclen)
{
    mfcc_len = veclen;

    vecsize[0] = veclen;

}

uint32
v4_feat_blksize(uint32 veclen)
{
    v4_feat_set_in_veclen(veclen);
    return vecsize[0];
}

const uint32 *
v4_feat_vecsize(uint32 veclen)
{
    v4_feat_set_in_veclen(veclen);
    return vecsize;
}

vector_t **
v4_feat_alloc(uint32 n_frames, uint32 veclen)
{
    vector_t **out;
    float *data;
    uint32 len;
    uint32 i, j, k;
    uint32 frame_size;

    v4_feat_set_in_veclen(veclen);
    out = (vector_t **)ckd_calloc_2d(n_frames, n_feat, sizeof(vector_t));
    
    for (i = 0, frame_size = 0; i < n_feat; i++)
	frame_size += vecsize[i];

    len = n_frames * frame_size;
    
    data = ckd_calloc(len, sizeof(float32));
    
    for (i = 0, k = 0; i < n_frames; i++) {
	
	assert((k % frame_size) == 0);
	
	for (j = 0; j < n_feat; j++) {
	    out[i][j] = &data[k];
	    k += vecsize[j];
	}
    }

    assert(k == len);

    return out;
}

void
v4_feat_free(vector_t **f)
{
    ckd_free(f[0][0]);		/* frees the data block */

    ckd_free_2d((void **)f);	/* frees the access overhead */
}

vector_t **
v4_feat_compute(vector_t *mfcc,
		uint32 *inout_n_frame,
		uint32 veclen)
{
    vector_t mfcc_frame;
    vector_t **out;
    uint32 svd_n_frame;
    uint32 n_frame;
    uint32 i, j;
    void v4_mfcc_print(vector_t *mfcc, uint32 n_frame, uint32 veclen);
    
    v4_feat_set_in_veclen(veclen);
    n_frame = svd_n_frame = *inout_n_frame;

    out = v4_feat_alloc(n_frame,veclen);

    for (i = 0; i < n_frame; i++) {
	memcpy(out[i][0], mfcc[i], mfcc_len * sizeof(float32));
    }

    *inout_n_frame = n_frame;

    return out;
}

void
v4_mfcc_print(vector_t *mfcc, uint32 n_frame, uint32 veclen)
{
    uint32 i, j, k;

    v4_feat_set_in_veclen(veclen);
    for (i = 0; i < n_frame; i++) {
	printf("mfcc[%04u]: ", i);
	for (k = 0; k < mfcc_len; k++) {
	    printf("%6.3f ", mfcc[i][k]);
	}
	printf("\n");
    }
}


void
v4_feat_print(const char *label,
	      vector_t **f,
	      uint32 n_frames,
	      uint32 veclen)
{
    uint32 i;
    int32 j;
    uint32 k;
    char *name[] = { "" };
    vector_t *frame;
    vector_t stream;

    v4_feat_set_in_veclen(veclen);
    for (i = 0; i < n_frames; i++) {
	frame = f[i];

	for (j = 0; j < n_feat; j++) {
	    stream = frame[j];

	    printf("%s%s[%04u]: ", label, name[j], i);
	    for (k = 0; k < vecsize[j]; k++) {
		printf("%6.3f ", stream[k]);
	    }
	    printf("\n");
	}
	printf("\n");
    }
}

/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	v4_feat.c,v $
 * Revision 1.1  97/07/16  11:36:22  eht
 * Initial revision
 * 
 */