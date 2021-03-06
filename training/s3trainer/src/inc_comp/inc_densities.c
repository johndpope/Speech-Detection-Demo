/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/inc_comp/RCS/inc_densities.c,v 1.2 1996/07/29 16:26:54 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1994 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: inc_densities.c
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

static char rcsid[] = "@(#)$Id: inc_densities.c,v 1.2 1996/07/29 16:26:54 eht Exp $";

#include "inc_densities.h"

#include <s3/ckd_alloc.h>
#include <s3/feat.h>
#include <s3/s3.h>

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

static int
not_done(uint32 k, uint32 *did, uint32 n_did)
{
    uint32 i;

    for (i = 0; (i < n_did) && (did[i] != k); i++);

    if (i < n_did)
	return FALSE;
    else
	return TRUE;
}

int
inc_densities(float32 ***new_mixw,
	      vector_t ***new_mean,
	      vector_t ***new_var,

	      float32 ***mixw,
	      vector_t ***mean,
	      vector_t ***var,
	      float32 ***dnom,
	      
	      uint32 n_mixw,
	      uint32 n_mgau,
	      uint32 n_feat,
	      uint32 n_density,

	      uint32 n_inc)
{
    uint32 i, j, k, l, r;
    uint32 **did;
    float32 max_wt;
    uint32 max_wt_idx;
    const uint32 *veclen;
    float32 std;
    float32 m;

    veclen = feat_vecsize();

    assert(n_mgau <= n_mixw);

    if (n_mgau < n_mixw) {
	E_FATAL("Tied mixture Gaussians not yet implemented\n");
    }

    /* copy old parameters into new arrays */
    for (i = 0; i < n_mgau; i++) {
	for (j = 0; j < n_feat; j++) {
	    for (k = 0; k < n_density; k++) {
		memcpy(new_mean[i][j][k], mean[i][j][k], veclen[j]*sizeof(float32));
		memcpy(new_var[i][j][k], var[i][j][k], veclen[j]*sizeof(float32));
		new_mixw[i][j][k] = mixw[i][j][k];
	    }
	}
    }

    /* Over all mixtures:
     * 	- Find the largest unsplit component density.
     *		- Split it into two components where the new component
     *			mixw_a = mixw_b = 1/2 mixw
     *			mean_a = mean + 0.2 std
     *			mean_b = mean - 0.2 std
     *			var_a = var_b = var
     *
     * New parameters are placed beginning at index n_density
     */

    did = (uint32 **)ckd_calloc_2d(n_feat, n_inc, sizeof(uint32));

    for (i = 0; i < n_mgau; i++) {
	printf("%u:", i);
	fflush(stdout);
	
	for (r = 0; r < n_inc; r++) {
	    for (j = 0; j < n_feat; j++) {
		did[j][r] = n_density;

		/* find the density w/ the largest EM count not yet split (i.e. most probable, most occurances) */
		for (k = 0, max_wt = -1.0, max_wt_idx = n_density; k < n_density; k++) {
		    if ((max_wt < dnom[i][j][k]) && not_done(k, did[j], r)) {
			max_wt = dnom[i][j][k];
			max_wt_idx = k;
		    }
		}

		if ( dnom[i][j][max_wt_idx] < MIN_IEEE_NORM_POS_FLOAT32 ) {
		    E_WARN("(mgau= %u, feat= %u, density= %u) never observed skipping\n",
			   i, j, k);

		    new_mixw[i][j][n_density+r] = 0;

		    memcpy(new_var[i][j][n_density+r], var[i][j][0],
			   veclen[j]*sizeof(float32));

		    memcpy(new_mean[i][j][n_density+r], mean[i][j][0],
			   veclen[j]*sizeof(float32));

		    continue;
		}

		/* mixing weight of prior and new densities == 1/2 prior mixing weight */
		new_mixw[i][j][max_wt_idx] /= 2;
		new_mixw[i][j][n_density+r] = new_mixw[i][j][max_wt_idx];

		/* Keep variance of new class same as old */
		memcpy(new_var[i][j][n_density+r], var[i][j][max_wt_idx],
		       veclen[j]*sizeof(float32));

		/* mean_a = mean + 0.2 std */
		/* mean_b = mean - 0.2 std */
		for (l = 0; l < veclen[j]; l++) {
		    std = sqrt(var[i][j][max_wt_idx][l]);
		    
		    new_mean[i][j][max_wt_idx][l] = mean[i][j][max_wt_idx][l] + 0.2 * std;
		    new_mean[i][j][n_density+r][l]  = mean[i][j][max_wt_idx][l] - 0.2 * std;
		}

		did[j][r] = max_wt_idx;
		printf("%u(%.2e)", did[j][r], max_wt);
		fflush(stdout);
	    }
	}
	printf("\n");
    }
	    

    return S3_SUCCESS;
}

/*
 * Log record.  Maintained by RCS.
 *
 * $Log: inc_densities.c,v $
 * Revision 1.2  1996/07/29  16:26:54  eht
 * development release
 *
 * Revision 1.1  1996/01/26  18:22:55  eht
 * Initial revision
 *
 *
 */
