/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/init_gau/RCS/main.c,v 1.9 97/07/16 11:36:22 eht Exp Locker: eht $
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
 *	This routine uses a model definition file, a state parameter
 *	definition file and a control file to create a set of initial
 *	means and variance parameters for 1 density mixture Gaussian
 *	context independent system.
 * 
 * Author: 
 *	Eric H. Thayer (eht@cs.cmu.edu)
 * 
 *********************************************************************/

static char rcsid[] = "@(#)$Id: main.c,v 1.9 97/07/16 11:36:22 eht Exp Locker: eht $";

#include "init_gau.h"
#include "parse_cmd_ln.h"

#include <s3/corpus.h>
#include <s3/cmd_ln.h>
#include <s3/lexicon.h>
#include <s3/acmod_set.h>
#include <s3/model_def_io.h>
#include <s3/s3ts2cb_io.h>
#include <s3/ts2cb.h>
#include <s3/ckd_alloc.h>
#include <s3/feat.h>
#include <s3/s3.h>

#include <sys_compat/file.h>

#include <string.h>

static int
initialize(lexicon_t **out_lex,
	   model_def_t **out_mdef,
	   int argc,
	   char *argv[])
{
    int ret;
    lexicon_t *lex = NULL;
    model_def_t *mdef = NULL;
    uint32 i;
    const char *fdictfn;
    const char *dictfn;
    const char *ts2cbfn;
    uint32 n_ts;
    uint32 n_cb;

    /* define, parse and (partially) validate the command line */
    parse_cmd_ln(argc, argv);

    if (cmd_ln_access("-feat") != NULL) {
	feat_set((const char *)cmd_ln_access("-feat"));
	feat_set_in_veclen(*(int32 *)cmd_ln_access("-ceplen"));
    }
    else {
	E_ERROR("Specify the feature extraction algorithm using -feat\n");

	return S3_ERROR;
    }

    if (cmd_ln_access("-segdir"))
	corpus_set_seg_dir(cmd_ln_access("-segdir"));
    if (cmd_ln_access("-segext"))
	corpus_set_seg_ext(cmd_ln_access("-segext"));

    corpus_set_mfcc_dir(cmd_ln_access("-cepdir"));
    corpus_set_mfcc_ext(cmd_ln_access("-cepext"));

    if (cmd_ln_access("-lsnfn"))
	corpus_set_lsn_filename(cmd_ln_access("-lsnfn"));

    corpus_set_ctl_filename(cmd_ln_access("-ctlfn"));
    
    if ( cmd_ln_access("-nskip") && cmd_ln_access("-runlen") ) {
	corpus_set_interval(*(int32 *)cmd_ln_access("-nskip"),
			    *(int32 *)cmd_ln_access("-runlen"));
    }
    else if (cmd_ln_access("-part") && cmd_ln_access("-npart")) {
	corpus_set_partition(*(uint32 *)cmd_ln_access("-part"),
			     *(uint32 *)cmd_ln_access("-npart"));
    }
    

    if (corpus_init() != S3_SUCCESS) {
	return S3_ERROR;
    }
    
    if (cmd_ln_access("-moddeffn")) {
	E_INFO("Reading %s\n", cmd_ln_access("-moddeffn"));
    
	/* Read in the model definitions.  Defines the set of
	   CI phones and context dependent phones.  Defines the
	   transition matrix tying and state level tying. */
	if (model_def_read(&mdef,
			   cmd_ln_access("-moddeffn")) != S3_SUCCESS) {
	    return S3_ERROR;
	}
	
	ts2cbfn = (const char *)cmd_ln_access("-ts2cbfn");
	if (strcmp(SEMI_LABEL, ts2cbfn) == 0) {
	    mdef->cb = semi_ts2cb(mdef->n_tied_state);
	    n_ts = mdef->n_tied_state;
	    n_cb = 1;
	}
	else if (strcmp(CONT_LABEL, ts2cbfn) == 0) {
	    mdef->cb = cont_ts2cb(mdef->n_tied_state);
	    n_ts = mdef->n_tied_state;
	    n_cb = mdef->n_tied_state;
	}
	else if (s3ts2cb_read(ts2cbfn,
			      &mdef->cb,
			      &n_ts,
			      &n_cb) != S3_SUCCESS) {
	    return S3_ERROR;
	}

	dictfn = cmd_ln_access("-dictfn");

	if (dictfn == NULL) {
	    E_FATAL("You must specify a content dictionary using -dictfn\n");
	}

	E_INFO("Reading %s\n", dictfn);
	
	lex = lexicon_read(NULL,	/* no lexicon to start */
			   dictfn,
			   mdef->acmod_set);
	if (lex == NULL)
	    return S3_ERROR;
    
	fdictfn = cmd_ln_access("-fdictfn");

	if (fdictfn) {
	    E_INFO("Reading %s\n", fdictfn);
	    
	    (void)lexicon_read(lex,	/* add filler words content lexicon */
			       fdictfn,
			       mdef->acmod_set);
	}
    }

    *out_mdef = mdef;
    *out_lex = lex;

    return S3_SUCCESS;
}

main(int argc, char *argv[])
{
    lexicon_t *lex;
    model_def_t *mdef;

    if (initialize(&lex, &mdef,
		   argc, argv) != S3_SUCCESS) {
	E_ERROR("errors initializing.\n");
	exit(1);
    }
    
    if (init_gau(lex, mdef) != S3_SUCCESS) {
	exit(1);
    }

    exit(0);
}

/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	main.c,v $
 * Revision 1.9  97/07/16  11:36:22  eht
 * *** empty log message ***
 * 
 * Revision 1.8  96/08/06  14:15:15  eht
 * Define missing prototype
 * 
 * Revision 1.7  1996/08/06  14:08:28  eht
 * Deal w/ new model definition structure which includes an acoustic model set
 * mapping structure
 *
 * Revision 1.6  1996/03/25  15:43:43  eht
 * Deal w setting the input feature vector length
 * Deal w/ -nskip and -runlen arguments
 *
 * Revision 1.5  1996/01/26  18:22:07  eht
 * Use the feat module.
 *
 * Revision 1.4  1995/12/14  20:00:17  eht
 * Added (void *) type cast for ckd_free() argument.
 *
 * Revision 1.3  1995/12/14  19:57:44  eht
 * Yet another interim development version
 *
 * Revision 1.2  1995/12/01  20:55:40  eht
 * interim development version
 *
 * Revision 1.1  1995/12/01  16:39:04  eht
 * Initial revision
 *
 *
 */
