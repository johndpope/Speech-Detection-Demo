/*********************************************************************
 *
 * $Header: /usr2/eht/s3/src/kmeans_init/RCS/cmd_ln.c,v 1.1 97/07/16 11:36:22 eht Exp Locker: eht $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1994 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: cmd_ln.c
 * 
 * Description: 
 *    This file defines the command line arguments for kmeans_init
 *
 * Author: 
 * 	Eric Thayer (eht@cs.cmu.edu)
 *********************************************************************/

static char rcsid[] = "@(#)$Id: cmd_ln.c,v 1.1 97/07/16 11:36:22 eht Exp Locker: eht $";

#include "cmd_ln.h"

#include <s3/cmd_ln.h>
#include <s3/err.h>

#include <sys_compat/misc.h>

int
parse_cmd_ln(int argc, char *argv[])
{
    static arg_def_t defn[] = {
	{ "-segdir",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Directory containing the state segmentations" },

	{ "-segext",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  "v8_seg",
	  "Extention of state segmentation files" },

	{ "-omoddeffn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Model definition of output models" },

	{ "-dmoddeffn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Model definition used for observation dump" },

	{ "-ts2cbfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Tied-state-to-codebook mapping file" },

	{ "-lsnfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "LSN file name (word transcripts)" },
	  
	{ "-dictfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Dictionary file name"},
	  
	{ "-fdictfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Filler word dictionary file name"},
	  
	{ "-cbcntfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "File containing # of times a codebook ID appears in the corpus" },

	{ "-maxcbobs",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Cluster at most this many observations per codebook" },

	{ "-maxtotobs",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Cluster at most approximately this many observations over all codebooks" },

	{ "-featsel",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "The feature stream ( 0, 1, ...) to select" },

	/* Defines a corpus */
	{ "-ctlfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "The training corpus control file" },

	/* Cepstrum file location and extension */
	{ "-cepext",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  "mfc",
	  "The cepstrum file extension" },
	{ "-cepdir",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "The cepstrum data root directory" },

	{ "-ceplen",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  "13",
	  "The length of the input feature (e.g. MFCC) vectors"},
	{ "-agc",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  "max",
	  "The type of automatic gain control to do {max|emax}"},
	{ "-cmn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  "current",
	  "The do cepstral mean normalization based on {current|prior} utterance(s)"},
        { "-varnorm",
          CMD_LN_BOOLEAN,
          CMD_LN_NO_VALIDATION,
          "no",
          "Normalize utterance by its variance"},

	{ "-silcomp",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  "none",
	  "Do silence compression based on {current|prior} utterance"},
	{ "-feat",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  "1s_12c_12d_3p_12dd",
	  "This argument selects the derived feature computation to use."},

	{ "-segdmpdirs",
	  CMD_LN_STRING_LIST,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "segment dmp directories"},

	{ "-segdmpfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "segment dmp file"},

	{ "-segidxfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "segment dmp index file"},

	{ "-fpcachesz",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  "3000",
	  "# of file descriptors to cache for observation dmp files" },
	
	{ "-obscachesz",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  "92",
	  "# of Mbytes cache to use for observations" },

	{ "-ndensity",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "# of densities to initialize per tied state per feature" },

	{ "-ntrial",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  "5",
	  "random initialized K-means: # of trials of k-means w/ random initialization from within corpus" },
	
	{ "-minratio",
	  CMD_LN_FLOAT32,
	  CMD_LN_NO_VALIDATION,
	  "0.01",
	  "K-means: minimum convergence ratio, (p_squerr - squerr) / p_squerr"},

	{ "-maxiter",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  "100",
	  "K-means: maximum # of iterations of updating to apply"},

	{ "-mixwfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Output file for mixing weights" },
	
	{ "-meanfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Output file for means" },

	{ "-varfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Output file for variances" },
	
	{ "-method",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  "rkm",
	  "Initialization method.  Options: rkm | fnkm" },
	
	{ "-reest",
	  CMD_LN_BOOLEAN,
	  CMD_LN_NO_VALIDATION,
	  "yes",
	  "Reestimate states according to usual definitions assuming vit seg."},
	
	{ "-niter",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  "20",
	  "# of iterations of reestimation to perform per state" },

	{ "-gthobj",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  "state",
	  "Gather what kind of obj {state, phn, frame}" },
	
	{ "-stride",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  "32",
	  "Gather every -stride'th frame" },
	
	{ "-runlen",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "# of utts to process from ctl file" },

	{ "-tsoff",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  "0",
	  "Begin tied state reestimation with this tied state" },

	{ "-tscnt",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Reestimate this many tied states (if available)" },

	{ "-tsrngfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "The range of tied states reestimated expressed as offset,count"},

	{ "-vartiethr",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  "0",		/* i.e. no variance tying based on occurrance count */
	  "Tie variances if # of observations for state exceed this number" },

	{ NULL, CMD_LN_UNDEF, CMD_LN_NO_VALIDATION, CMD_LN_NO_DEFAULT, NULL }
    };
    
    cmd_ln_define(defn);

    if (argc == 1) {
	cmd_ln_print_definitions();
	exit(1);
    }

    cmd_ln_parse(argc, argv);

    if (cmd_ln_validate() == FALSE) {
	E_FATAL("Unable to validate command line arguments\n");
    }

    cmd_ln_print_configuration();

    return 0;
}

/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	cmd_ln.c,v $
 * Revision 1.1  97/07/16  11:36:22  eht
 * Initial revision
 * 
 *
 */
