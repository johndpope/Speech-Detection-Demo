/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/param_cnt/RCS/cb_cnt.c,v 1.1 97/03/07 08:40:14 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1994 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: mixture_cnt.c
 * 
 * Description: 
 *
 * Author: 
 * 	Eric H. Thayer
 *********************************************************************/

static char rcsid[] = "@(#)$Id: cb_cnt.c,v 1.1 97/03/07 08:40:14 eht Exp $";

#include "cb_cnt.h"
#include "mk_sseq.h"

#include <s3/prim_type.h>
#include <s3/cvt2triphone.h>
#include <s3/ckd_alloc.h>
#include <s3/s3.h>

#include <stdio.h>

static int32 did_warn = 0;


int
cb_cnt(uint32 *cnt,	/* observation counts */

       model_def_t *mdef,	/* model definitions */
	    
       uint16 *seg,	/* Viterbi (CI phone,state) pairs for all frames */
       uint32 n_frame,

       acmod_id_t *phone,	/* CI phone sequence (must already be validated) */
       char *btw_mark,
       uint32 n_phone)
{
    uint32 *ci_sseq = NULL;
    uint32 *sseq = NULL;
    uint32 *cb = NULL;
    uint32 i;
    acmod_set_t *acmod_set = mdef->acmod_set;

    cb = mdef->cb;

    /* make a tied state id sequence from the state segmentation and the
       phone list */
    ci_sseq = mk_sseq(seg, n_frame, phone, n_phone, mdef);
    for (i = 0; i < n_frame; i++) {
	cnt[cb[ci_sseq[i]]]++;
    }

    if (cvt2triphone(acmod_set, phone, btw_mark, n_phone) != S3_SUCCESS) {
	if (!did_warn) {
	    E_WARN("Conversion from CI phones to triphones failed\n");
	}
	
	ckd_free(ci_sseq);
	return S3_SUCCESS;
    }
    
    /* make a tied state id sequence from the state segmentation and the
       phone list */
    sseq = mk_sseq(seg, n_frame, phone, n_phone, mdef);

    for (i = 0; i < n_frame; i++) {
	if (ci_sseq[i] != sseq[i])
	    cnt[cb[sseq[i]]]++;
    }

    ckd_free(ci_sseq);
    ckd_free(sseq);
    
    return S3_SUCCESS;
}

/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	cb_cnt.c,v $
 * Revision 1.1  97/03/07  08:40:14  eht
 * Initial revision
 * 
 * Revision 1.1  1996/03/25  15:21:20  eht
 * Initial revision
 *
 *
 */
