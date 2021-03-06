/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libs2io/RCS/s2_write_hmm.c,v 1.7 97/07/16 11:36:22 eht Exp $
 *
 * Carnegie Mellon Speech Group
 *
 * Copyright (c) 1995 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: s2_write_tmat.c
 * 
 * Description: 
 * 
 * Author: 
 * 	Eric H. Thayer (eht@cs.cmu.edu)
 *********************************************************************/

/* SPHINX-III header files */
#include <s3/s2_write_hmm.h>
#include <s3/s2_param.h>
#include <s3/cmd_ln.h>
#include <s3/vector.h>
#include <s3/ckd_alloc.h>
#include <s3/err.h>
#include <s3/s3.h>

/* Header files imported from SPHINX-II */
#include <s2/log.h>
#include <s2/magic.h>
#include <s2/byteorder.h>

/* SPHINX-III system portability */
#include <sys_compat/file.h>

/* System-wide header files */
#include <assert.h>
#include <stdio.h>

static char rcsid[] = "@(#)$Id: s2_write_hmm.c,v 1.7 97/07/16 11:36:22 eht Exp $";

static int
fwrite_long(FILE *fp, int32 i)
{
    SWAPL(&i);
    if (fwrite (&i, sizeof(int32), 1, fp) != 1)
	return S3_ERROR;
    else
	return S3_SUCCESS;
}

static int
write_transition(int32 from, int32 to, int32 prob, int32 opdf_index, FILE *fp)
{
    fwrite_long (fp, from);
    fwrite_long (fp, to);
    fwrite_long (fp, prob);
    fwrite_long (fp, opdf_index);

    return S3_SUCCESS;
}

static int
write_sdm(float32 **tmat, FILE *fp)
{
    int32 n_arc;
    

    fwrite_long (fp, TIED_DIST);
				/* what type of file it is (trans only) */
				/* might also be BIG_HMM */

    fwrite_long (fp, S2_N_CODEWORD);	/* number of codewords??? */

    fwrite_long (fp, S2_N_STATE-1);	/* n_omatrix */


    /*
     * Output pdfs would go here, but instead go in *.{ccode,xcode,...}
     */

    fwrite_long (fp, S2_N_STATE);	/* n_states */
    fwrite_long (fp, 1);			/* n_initial (states) */
    fwrite_long (fp, 0);			/* list of initial states {0} */
    fwrite_long (fp, 1);			/* n_final */
    fwrite_long (fp, (S2_N_STATE-1));  /* list of final states {S2_N_STATE-1} */

    n_arc = 14;
    fwrite_long (fp, n_arc);		/* number of arcs */

    /*
     * Dump these in the same order as s2 did.
     */
    write_transition (0, 0, LOG(tmat[0][0]), 0, fp);
    write_transition (0, 1, LOG(tmat[0][1]), 0, fp);
    write_transition (1, 1, LOG(tmat[1][1]), 1, fp);
    write_transition (1, 2, LOG(tmat[1][2]), 1, fp);
    write_transition (2, 2, LOG(tmat[2][2]), 2, fp);
    write_transition (2, 3, LOG(tmat[2][3]), 2, fp);
    write_transition (3, 3, LOG(tmat[3][3]), 3, fp);
    write_transition (3, 4, LOG(tmat[3][4]), 3, fp);
    write_transition (4, 4, LOG(tmat[4][4]), 4, fp);
    write_transition (4, 5, LOG(tmat[4][5]), 4, fp);
    write_transition (0, 2, LOG(tmat[0][2]), 0, fp);
    write_transition (1, 3, LOG(tmat[1][3]), 1, fp);
    write_transition (2, 4, LOG(tmat[2][4]), 2, fp);
    write_transition (3, 5, LOG(tmat[3][5]), 3, fp);

    return S3_SUCCESS;
}

static int
write_dhmm(float32 **tmat, float32 ***mixw, FILE *fp)
{
    int32 n_arc;
    uint32 i, j, k;
    int32 tmp;

    fwrite_long (fp, COUNT_F);
				/* what type of file it is (trans only) */
				/* might also be BIG_HMM */

    fwrite_long (fp, S2_N_CODEWORD);	/* number of codewords??? */

    fwrite_long (fp, S2_N_STATE-1);	/* n_omatrix */


    /*
     * Output pdfs
     */
    for (j = 0; j < 4; j++) {
	for (i = 0; i < S2_N_STATE-1; i++) {
	    for (k = 0; k < S2_N_CODEWORD; k++) {
		tmp = LOG(mixw[i][j][k]);
		fwrite_long(fp, tmp);
	    }
	}
    }

    fwrite_long (fp, S2_N_STATE);	/* n_states */
    fwrite_long (fp, 1);			/* n_initial (states) */
    fwrite_long (fp, 0);			/* list of initial states {0} */
    fwrite_long (fp, 1);			/* n_final */
    fwrite_long (fp, (S2_N_STATE-1));  /* list of final states {S2_N_STATE-1} */

    n_arc = 14;
    fwrite_long (fp, n_arc);		/* number of arcs */

    /*
     * Dump these in the same order as s2 did.
     */
    write_transition (0, 0, LOG(tmat[0][0]), 0, fp);
    write_transition (0, 1, LOG(tmat[0][1]), 0, fp);

    write_transition (1, 1, LOG(tmat[1][1]), 1, fp);
    write_transition (1, 2, LOG(tmat[1][2]), 1, fp);

    write_transition (2, 2, LOG(tmat[2][2]), 2, fp);
    write_transition (2, 3, LOG(tmat[2][3]), 2, fp);

    write_transition (3, 3, LOG(tmat[3][3]), 3, fp);
    write_transition (3, 4, LOG(tmat[3][4]), 3, fp);

    write_transition (4, 4, LOG(tmat[4][4]), 4, fp);
    write_transition (4, 5, LOG(tmat[4][5]), 4, fp);

    /* skip arcs */
    write_transition (0, 2, LOG(tmat[0][2]), 0, fp);
    write_transition (1, 3, LOG(tmat[1][3]), 1, fp);
    write_transition (2, 4, LOG(tmat[2][4]), 2, fp);
    write_transition (3, 5, LOG(tmat[3][5]), 3, fp);

    return S3_SUCCESS;
}

static int
put_dhmm(float32 **tmat,
	 float32 ***mixw,
	 const char *dir,
	 const char *name)
{
    const char *hmm_ext;
    char fn[MAXPATHLEN];
    FILE *fp;

    hmm_ext = cmd_ln_access("-hmmext");

    sprintf(fn, "%s/%s.%s", dir, name, hmm_ext);
    
    fp = fopen(fn, "wb");
    if (fp == NULL) {
	E_ERROR_SYSTEM("can't open %s for writing", fn);
		       
	return S3_ERROR;
    }
    
    if (write_dhmm(tmat, mixw, fp) != S3_SUCCESS)
	return S3_ERROR;

    fclose(fp);

    return S3_SUCCESS;
}

static int
put_sdm(float32 **tmat, const char *in_dir_name, const char *ci_name)
{
    const char *hmm_ext;
    char ci_hmm_filename[MAXPATHLEN];
    FILE *fp;

    hmm_ext = cmd_ln_access("-hmmext");

    sprintf(ci_hmm_filename, "%s/%s.%s",
	    in_dir_name, ci_name, hmm_ext);
    
    fp = fopen(ci_hmm_filename, "wb");
    if (fp == NULL) {
	fflush(stdout);
	fprintf(stderr, "%s(%d): can't open %s for reading to extract tmat\n",
		__FILE__, __LINE__, ci_hmm_filename);
	fflush(stderr);

	return S3_ERROR;
    }
    
    if (write_sdm(tmat, fp) != S3_SUCCESS)
	return S3_ERROR;

    fclose(fp);

    return S3_SUCCESS;
}

static void
print_tmat(FILE *fp, float32 **tmat, int n_state)
{
    uint32 i, j;

    for (i = 0; i < n_state-1; i++) {
	for (j = 0; j < n_state; j++) {
	    fprintf(fp, "(%u %u) == %f\n", i, j, tmat[i][j]);
	}
    }

}

int
s2_write_dhmm(float32 ***tmat,
	      float32 ***mixw,
	      model_def_t *mdef,
	      const char  *dir)
{ 
    acmod_set_t *acmod_set;
    model_def_entry_t *defn;
    uint32 t, b, n_state, i, j, err;
    uint32 n_acmod;
    float32 ***omixw = NULL;

    acmod_set = mdef->acmod_set;

    n_acmod = acmod_set_n_acmod(acmod_set);

    for (i = 0; i < n_acmod; i++) {
	defn = &mdef->defn[i];
	b = (uint32)acmod_set_base_phone(acmod_set, (acmod_id_t)i);

	if (omixw) ckd_free((void *)omixw);

	n_state = defn->n_state;

	omixw = (float32 ***)ckd_calloc(n_state, sizeof(float32 **));

	for (j = 0; j < n_state-1; j++) {
	    omixw[j] = mixw[defn->state[j]];
	}
	
	if (put_dhmm(tmat[b],
		     omixw,
		     dir,
		     acmod_set_id2s2name(acmod_set, (acmod_id_t)i)) != S3_SUCCESS)
	    err = 1;
    }

    if (omixw) ckd_free((void *)omixw);
    
    if (!err) {
	return S3_SUCCESS;
    }
    else {
	return S3_ERROR;
    }
}

int
s2_write_hmm(float32 ***tmat,
	     acmod_set_t *acmod_set,
	     const char *out_dir_name)
{ 
    uint32 n_ci;
    uint32 i;
    int err;

    n_ci = acmod_set_n_ci(acmod_set);

    E_INFO("Writing %d tied CI transition matrices to %s\n",
	   n_ci, out_dir_name);

    err = 0;
    for (i = 0; i < n_ci; i++) {
	if (put_sdm(tmat[i],
		    out_dir_name,
		    acmod_set_id2name(acmod_set, i)) != S3_SUCCESS)
	    err = 1;
	
#ifdef S2_WRITE_HMM_VERBOSE
	if (!err) {
	    print_tmat(stdout, tmat[i], S2_N_STATE);
	    fflush(stdout);
	}
#endif
    }
    
    if (!err) {
	return S3_SUCCESS;
    }
    else {
	return S3_ERROR;
    }
}

/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	s2_write_hmm.c,v $
 * Revision 1.7  97/07/16  11:36:22  eht
 * *** empty log message ***
 * 
 * Revision 1.6  1996/01/23  18:12:42  eht
 * Changes to remove either:
 * 	unused local variables
 * 	broken printf() format specifications
 * 	missing function prototypes in header files
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
 * Revision 1.2  1995/09/08  19:11:14  eht
 * Updated to use new acmod_set module.  Prior to testing
 * on TI digits.
 *
 * Revision 1.1  1995/09/07  19:26:19  eht
 * Initial revision
 *
 *
 */
