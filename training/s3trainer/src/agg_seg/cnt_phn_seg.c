/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/agg_seg/RCS/cnt_phn_seg.c,v 1.1 97/07/25 11:29:37 eht Exp Locker: eht $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1997 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: cnt_st_seg.c
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

static char rcsid[] = "@(#)$Id: cnt_phn_seg.c,v 1.1 97/07/25 11:29:37 eht Exp Locker: eht $";

#include "cnt_phn_seg.h"
#include "mk_seg.h"

#include <s3/lexicon.h>
#include <s3/corpus.h>
#include <s3/segdmp.h>
#include <s3/feat.h>
#include <s3/ck_seg.h>
#include <s3/mk_sseq.h>
#include <s3/mk_phone_seq.h>
#include <s3/ckd_alloc.h>

static uint32 *
get_next_phnseq(model_def_t *mdef,
		lexicon_t *lex,
		uint32 *out_n_phone)
{
    char *trans;
    uint16 *seg;
    uint32 n_frame;
    acmod_id_t *phone;
    uint32 n_phone;
    
    corpus_get_sent(&trans);

    mk_phone_seq(&phone, &n_phone, trans, mdef->acmod_set, lex);

    ckd_free(trans);

    *out_n_phone = n_phone;

    return phone;
}

typedef struct seg_len_str {
    uint32 len;
    struct seg_len_str *nxt;
} seg_len_t;

static seg_len_t **hd = NULL;
static seg_len_t **tl = NULL;

int
cnt_phn_seg(model_def_t *mdef,
	    lexicon_t *lex,
	    uint32 **out_n_seg,
	    uint32 ***out_n_frame_per)
{
    uint32 seq_no = 0;
    uint16 *seg;
    uint32 n_frame;
    uint32 i, j;
    uint32 n_acmod;
    uint32 *phone;
    uint32 n_phone;
    uint32 *n_seg;
    uint32 **n_frame_per;
    uint32 *start;
    uint32 *len;
    seg_len_t *cur;
    seg_len_t *tmp;
    seg_len_t *phn_hd;
    seg_len_t *phn_tl;

    n_acmod = acmod_set_n_acmod(mdef->acmod_set);

    E_INFO("Counting # occ. for %u models\n", n_acmod);
    
    n_seg = ckd_calloc(n_acmod, sizeof(uint32));
    hd = ckd_calloc(n_acmod, sizeof(seg_len_t *));
    tl = ckd_calloc(n_acmod, sizeof(seg_len_t *));

    for (seq_no = corpus_get_begin(); corpus_next_utt(); seq_no++) {
	if (!(seq_no % 250)) {
	    fprintf(stderr, " cnt[%u]", seq_no);
	    fflush(stderr);
	}
	
	corpus_get_seg(&seg, &n_frame);

	phone = get_next_phnseq(mdef, lex, &n_phone);
	
	ck_seg(mdef->acmod_set, phone, n_phone, seg, n_frame, corpus_utt());
	
	start = ckd_calloc(n_phone, sizeof(uint32));
	len = ckd_calloc(n_phone, sizeof(uint32));

	mk_seg(mdef->acmod_set, seg, n_frame, phone, start, len, n_phone);

	ckd_free(start);
	ckd_free(seg);
	ckd_free(phone);

	for (i = 0; i < n_phone; i++) {
	    /* insert the len for list phone[i] */

	    phn_hd = hd[phone[i]];
	    phn_tl = tl[phone[i]];

	    cur = (seg_len_t *)ckd_calloc(1, sizeof(seg_len_t));

	    cur->len = len[i];

	    if (phn_tl == NULL) {
		hd[phone[i]] = tl[phone[i]] = cur;
	    }
	    else {
		phn_tl->nxt = cur;
		tl[phone[i]] = cur;
	    }
	}
	ckd_free(len);
    }

    n_frame_per = (uint32 **)ckd_calloc(n_acmod, sizeof(uint32 *));

    for (i = 0; i < n_acmod; i++) {
	if (hd[i] == NULL) {
	    n_seg[i] = 0;
	}
	else {
	    for (cur = hd[i], j = 0; cur != NULL; j++, cur = cur->nxt);

	    n_seg[i] = j;

	    n_frame_per[i] = (uint32 *)ckd_calloc(n_seg[i], sizeof(uint32));

	    for (cur = hd[i], j = 0; cur != NULL; j++, cur = cur->nxt)
		n_frame_per[i][j] = cur->len;

	    for (cur = hd[i]; cur != NULL; cur = tmp) {
		tmp = cur->nxt;
		ckd_free(cur);
	    }
	    
	    E_INFO("phn= %s n_seg= %u\n",
		   acmod_set_id2name(mdef->acmod_set, i),
		   n_seg[i]);
	}
    }

    ckd_free(hd);
    ckd_free(tl);

    *out_n_seg = n_seg;
    *out_n_frame_per = n_frame_per;

    return S3_SUCCESS;
}

/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	cnt_phn_seg.c,v $
 * Revision 1.1  97/07/25  11:29:37  eht
 * Initial revision
 * 
 * Revision 1.1  97/07/16  11:36:22  eht
 * Initial revision
 * 
 *
 */
