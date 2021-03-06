/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/inc_comp/RCS/main.c,v 1.5 97/07/16 11:36:22 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1994 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: main.c
 * 
 * Description: 
 * 	For every tied mixture, create a new component density.
 *
 * Background:
 *
 * Author: 
 * 	Eric H. Thayer (eht@cs.cmu.edu)
 *********************************************************************/

static char rcsid[] = "@(#)$Id: main.c,v 1.5 97/07/16 11:36:22 eht Exp $";

#include "parse_cmd_ln.h"
#include "accum_wt_param.h"
#include "inc_densities.h"

#include <s3/vector.h>
#include <s3/gauden.h>
#include <s3/s3gau_io.h>
#include <s3/s3mixw_io.h>
#include <s3/feat.h>
#include <s3/ckd_alloc.h>
#include <s3/cmd_ln.h>
#include <s3/s3.h>

main(int argc, char *argv[])
{
    vector_t ***mean;
    vector_t ***var;
    vector_t ***new_mean;
    vector_t ***new_var;
    float32  ***dnom;
    float32 ***mixw;
    float32 ***new_mixw;
    uint32 n_mixw;
    uint32 n_mgau;
    uint32 n_feat;
    uint32 feat_veclen;
    uint32 t_n_feat;
    uint32 n_density;
    uint32 t_n_density;
    uint32 n_inc;
    int32 pass2var;
    const uint32 *veclen;

    parse_cmd_ln(argc, argv);

    if (cmd_ln_access("-feat") != NULL) {
	feat_set((const char *)cmd_ln_access("-feat"));
    }
    else {
	E_FATAL("You must specify the acoustic feature set using -feat\n");
    }
    feat_veclen = *(int32 *)cmd_ln_access("-ceplen");
    feat_set_in_veclen(feat_veclen);

    E_INFO("Reading mixing weight file %s.\n",
	   cmd_ln_access("-inmixwfn"));

    if (s3mixw_read((const char *)cmd_ln_access("-inmixwfn"),
		    &mixw,
		    &n_mixw,
		    &n_feat,
		    &n_density) != S3_SUCCESS) {
	exit(1);
    }

    n_inc = *(int32 *)cmd_ln_access("-ninc");

    if (n_inc > n_density) {
	E_WARN("# of densities to split (== %u) > total # of densities/mixture (== %u); # split <- %u # den/mix\n",
	       n_inc, n_density, n_density);
	n_inc = n_density;
    }
    
    if (s3gau_read((const char *)cmd_ln_access("-inmeanfn"),
		   &mean,
		   &n_mgau,
		   &n_feat,
		   &n_density,
		   &veclen) != S3_SUCCESS) {
	exit(1);
    }

    if (s3gau_read((const char *)cmd_ln_access("-invarfn"),
		   &var,
		   &n_mgau,
		   &n_feat,
		   &n_density,
		   &veclen) != S3_SUCCESS) {
	exit(1);
    }

    if (s3gaudnom_read((const char *)cmd_ln_access("-dcountfn"),
		       &dnom,
		       &n_mgau,
		       &n_feat,
		       &n_density) != S3_SUCCESS) {
	exit(1);
    }
	
    new_mean = gauden_alloc_param(n_mgau, n_feat, n_density+n_inc, veclen);
    new_var  = gauden_alloc_param(n_mgau, n_feat, n_density+n_inc, veclen);
    new_mixw = (float32 ***)ckd_calloc_3d(n_mixw, n_feat, n_density+n_inc,
					  sizeof(float32));
    
    E_INFO("output n_density == %u\n", n_density+n_inc);

    inc_densities(new_mixw,
		  new_mean,
		  new_var,

		  mixw,
		  mean,
		  var,
		  dnom,

		  n_mixw,
		  n_mgau,
		  n_feat,
		  n_density,

		  n_inc);

    if (cmd_ln_access("-outmixwfn") != NULL) {
	if (s3mixw_write((char *)cmd_ln_access("-outmixwfn"),
			 new_mixw,
			 n_mixw,
			 n_feat,
			 n_density+n_inc) != S3_SUCCESS) {
	    exit(1);
	}
    }
    else {
	E_FATAL("You must use the -outmixwfn argument\n");
    }

    if (cmd_ln_access("-outmeanfn") != NULL) {
	if (s3gau_write((const char *)cmd_ln_access("-outmeanfn"),
			(const vector_t ***)new_mean,
			n_mgau,
			n_feat,
			n_density+n_inc,
			feat_vecsize()) != S3_SUCCESS) {
	    exit(1);
	}
    }
    else {
	E_FATAL("You must use the -outmeanfn argument\n");
    }

    if (cmd_ln_access("-outvarfn") != NULL) {
	if (s3gau_write((const char *)cmd_ln_access("-outvarfn"),
			(const vector_t ***)new_var,
			n_mgau,
			n_feat,
			n_density+n_inc,
			feat_vecsize()) != S3_SUCCESS) {
	    exit(1);
	}
    }
    else {
	E_FATAL("You must use the -outvarfn argument\n");
    }

    exit(0);
}

/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	main.c,v $
 * Revision 1.5  97/07/16  11:36:22  eht
 * *** empty log message ***
 * 
 * Revision 1.4  1996/08/06  14:07:14  eht
 * Deal w/ 2 pass variance computation
 *
 * Revision 1.3  1996/07/29  16:26:54  eht
 * development release
 *
 * Revision 1.2  1996/01/26  18:22:55  eht
 * Development version
 *
 * Revision 1.1  1995/12/15  18:37:07  eht
 * Initial revision
 *
 *
 */
