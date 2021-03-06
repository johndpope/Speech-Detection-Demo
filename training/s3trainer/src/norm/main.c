/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/norm/RCS/main.c,v 1.16 97/07/16 11:22:03 eht Exp Locker: eht $
 *
 * Carnegie Mellon ARPA Speech Group
 *
 * Copyright (c) 1995 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: main.c
 * 
 * Description: 
 *	This normalizes the counts produced by bw(1).
 * 
 * Author: 
 *	Eric H. Thayer (eht@cs.cmu.edu)
 * 
 *********************************************************************/

static char rcsid[] = "@(#)$Id: main.c,v 1.16 97/07/16 11:22:03 eht Exp Locker: eht $";

#include "parse_cmd_ln.h"

/* The SPHINX-III common library */
#include <s3/common.h>

#include <s3/model_inventory.h>
#include <s3/model_def_io.h>
#include <s3/s3gau_io.h>
#include <s3/s3mixw_io.h>
#include <s3/s3tmat_io.h>
#include <s3/regmat_io.h>
#include <s3/matrix.h>

/* Some SPHINX-II compatibility definitions */
#include <s3/s2_param.h>

#include <s2/log.h>

#include <sys_compat/file.h>
#include <sys_compat/misc.h>

#include <stdio.h>
#include <math.h>
#include <assert.h>

#include <s3/mllr.h>
#include <s3/mllr_io.h>

static int normalize(void);

static int
initialize(int argc,
	   char *argv[])
{
    char file_name[MAXPATHLEN+1];

    /* define, parse and (partially) validate the command line */
    parse_cmd_ln(argc, argv);

    return S3_SUCCESS;
}

static void
free_tmat_accum(float32 ***tmat_acc)
{
    ckd_free_3d((void ***)tmat_acc);
}

static void
free_mixw_accum(float32 ***mixw_acc)
{
    ckd_free_3d((void ***)mixw_acc);
}

static void
free_mean_accum(vector_t ***mean)
{
    ckd_free((void *)mean[0][0][0]);
    ckd_free_3d((void ***)mean);
}

static void
free_var_accum(vector_t ***var)
{
    ckd_free((void *)var[0][0][0]);
    ckd_free_3d((void ***)var);		
}

static void
free_dnom_accum(float32 ***dnom)
{
    ckd_free_3d((void ***)dnom);
}

static int
ck_readable(const char *fn)
{
    FILE *fp;

    fp = fopen(fn, "r");
    if (fp != NULL) {
	fclose(fp);
	return TRUE;
    }
    else {
	return FALSE;
    }
}

static int
rdacc_tmat(const char *dir,

	   float32 ****inout_tmat_acc,
	   uint32 *inout_n_tmat,
	   uint32 *inout_n_state_pm)
{
    char fn[MAXPATHLEN+1];
    float32 ***in_tmat_acc;
    float32 ***tmat_acc;
    uint32 n_tmat;
    uint32 n_state_pm;

    sprintf(fn, "%s/tmat_counts", dir);

    if (ck_readable(fn)) {
	if (s3tmat_read(fn,
			&in_tmat_acc,
			&n_tmat,
			&n_state_pm) != S3_SUCCESS) {
	    return S3_ERROR;
	}
	tmat_acc = *inout_tmat_acc;

	if (tmat_acc == NULL) {
	    *inout_tmat_acc = tmat_acc = in_tmat_acc;
	    *inout_n_tmat = n_tmat;
	    *inout_n_state_pm = n_state_pm;
	}
	else {
	    int err = FALSE;

	    if (*inout_n_tmat != n_tmat) {
		E_ERROR("# tmat in, %u != prior # tmat, %u\n",
			n_tmat, *inout_n_tmat);
		err = TRUE;
	    }
	    if (*inout_n_state_pm != n_state_pm) {
		E_ERROR("# tmat in, %u != prior # tmat, %u\n",
			n_state_pm, *inout_n_state_pm);
		err = TRUE;
	    }

	    if (err)
		return S3_ERROR;

	    accum_3d(tmat_acc, in_tmat_acc,
		     n_tmat, n_state_pm-1, n_state_pm);

	    ckd_free_3d((void ***)in_tmat_acc);
	}
    }
    else {
	E_ERROR("Unable to access %s\n", fn);

	return S3_ERROR;
    }

    return S3_SUCCESS;
}

static int
rdacc_mixw(const char *dir,
	   
	   float32 ****inout_mixw_acc,
	   uint32 *inout_n_mixw,
	   uint32 *inout_n_stream,
	   uint32 *inout_n_density)
{
    char fn[MAXPATHLEN+1];
    float32 ***in_mixw_acc;
    float32 ***mixw_acc;
    uint32 n_mixw;
    uint32 n_stream;
    uint32 n_density;

    sprintf(fn, "%s/mixw_counts", dir);

    if (ck_readable(fn)) {
	if (s3mixw_read(fn,
			&in_mixw_acc,
			&n_mixw,
			&n_stream,
			&n_density) != S3_SUCCESS) {
	    return S3_ERROR;
	}

	mixw_acc = *inout_mixw_acc;

	if (mixw_acc == NULL) {
	    *inout_mixw_acc = mixw_acc = in_mixw_acc;
	    *inout_n_mixw = n_mixw;
	    *inout_n_stream = n_stream;
	    *inout_n_density = n_density;
	}
	else {
	    int err = FALSE;
	    
	    if (*inout_n_mixw != n_mixw) {
		E_ERROR("# mixw in file %s (== %u) != prior # mixw (== %u)\n",
			fn, n_mixw, *inout_n_mixw);
		err = TRUE;
	    }

	    if (*inout_n_stream != n_stream) {
		E_ERROR("# stream in file %s (== %u) != prior # stream (== %u)\n",
			fn, n_stream, *inout_n_stream);
		err = TRUE;
	    }

	    if (*inout_n_density != n_density) {
		E_ERROR("# density comp/mix in file %s (== %u) != prior # density (== %u)\n",
			fn, n_density, *inout_n_density);
		err = TRUE;
	    }

	    if (err)
		return S3_ERROR;

	    accum_3d(mixw_acc, in_mixw_acc,
		     n_mixw, n_stream, n_density);

	    ckd_free_3d((void ***)in_mixw_acc);
	}
    }
    else {
	E_ERROR("Unable to access %s\n", fn);

	return S3_ERROR;
    }

    return S3_SUCCESS;
}

static int
rdacc_den(const char *dir,

	  vector_t ****inout_wt_mean,
	  vector_t ****inout_wt_var,
	  int32 *inout_pass2var,
	  float32  ****inout_dnom,
	  uint32 *inout_n_mgau,
	  uint32 *inout_n_stream,
	  uint32 *inout_n_density,
	  const uint32 **inout_veclen)
{
    char fn[MAXPATHLEN+1];
    vector_t ***in_wt_mean;
    vector_t ***wt_mean;
    vector_t ***in_wt_var;
    vector_t ***wt_var;
    float32 ***in_dnom;
    float32 ***dnom;
    uint32 n_mgau;
    uint32 n_stream;
    uint32 n_density;
    const uint32 *veclen;
    const uint32 *in_veclen;
    int32 pass2var;
    int i;

    sprintf(fn, "%s/gauden_counts", dir);

    if (ck_readable(fn)) {
	if (s3gaucnt_read(fn,
			  &in_wt_mean,
			  &in_wt_var,
			  &pass2var,
			  &in_dnom,
			  &n_mgau,
			  &n_stream,
			  &n_density,
			  &in_veclen) != S3_SUCCESS) {
	    fflush(stdout);
	    perror(fn);
	    
	    return S3_ERROR;
	}

	wt_mean = *inout_wt_mean;
	wt_var = *inout_wt_var;
	dnom = *inout_dnom;
	veclen = *inout_veclen;

	if (wt_mean == NULL) {
	    int err = FALSE;

	    /* if a gauden_counts file exists, it will have reestimated means */

	    *inout_wt_mean = wt_mean = in_wt_mean;
	    *inout_dnom = dnom = in_dnom;
	    *inout_n_mgau = n_mgau;
	    *inout_n_stream = n_stream;
	    *inout_n_density = n_density;
	    *inout_veclen = in_veclen;
	    *inout_pass2var = pass2var;

	    if (wt_var == NULL && in_wt_var != NULL) {
		*inout_wt_var = wt_var = in_wt_var;
	    }
	}
	else {
	    int err = FALSE;
	    
	    /* check if matrices are able to be added */
	    if (*inout_n_mgau != n_mgau) {
		E_ERROR("# mix. Gau. for file %s (== %u) != prior # mix. Gau. (== %u)\n",
			fn, n_mgau, *inout_n_mgau);
		err = TRUE;
	    }

	    if (*inout_n_stream != n_stream) {
		E_ERROR("# stream for file %s (== %u) != prior # stream (== %u)\n",
			fn, n_stream, *inout_n_stream);
		err = TRUE;
	    }

	    if (*inout_n_density != n_density) {
		E_ERROR("# density comp/mix for file %s (== %u) != prior # density, %u\n",
			fn, n_density, *inout_n_density);
		err = TRUE;
	    }

	    if (*inout_pass2var != pass2var) {
		E_ERROR("2 pass var %s in %s, but %s in others.\n",
			fn, (pass2var ? "true" : "false"),
			(*inout_pass2var ? "true" : "false"));
		err = TRUE;
	    }

	    for (i = 0; i < n_stream; i++) {
		if (veclen[i] != in_veclen[i]) {
		    E_ERROR("vector length of stream %u (== %u) != prior length (== %u)\n",
			    i, in_veclen[i], veclen[i]);
		    err = TRUE;
		}
	    }

	    ckd_free((void *)in_veclen);
	    
	    if (err)
		return S3_ERROR;

	    /* accumulate values */
	    accum_3d(dnom, in_dnom,
		     n_mgau, n_stream, n_density);

	    gauden_accum_param(wt_mean, in_wt_mean,
			       n_mgau, n_stream, n_density, veclen);
	    gauden_free_param(in_wt_mean);

	    if (wt_var) {
		assert(in_wt_var);
		
		gauden_accum_param(wt_var, in_wt_var,
				   n_mgau, n_stream, n_density, veclen);
		gauden_free_param(in_wt_var);
	    }
	}
    }
    else {
	E_ERROR("Unable to access %s\n", fn);

	return S3_ERROR;
    }

    return S3_SUCCESS;
}

static int
normalize()
{
    char file_name[MAXPATHLEN+1];
    const char *comment;
    float32 ***mixw_acc = NULL;
    float32 ***in_mixw = NULL;
    float64 s;
    int zero;
    uint32 n_mixw;
    uint32 n_stream;
    uint32 n_mllr_class;
    uint32 n_density;
    float32 ***tmat_acc = NULL;
    uint32 n_tmat;
    uint32 n_state_pm;
    uint32 i, j, k, l;
    float64 sum;
    vector_t ***in_mean = NULL;
    vector_t ***wt_mean = NULL;
    vector_t ***in_var = NULL;
    vector_t ***wt_var = NULL;
    int32 pass2var = FALSE;
    float32 ***dnom = NULL;
    uint32 n_mgau;
    uint32 n_gau_stream;
    uint32 n_gau_density;
    const uint32 *veclen = NULL;
    uint32 z, pz, pti;
    uint32 total;
    const char **accum_dir;
    const char *oaccum_dir;
    const char *in_mixw_fn;
    const char *out_mixw_fn;
    const char *out_tmat_fn;
    const char *in_mean_fn;
    const char *out_mean_fn;
    const char *in_var_fn;
    const char *out_var_fn;
    const char *out_dcount_fn;
    float32 pcntl;
    int err;
    const char *out_reg_fn;
    uint32 mllr_mult;
    uint32 mllr_add;
    float32 *****regl = NULL;
    float32 ****regr = NULL;
    float32 ****A;
    float32 ***B; 

    out_reg_fn = (const char *)cmd_ln_access("-regmatfn");
    
    accum_dir = (const char **)cmd_ln_access("-accumdir");
    oaccum_dir = (const char *)cmd_ln_access("-oaccumdir");

    out_mixw_fn = (const char *)cmd_ln_access("-mixwfn");
    out_tmat_fn = (const char *)cmd_ln_access("-tmatfn");
    out_mean_fn = (const char *)cmd_ln_access("-meanfn");
    out_var_fn = (const char *)cmd_ln_access("-varfn");
    in_mixw_fn = (const char *)cmd_ln_access("-inmixwfn");
    in_mean_fn = (const char *)cmd_ln_access("-inmeanfn");
    in_var_fn = (const char *)cmd_ln_access("-invarfn");
    out_dcount_fn = (const char *)cmd_ln_access("-dcountfn");

    /* must be at least one accum dir */
    assert(accum_dir[0] != NULL);

    if (out_mixw_fn == NULL) {
	E_INFO("No -mixwfn specified, will skip if any\n");
    }
    if (out_tmat_fn == NULL) {
	E_INFO("No -tmatfn specified, will skip if any\n");
    }
    if (out_mean_fn == NULL) {
	E_INFO("No -meanfn specified, will skip if any\n");
    }
    if (out_var_fn == NULL) {
	E_INFO("No -varfn specified, will skip if any\n");
    }
    if (in_mixw_fn != NULL) {
	E_INFO("Selecting unseen mixing weight parameters from %s\n",
	       in_mixw_fn);
    }
    if (in_mean_fn != NULL) {
	E_INFO("Selecting unseen density mean parameters from %s\n",
	       in_mean_fn);
    }
    if (in_var_fn != NULL) {
	E_INFO("Selecting unseen density variance parameters from %s\n",
	       in_var_fn);
    }

    n_stream = 0;
    for (i = 0; accum_dir[i]; i++) {
	E_INFO("Reading and accumulating counts from %s\n", accum_dir[i]);

	if (out_mixw_fn) {
	    rdacc_mixw(accum_dir[i],
		       &mixw_acc, &n_mixw, &n_stream, &n_density);
	}

	if (out_tmat_fn) {
	    rdacc_tmat(accum_dir[i],
		       &tmat_acc, &n_tmat, &n_state_pm);
	}

        if (out_reg_fn) {
            if (regmat_read(accum_dir[i], &regl, &regr, &veclen, &n_mllr_class, &n_stream, &mllr_mult, &mllr_add) != S3_SUCCESS) {
                E_FATAL_SYSTEM("Couldn't read MLLR accumulator in %s", accum_dir[i]);
            } 
        }
	if (out_mean_fn || out_var_fn) {
	    /* NB: Assuming in_mean and in_var are consistent */
	    if (in_mean_fn) {
		if (s3gau_read(in_mean_fn,
			       &in_mean,
			       &n_mgau,
			       &n_gau_stream,
			       &n_gau_density,
			       &veclen) != S3_SUCCESS) {
		    E_FATAL_SYSTEM("Couldn't read %s", in_mean_fn);
		}
		ckd_free((void *)veclen);
	    }

	    if (in_var_fn) {
		if (s3gau_read(in_var_fn,
			       &in_var,
			       &n_mgau,
			       &n_gau_stream,
			       &n_gau_density,
			       &veclen) != S3_SUCCESS) {
		    E_FATAL_SYSTEM("Couldn't read %s", in_var_fn);
		}
		ckd_free((void *)veclen);
	    }

	    rdacc_den(accum_dir[i],
		      &wt_mean,
		      &wt_var,
		      &pass2var,
		      &dnom,
		      &n_mgau,
		      &n_gau_stream,
		      &n_gau_density,
		      &veclen);

	    if (out_mixw_fn) {
		int err = FALSE;

		if (n_stream != n_gau_stream) {
		    E_ERROR("mixw inconsistent w/ densities WRT # "
			    "streams (%u != %u)\n",
			    n_stream, n_gau_stream);
		}

		if (n_density != n_gau_density) {
		    E_ERROR("mixw inconsistent w/ densities WRT # "
			    "den/mix (%u != %u)\n",
			    n_density, n_gau_density);
		}
	    }
	    else {
		n_stream = n_gau_stream;
		n_density = n_gau_density;
	    }
	}
    }

    if (oaccum_dir && mixw_acc) {
	/* write the total mixing weight reest. accumulators */

	err = 0;
	sprintf(file_name, "%s/mixw_counts", oaccum_dir);

	if (in_mixw_fn) {
	    if (s3mixw_read(in_mixw_fn,
			    &in_mixw,
			    &i,
			    &j,
			    &k) != S3_SUCCESS) {
		E_FATAL_SYSTEM("Unable to read %s", in_mixw_fn);
	    }
	    if (i != n_mixw) {
		E_FATAL("# mixw in input mixw file != # mixw in output mixw file\n");
	    }
	    if (j != n_stream) {
		E_FATAL("# stream in input mixw file != # stream in output mixw file\n");
	    }
	    if (k != n_density) {
		E_FATAL("# density in input mixw file != # density in output mixw file\n");
	    }
	    
	    for (i = 0; i < n_mixw; i++) {
		for (j = 0; j < n_stream; j++) {
		    for (k = 0, s = 0; k < n_density; k++) {
			s += mixw_acc[i][j][k];
		    }
		    if ((s == 0) && in_mixw) {
			for (k = 0, s = 0; k < n_density; k++) {
			    mixw_acc[i][j][k] = in_mixw[i][j][k];
			}
			E_INFO("set mixw %u stream %u to input mixw value\n", i, j);
		    }
		}
	    }
	}

	do {
	    /* Write out the accumulated reestimation sums */
	    if (s3mixw_write(file_name,
			     mixw_acc,
			     n_mixw,
			     n_stream,
			     n_density) != S3_SUCCESS) {
		if (err == 0) {
		    E_ERROR("Unable to write %s; Retrying...\n", file_name);
		}
		++err;
		sleep(600);
	    }
	} while (err > 1);
    }

    if (oaccum_dir && (wt_mean || wt_var)) {
	/* write the total mixing Gau. den reest. accumulators */

	err = 0;
	sprintf(file_name, "%s/gauden_counts", oaccum_dir);
	do {
	    if (s3gaucnt_write(file_name,
			       wt_mean,
			       wt_var,
			       pass2var,
			       dnom,
			       n_mgau,
			       n_gau_stream,
			       n_gau_density,
			       veclen) != S3_SUCCESS) {
		if (err == 0) {
		    E_ERROR("Unable to write %s; Retrying...\n", file_name);
		}
		++err;
		sleep(600);
	    }
	} while (err > 1);
    }

    if (oaccum_dir && tmat_acc) {
	/* write the total transition matrix reest. accumulators */

	err = 0;
	sprintf(file_name, "%s/tmat_counts", oaccum_dir);
	do {
	    if (s3tmat_write(file_name,
			     tmat_acc,
			     n_tmat,
			     n_state_pm) != S3_SUCCESS) {
		if (err == 0) {
		    E_ERROR("Unable to write %s; Retrying...\n", file_name);
		}
		++err;
		sleep(600);
	    }
	} while (err > 1);
    }

    if (oaccum_dir && regr && regl) {
	/* write the total MLLR regression matrix accumulators */

	err = 0;
	sprintf(file_name, "%s/regmat_counts", oaccum_dir);
	do {
	    if (s3regmatcnt_write(file_name,
				  regr,
				  regl,
				  n_mllr_class,
				  n_stream,
				  veclen,
				  mllr_mult,
				  mllr_add) != S3_SUCCESS) {
		if (err == 0) {
		    E_ERROR("Unable to write %s; Retrying...\n", file_name);
		}
		++err;
		sleep(600);
	    }
	} while (err > 1);
    }

    if (wt_mean || wt_var) {
	if (out_mean_fn) {
	    E_INFO("Normalizing mean for n_mgau= %u, n_stream= %u, n_density= %u\n",
		   n_mgau, n_stream, n_density);

	    gauden_norm_wt_mean(in_mean, wt_mean, dnom,
				n_mgau, n_stream, n_density, veclen);
	}
	else {
	    if (wt_mean) {
		E_INFO("Ignoring means since -meanfn not specified\n");
	    }
	}

	if (out_var_fn) {
	    if (wt_var) {
		E_INFO("Normalizing var\n");
		gauden_norm_wt_var(in_var, wt_var, pass2var, dnom,
				   wt_mean,	/* wt_mean now just mean */
				   n_mgau, n_stream, n_density, veclen);
	    }
	}
	else {
	    if (wt_var) {
		E_INFO("Ignoring variances since -varfn not specified\n");
	    }
	}
    }
    else {
	E_INFO("No means or variances to normalize\n");
    }

    /*
     * Write the parameters to files
     */

    /************ Added by VIPUL and BIXA on July 11th 1996 *******/
    if (out_reg_fn) {
        if(compute_mllr(regl,regr,veclen,n_mllr_class,n_stream,mllr_mult,mllr_add,&A,&B) != S3_SUCCESS) { 
            E_FATAL_SYSTEM("MLLR computation failed\n");
        }
    }
   /*********End of addition by VIPUL ************************/

    if (out_mixw_fn) {
	if (mixw_acc) {
	    if (s3mixw_write(out_mixw_fn,
			     mixw_acc,
			     n_mixw,
			     n_stream,
			     n_density) != S3_SUCCESS) {
		return S3_ERROR;
	    }
	}
	else {
	    E_WARN("NO mixing weight accumulators seen, but -mixwfn specified.\n");
	}
    }
    else {
	if (mixw_acc) {
	    E_INFO("Mixing weight accumulators seen, but -mixwfn NOT specified.\n");
	}
    }

    /************ Added by VIPUL and BHIKSHA on July 11th 1996 *******/
    if (out_reg_fn) {
        if(store_reg_mat(out_reg_fn,veclen,n_mllr_class,n_stream,A,B) != S3_SUCCESS) {
            return S3_ERROR;
        }
    }
    /*********End of addition by VIPUL ************************/

    if (out_tmat_fn) {
	if (tmat_acc) {
	    if (s3tmat_write(out_tmat_fn,
			     tmat_acc,
			     n_tmat,
			     n_state_pm) != S3_SUCCESS) {
		return S3_ERROR;
	    }
	}
	else {
	    E_WARN("NO transition matrix accumulators seen, but -tmatfn specified.\n");
	}
    }
    else {
	if (tmat_acc) 
	    E_INFO("Transition matrix accumulators seen, but -tmatfn NOT specified\n");
    }

    
    if (out_mean_fn) {
	if (wt_mean) {
	    if (s3gau_write(out_mean_fn,
			    (const vector_t ***)wt_mean,
			    n_mgau,
			    n_stream,
			    n_density,
			    veclen) != S3_SUCCESS)
		return S3_ERROR;
	    
	    if (out_dcount_fn) {
		if (s3gaudnom_write(out_dcount_fn,
				    dnom,
				    n_mgau,
				    n_stream,
				    n_density) != S3_SUCCESS)
		    return S3_ERROR;
	    }
	}
	else
	    E_WARN("NO reestimated means seen, but -meanfn specified\n");
    }
    else {
	if (wt_mean) {
	    E_INFO("Reestimated means seen, but -meanfn NOT specified\n");
	}
    }
    
    if (out_var_fn) {
	if (wt_var) {
	    if (s3gau_write(out_var_fn,
			    (const vector_t ***)wt_var,
			    n_mgau,
			    n_stream,
			    n_density,
			    veclen) != S3_SUCCESS)
		return S3_ERROR;
	}
	else
	    E_WARN("NO reestimated variances seen, but -varfn specified\n");
    }
    else {
	if (wt_var) {
	    E_INFO("Reestimated variances seen, but -varfn NOT specified\n");
	}
    }

    if (veclen)
	ckd_free((void *)veclen);

    return S3_SUCCESS;
}

main(int argc, char *argv[])
{
    if (initialize(argc, argv) != S3_SUCCESS) {
	E_FATAL("errors initializing.\n");
    }

    if (normalize() != S3_SUCCESS) {
	E_FATAL("errors normalizing.\n");
    }

    exit(0);
}

/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	main.c,v $
 * Revision 1.16  97/07/16  11:22:03  eht
 * Some changes needed by reorganization of library routines
 * 
 * Revision 1.15  97/03/07  08:46:26  eht
 * - deal w/ new i/o routines
 * - deal w/ multi-class MLLR changes
 * - fix seg fault bug when not reestimating means/vars
 * 
 * Revision 1.14  1996/09/12  18:13:58  eht
 * Bixa & Vipul MLLR changes
 *
 * Revision 1.12  1996/03/25  15:25:06  eht
 * Deal w/ varying input feature (e.g. MFCC) vector length
 *
 * Revision 1.11  1996/01/26  17:40:51  eht
 * Use new feature module
 * Have gauden() either store the current veclen or get it from
 * feat module.
 *
 * Revision 1.10  1995/12/15  18:37:07  eht
 * Added some type cases for memory alloc/free
 *
 * Revision 1.9  1995/12/14  20:15:43  eht
 * Made info output prose a little more clear.
 *
 * Revision 1.8  1995/11/30  21:03:11  eht
 * Added warning messages if command line says to write a parameter
 * file, but no corresponding count file existed.
 *
 * Revision 1.7  1995/11/16  21:15:31  eht
 * Fixed some bugs for case when no transition matrix
 * counts are gathered
 *
 * Revision 1.6  1995/10/12  18:28:28  eht
 * Get log.h from <s2/log.h>
 *
 * Revision 1.5  1995/10/10  13:09:40  eht
 * Changed to use <s3/prim_type.h>
 *
 * Revision 1.4  1995/10/09  15:29:21  eht
 * Removed __FILE__, __LINE__ arguments to ckd_alloc stuff
 *
 * Revision 1.3  1995/08/09  20:36:29  eht
 * Export normalization of gaussian density to gauden.c
 *
 * Revision 1.2  1995/06/20  12:05:19  eht
 * Removed output of dnom
 *
 * Revision 1.1  1995/06/02  20:35:38  eht
 * Initial revision
 *
 *
 */
