/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/inc_comp/RCS/accum_wt_param.c,v 1.4 97/07/16 11:36:22 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1994 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: accum_wt_param.c
 * 
 * Description: 
 * 
 * Author: 
 * 	Eric H. Thayer (eht@cs.cmu.edu)
 *********************************************************************/

static char rcsid[] = "@(#)$Id: accum_wt_param.c,v 1.4 97/07/16 11:36:22 eht Exp $";

#include "accum_wt_param.h"

#include <s3/s3gau_io.h>
#include <s3/gauden.h>
#include <s3/ckd_alloc.h>
#include <s3/matrix.h>
#include <s3/s3.h>

#include <sys_compat/file.h>

int
accum_wt_param(char **accum_dir,
	       vector_t ****out_wt_mean,
	       vector_t ****out_wt_var,
	       int32 *out_pass2var,
	       float32 ****out_dnom,
	       uint32 *out_n_mgau,
	       uint32 *out_n_feat,
	       uint32 *out_n_density)
{
    uint32 i;
    char fn[MAXPATHLEN];
    const char *comment;
    
    vector_t ***in_wt_mean;
    vector_t ***in_wt_var;
    float32 ***in_dnom;

    vector_t ***acc_mean;
    vector_t ***acc_var;
    float32 ***acc_dnom;

    uint32 n_mgau = 0;
    uint32 t_n_mgau;
    uint32 n_feat = 0;
    uint32 t_n_feat;
    uint32 n_density = 0;
    uint32 t_n_density;
    const uint32 *t_veclen;
    const uint32 *veclen = NULL;
    int32 pass2var;

    for (i = 0; accum_dir[i] != NULL; i++) {
	sprintf(fn, "%s/gauden_counts", accum_dir[i]);

	if (access(fn, F_OK) == 0) {
	    E_INFO("Reading gaussian density count file %s\n", fn);

	    if (s3gaucnt_read(fn,
			      &in_wt_mean,
			      &in_wt_var,
			      &pass2var,
			      &in_dnom,
			      &t_n_mgau,
			      &t_n_feat,
			      &t_n_density,
			      &t_veclen) != S3_SUCCESS) {
		fflush(stdout);
		perror(fn);
		
		return S3_ERROR;
	    }
	    ckd_free((void *)comment);

	    if (n_mgau == 0) {
		n_mgau = t_n_mgau;
		n_feat = t_n_feat;
		n_density = t_n_density;
		veclen = t_veclen;

		acc_mean = gauden_alloc_param(n_mgau, n_feat, n_density, veclen);
		acc_var  = gauden_alloc_param(n_mgau, n_feat, n_density, veclen);
		acc_dnom = (float32 ***)ckd_calloc_3d(n_mgau,
						      n_feat,
						      n_density,
						      sizeof(float32));
	    }
	    else {
		if (n_mgau != t_n_mgau) {
		    E_ERROR("n_mgau, %u, in %s is inconsistent w/ the rest, %u.\n",
			    t_n_mgau, fn, n_mgau);
		}
		if (n_feat != t_n_feat) {
		    E_ERROR("n_feat, %u, in %s is inconsistent w/ the rest, %u.\n",
			    t_n_feat, fn, n_feat);
		}
		if (n_density != t_n_density) {
		    E_ERROR("n_density, %u, in %s is inconsistent w/ the rest, %u.\n",
			    t_n_density, fn, n_density);
		}

		ckd_free((void *)t_veclen);
	    }
	}
	else {
	    in_wt_var = in_wt_mean = NULL;
	    in_dnom = NULL;
	    t_veclen = NULL;
	}

	if (in_wt_mean == NULL) {
	    E_ERROR("Could not get weighted means for accumdir %s\n", accum_dir[i]);

	    exit(1);
	}

	if (in_wt_var == NULL) {
	    E_ERROR("Need weighted variances in accumdir %s to increment densities\n", accum_dir[i]);

	    exit(1);
	}

	/* accumulate the parameters read in from the count file */
	gauden_accum_param(acc_mean, in_wt_mean,
			   n_mgau, n_feat, n_density, veclen);
	ckd_free((void *)in_wt_mean[0][0][0]);
	ckd_free_3d((void ***)in_wt_mean);

	gauden_accum_param(acc_var, in_wt_var,
			   n_mgau, n_feat, n_density, veclen);
	ckd_free((void *)in_wt_var[0][0][0]);
	ckd_free_3d((void ***)in_wt_var);

	accum_3d(acc_dnom, in_dnom,
		 n_mgau, n_feat, n_density);
	ckd_free_3d((void ***)in_dnom);
    }

    /* pass outputs to the caller */
    *out_wt_mean = acc_mean;
    *out_wt_var  = acc_var;
    *out_dnom    = acc_dnom;
    *out_n_mgau  = n_mgau;
    *out_n_feat  = n_feat;
    *out_n_density = n_density;
    *out_pass2var = pass2var;

    return S3_SUCCESS;
}


/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	accum_wt_param.c,v $
 * Revision 1.4  97/07/16  11:36:22  eht
 * *** empty log message ***
 * 
 * Revision 1.3  1996/08/06  14:07:14  eht
 * Deal w/ 2 pass variance computation
 *
 * Revision 1.2  1996/07/29  16:26:54  eht
 * development release
 *
 * Revision 1.1  1996/01/26  18:22:55  eht
 * Initial revision
 *
 *
 */
