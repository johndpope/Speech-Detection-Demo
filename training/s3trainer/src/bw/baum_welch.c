/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/bw/RCS/baum_welch.c,v 1.14 97/07/16 11:36:22 eht Exp Locker: eht $
 *
 * Carnegie Mellon ARPA Speech Group
 *
 * Copyright (c) 1995 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: baum_welch.c
 * 
 * Description: 
 *	This file contains the code to update the HMM reestimation
 *	equations given a model and an observation sequence of
 *	in a collection of multiple observation sequences (i.e. an
 *	utterance in a corpus of utterances).
 * 
 * Author: 
 * 	Eric H. Thayer (eht@cs.cmu.edu)
 *********************************************************************/

static char rcsid[] = "@(#)$Id: baum_welch.c,v 1.14 97/07/16 11:36:22 eht Exp Locker: eht $";

#include "baum_welch.h"
#include "forward.h"
#include "backward.h"
#include "accum.h"
#include <s3/state_seq.h>

#include <s3/model_inventory.h>
#include <s3/ckd_alloc.h>
#include <s3/profile.h>
#include <s3/corpus.h>

#include <s3/s2_param.h>

#include <s3/s3.h>

#include <math.h>
#include <assert.h>
#include <string.h>

/*********************************************************************
 *
 * Function:	baum_welch_update()
 * 
 * Description: 
 *	This routine updates the HMM reestimation equations given a
 *	model and an observation sequence of in a collection of
 *	multiple observation sequences (i.e. an	utterance in a
 *	corpus of utterances).
 *
 * Function Inputs: 
 *	float64 *log_forw_prob-
 *		log() of the probability of observation sequence
 *		given the model.
 *
 *	vector_t **feature -
 *		The observed feature vectors.	feature[t][f]
 *		is the feature vector for time t, feature f.
 *
 *	uint32 n_obs -
 *		The number of observations.  Defines the
 *		number of rows in the feature matrix.
 *
 *	state_t *state -
 *		The set of HMM states of the HMM to train.
 *
 *	uint32 n_state -
 *		The number of states present in the HMM.
 *
 *	model_inventory_t *inv -
 *		A structure containing pointers to the initial
 *		model parameters and reestimation sums.
 *
 *	int32 mixw_reest -
 *		A boolean value to indicate whether mixing weights
 *		should be reestimated.
 *
 *	int32 tmat_reest -
 *		A boolean value to indicate whether transition matrices
 *		should be reestimated.
 *
 *	int32 mean_reest -
 *		A boolean value to indicate whether means
 *		should be reestimated.
 *
 *	int32 var_reest -
 *		A boolean value to indicate whether variances
 *		should be reestimated.
 * 
 * Global Inputs: 
 *	None
 * 
 * Return Values: 
 * 
 * Global Outputs: 
 *	None
 * 
 *********************************************************************/

int32
baum_welch_update(float64 *log_forw_prob,
		  vector_t **feature,
		  uint32 n_obs,
		  float32 ****spkr_xfrm_ainv,
		  float32  ***spkr_xfrm_b,
		  state_t *state,
		  uint32 n_state,
		  model_inventory_t *inv,
		  float64 a_beam,
		  float64 b_beam,
		  float32 spthresh,
		  int32 mixw_reest,
		  int32 tmat_reest,
		  int32 mean_reest,
		  int32 var_reest,
		  int32 pass2var,
		  int32 mllr_mult, /* MLLR: accumulate multiplicative term */
		  int32 mllr_add)  /* MLLR: accumulate additive term */
{
    float64 *scale = NULL;
    float64 **dscale = NULL;
    float64 **active_alpha;
    uint32 **active_astate;
    uint32 *n_active_astate;
    float64 log_fp;	/* accumulator for the log of the probability
			 * of observing the input given the model */
    uint32 t;		/* time */
    gauden_t *g;	/* A structure containing the Gaussian
			 * densities */
    int ret;

    timing_t *fwd_timer = NULL;
    timing_t *bwd_timer = NULL;
    timing_t *rstu_timer = NULL;
    uint32 i,j;

    /* caller must ensure that there is some non-zero amount
       of work to be done here */
    assert(n_obs > 0);
    assert(n_state > 0);

    fwd_timer = timing_get("fwd");
    bwd_timer = timing_get("bwd");
    rstu_timer = timing_get("rstu");
    
    scale = (float64 *)ckd_calloc(n_obs, sizeof(float64));
    dscale = (float64 **)ckd_calloc(n_obs, sizeof(float64 *));
    n_active_astate = (uint32 *)ckd_calloc(n_obs, sizeof(uint32));
    active_alpha  = (float64 **)ckd_calloc(n_obs, sizeof(float64 *));
    active_astate = (uint32 **)ckd_calloc(n_obs, sizeof(uint32 *));

    /* Compute the scaled alpha variable and scale factors
     * for all states and time subject to the pruning constraints */
    if (fwd_timer)
	timing_start(fwd_timer);
    ret = forward(active_alpha, active_astate, n_active_astate, scale, dscale,
		  feature, n_obs, state, n_state,
		  inv, a_beam);
    if (fwd_timer)
	timing_stop(fwd_timer);

    if (ret != S3_SUCCESS) {

	/* Some problem with the utterance, release per utterance storage and
	 * forget about adding the utterance accumulators to the global accumulators */

	goto error;
    }

    /* Compute the scaled beta variable and update the reestimation
     * sums */
    if (bwd_timer)
	timing_start(bwd_timer);
    ret = backward_update(active_alpha, active_astate, n_active_astate, scale, dscale,
			  feature, n_obs, spkr_xfrm_ainv, spkr_xfrm_b,
			  state, n_state,
			  inv, b_beam, spthresh,
			  mixw_reest, tmat_reest, mean_reest, var_reest, pass2var,
			  mllr_mult, mllr_add);
    if (bwd_timer)
	timing_stop(bwd_timer);

    if (ret != S3_SUCCESS) {

	/* Some problem with the utterance, release per utterance storage and
	 * forget about adding the utterance accumulators to the global accumulators */

	goto error;
    }


    /* If no error was found in the forward or backward procedures,
     * add the resulting utterance reestimation accumulators to the
     * global reestimation accumulators */
    if (rstu_timer)
	timing_start(rstu_timer);
    accum_global(inv, state, n_state,
		 mixw_reest, tmat_reest, mean_reest, var_reest,
		 mllr_mult, mllr_add);
    if (rstu_timer)
	timing_stop(rstu_timer);

    for (i = 0; i < n_active_astate[n_obs-1] && active_astate[n_obs-1][i] != (n_state-1); i++);

    assert(i < n_active_astate[n_obs-1]);

    /* Calculate log[ p( O | \lambda ) ] */
    log_fp = log(active_alpha[n_obs-1][i]);
    for (t = 0; t < n_obs; t++) {
	log_fp -= log(scale[t]);
        for (j = 0; j < inv->gauden->n_feat; j++) {
	    log_fp += dscale[t][j];
        }
    }

    *log_forw_prob = log_fp;

    ckd_free((void *)scale);
    ckd_free(n_active_astate);
    for (i = 0; i < n_obs; i++) {
	ckd_free((void *)active_alpha[i]);
	ckd_free((void *)active_astate[i]);
	ckd_free((void *)dscale[i]);
    }
    ckd_free((void *)active_alpha);
    ckd_free((void *)active_astate);
    ckd_free((void **)dscale);

    return S3_SUCCESS;

error:
    ckd_free((void *)scale);
    for (i = 0; i < n_obs; i++) {
	if (dscale[i])
	    ckd_free((void *)dscale[i]);
    }
    ckd_free((void **)dscale);
    
    ckd_free(n_active_astate);
    for (i = 0; i < n_obs; i++) {
	ckd_free((void *)active_alpha[i]);
	ckd_free((void *)active_astate[i]);
    }
    ckd_free((void *)active_alpha);
    ckd_free((void *)active_astate);

    E_ERROR("%s ignored\n", corpus_utt_brief_name());

    return S3_ERROR;
}

/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	baum_welch.c,v $
 * Revision 1.14  97/07/16  11:36:22  eht
 * *** empty log message ***
 * 
 * Revision 1.13  1996/07/29  16:13:03  eht
 * - MLLR reestimation
 * - float64 rep for alpha, outprob, scale
 * - Got rid of 4D den and den_idx arrays as that they were using up
 *   too much memory.
 *
 * Revision 1.12  1996/03/26  13:49:54  eht
 * - Fixed beam bug where defined as float32 rather than float64
 * - Deal w/ case when # of densities referenced per utterances is much less than
 *   the total # of densities to train
 *
 * Revision 1.11  1996/03/05  12:47:45  eht
 * Fixed forward timer bug
 *
 * Revision 1.10  1996/03/04  17:32:07  eht
 * Add cpu usage counters
 *
 * Revision 1.9  1996/02/02  17:38:17  eht
 * Added alpha and beta beams.
 *
 * Revision 1.8  1996/01/26  18:23:49  eht
 * Local accumulators are now freed and reeallocated after each utterance.
 * No longer a need to clear them.
 *
 * Revision 1.7  1995/12/14  19:46:38  eht
 * - Added type casts for pointer types so that ANSI-hyper compilers desist about
 * generating warning messages.
 * - Added a clr of the Gaussian density accumulators when an error condition
 * happens.  Before the next utterance would get (possible) garbage from the
 * prior utterance.
 *
 * Revision 1.6  1995/11/30  20:47:43  eht
 * Allow a tmat_reest flag to be given and passed to lower level functions
 *
 * Revision 1.5  1995/10/10  12:43:50  eht
 * Changed to use <s3/prim_type.h>
 *
 * Revision 1.4  1995/10/09  14:55:33  eht
 * Change interface to new ckd_alloc routines
 *
 * Revision 1.3  1995/09/14  15:05:41  eht
 * Update comments
 *
 * Revision 1.2  1995/08/09  20:16:50  eht
 * Fix where Gaussian density reestimation accumulators were not cleared
 *
 * Revision 1.1  1995/06/02  20:41:22  eht
 * Initial revision
 *
 *
 */
