/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libs2io/RCS/s2_write_cb.c,v 1.8 97/07/16 11:36:22 eht Exp $
 *
 * Carnegie Mellon Speech Group
 *
 * Copyright (c) 1995 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: s2_write_cb.c
 * 
 * Description: 
 * 
 * Author: 
 * 	Eric H. Thayer (eht@cs.cmu.edu)
 *********************************************************************/

#include <s3/s2_write_cb.h>

#include <s3/s2_param.h>
#include <s3/s2io.h>
#include <s3/err.h>
#include <s3/s3.h>

#include <s3/common.h>

#include <sys_compat/file.h>

#include <stdio.h>
#include <assert.h>
#include <string.h>

static char rcsid[] = "@(#)$Id: s2_write_cb.c,v 1.8 97/07/16 11:36:22 eht Exp $";

static uint32 cb_vlen[S2_N_FEATURE] = {
    S2_CEPF_VECLEN+1,
    S2_DCEPF_VECLEN+1,
    S2_POWF_VECLEN,
    S2_2DCEPF_VECLEN+1
};

static uint32 f_vlen[S2_N_FEATURE] = {
    S2_CEPF_VECLEN,
    S2_DCEPF_VECLEN,
    S2_POWF_VECLEN,
    S2_2DCEPF_VECLEN
};

static int screwball_offset[S2_N_FEATURE] = {
    1, 1, 0, 1
};

int
s2_write_cb(gauden_t *g,
	    const char *out_dir_name,
	    const char **cb_base,
	    const char *mean_ext,
	    const char *var_ext,
	    int do_floor)
{
    vector_t ***mean;
    vector_t ***var;
    float32 *raw;
    uint32 i, j, f, s;
    char cb_filename[MAXPATHLEN+1];

    if (!g) {
	E_FATAL("NULL pointer to new format gaussian.\n");
    }

    if (g->n_feat != S2_N_FEATURE) {
	E_ERROR("New format gaussian has wrong num features (%d should be %d).\n", g->n_feat, S2_N_FEATURE);
	return S3_ERROR;
    }

    for (i = 0; i < S2_N_FEATURE; i++) {
	if (!g->veclen || (g->veclen[i] != f_vlen[i])) {

	    fprintf(stderr, "%s(%d): New format gaussian feature %d wrong length (%d should be %d).\n", __FILE__, __LINE__, i, g->veclen[i], f_vlen[i]);
	    return S3_ERROR;
	}
    }

    mean = gauden_mean(g);
    var = gauden_var(g);

#if 0
    fflush(stdout);
    fprintf(stderr, "%s(%d): constant power variance\n",
	    __FILE__, __LINE__);
    fflush(stderr);

    /* power variance is a constant for S2 */
    for (i = 0; i < S2_N_CODEWORD; i++) {
	for (j = 0; j < f_vlen[S2_POW_FEATURE]; j++) {
	    var[0][S2_POW_FEATURE][i][j] = pv[j];
	}
    }
#endif /* COMMENT */

    fflush(stdout);
    fprintf(stderr, "%s(%d): writing gau den to (",
	    __FILE__, __LINE__);

    for (s = 0; s < 1; s++) {	/* PUT NUMBER OF PHONE SETS HERRE!!! */
        for (f = 0; f < S2_N_FEATURE; f++) {
	    sprintf(cb_filename, "%s/%s.%s",
		    out_dir_name, cb_base[f], mean_ext);

	    fprintf(stderr, "\n\t%s ", cb_filename);
	    fflush(stderr);

	    /* first the means */

	    raw = ckd_calloc(S2_N_CODEWORD * cb_vlen[f], sizeof(float32));

	    for (j = 0; j < S2_N_CODEWORD; j++) {
		memcpy(&raw[(j * cb_vlen[f]) + screwball_offset[f]],
		       mean[s][f][j],
		       f_vlen[f] * sizeof(float32));
	    }

	    if (awritefloat(cb_filename, raw, (S2_N_CODEWORD * cb_vlen[f]))
		< 0)
	        E_FATAL("could not write to %s\n", cb_filename);

	    ckd_free(raw);
	    raw = NULL;

	    /* then the (diagonal co-)variances */

	    if (var) {
		sprintf(cb_filename, "%s/%s.%s",
			out_dir_name, cb_base[f], var_ext);

		fprintf(stderr, "%s", cb_filename);
		fflush(stderr);

		raw = ckd_calloc(S2_N_CODEWORD * cb_vlen[f], sizeof(float32));
	    
		for (j = 0; j < S2_N_CODEWORD; j++) {
		    memcpy(&raw[(j * cb_vlen[f]) + screwball_offset[f]],
			   var[s][f][j],
			   f_vlen[f] * sizeof(float32));
		}

		if (awritefloat(cb_filename, raw, (S2_N_CODEWORD * cb_vlen[f]))
		    < 0)
		    E_FATAL("could not write to %s\n", cb_filename);
		
		ckd_free(raw);
		raw = NULL;
	    }
	}			/* end for f (feature) */
    }				/* end for cb_set */

    return S3_SUCCESS;
}

/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	s2_write_cb.c,v $
 * Revision 1.8  97/07/16  11:36:22  eht
 * *** empty log message ***
 * 
 * Revision 1.7  1996/01/23  18:12:42  eht
 * Changes to remove either:
 * 	unused local variables
 * 	broken printf() format specifications
 * 	missing function prototypes in header files
 *
 * Revision 1.6  1995/11/30  21:01:38  eht
 * Get rid of some debugging printfs
 * Also deal with not outputting var's if they are not available
 *
 * Revision 1.5  1995/10/17  14:03:23  eht
 * Changed to port to Windows NT
 *
 * Revision 1.4  1995/10/12  17:42:40  eht
 * Get SPHINX-II header files from <s2/...>
 *
 * Revision 1.3  1995/10/10  12:27:37  eht
 * Changed to use <s3/prim_type.h>
 *
 * Revision 1.2  1995/10/09  15:08:24  eht
 * changed ckd_alloc interface to remove need for __FILE__, __LINE__
 * arguments
 *
 * Revision 1.1  1995/09/07  19:25:49  eht
 * Initial revision
 *
 *
 */
