/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/bw/RCS/train_cmd_ln.c,v 1.14 97/07/16 11:36:22 eht Exp Locker: eht $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1994 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: train_cmd_ln.c
 * 
 * Description: 
 * 
 * Author: 
 * 	Eric H. Thayer (eht@cs.cmu.edu)
 *********************************************************************/

static char rcsid[] = "@(#)$Id: train_cmd_ln.c,v 1.14 97/07/16 11:36:22 eht Exp Locker: eht $";

#include "train_cmd_ln.h"

#include <s3/cmd_ln.h>
#include <s3/err.h>
#include <s3/s3.h>

#include <sys_compat/file.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <assert.h>

static int
validate_read_file(char *switch_name, void *arg)
{
    char *fn = arg;
    FILE *tmp;

    tmp = fopen(fn, "r");

    if (tmp == NULL) {
	E_WARN_SYSTEM("%s %s cannot be opened for reading\n", switch_name, arg);
	return FALSE;
    }
    else {
	fclose(tmp);
	return TRUE;
    }
}

static int
validate_optional_read_file(char *switch_name, void *arg)
{
    char *fn = arg;
    
    if (arg) return validate_read_file(switch_name, arg);
    else return TRUE;
}

static int
validate_writeable_dir(char *switch_name, void *arg)
{
#ifndef WIN32
    char *path = arg;
    struct stat s;
    uid_t me;
    gid_t my_group;

    if (path == NULL) {
	E_ERROR("%s is a necessary switch\n", switch_name);

	return FALSE;
    }

    if (stat(path, &s) < 0) {
	E_ERROR("%s %s does not exist or is inaccessible\n", switch_name, path);

	return FALSE;
    }

    if (!S_ISDIR(s.st_mode)) {
	E_ERROR("%s %s is not a directory\n", switch_name, path);

	return FALSE;
    }	

    if ((s.st_mode && S_IWOTH) ||
	((s.st_uid == getuid()) && (s.st_mode && S_IWUSR)) ||
	((s.st_gid == getgid()) && (s.st_mode && S_IWGRP))) {
	return TRUE;
    }
    else {
	E_ERROR("%s %s is not writeable\n", switch_name, path);

	return FALSE;
    }
#else
    /* WIN32 */

    /* Do no validation for now.  Need to figure out WIN32 compatible way */

    return TRUE;
#endif
}
static int
validate_opt_writeable_dir(char *switch_name, void *arg)
{
#ifndef WIN32
    char *path = arg;
    struct stat s;
    uid_t me;
    gid_t my_group;

    if (path == NULL) {
	return TRUE;
    }

    if (stat(path, &s) < 0) {
	E_ERROR("%s %s does not exist or is inaccessible\n", switch_name, path);

	return FALSE;
    }

    if (!S_ISDIR(s.st_mode)) {
	E_ERROR("%s %s is not a directory\n", switch_name, path);

	return FALSE;
    }	

    if ((s.st_mode && S_IWOTH) ||
	((s.st_uid == getuid()) && (s.st_mode && S_IWUSR)) ||
	((s.st_gid == getgid()) && (s.st_mode && S_IWGRP))) {
	return TRUE;
    }
    else {
	E_ERROR("%s %s is not writeable\n", switch_name, path);

	return FALSE;
    }
#else
    /* WIN32 */

    /* Do no validation for now.  Need to figure out WIN32 compatible way */

    return TRUE;
#endif
}

static int
validate_readable_dir(char *switch_name, void *arg)
{
#ifndef WIN32
    char *path = arg;
    struct stat s;
    uid_t me;
    gid_t my_group;

    if (path == NULL) {
	E_ERROR("%s is a necessary switch\n", switch_name);

	return FALSE;
    }

    if (stat(path, &s) < 0) {
	E_ERROR("%s %s does not exist or is inaccessible\n", switch_name, path);

	return FALSE;
    }

    if (!S_ISDIR(s.st_mode)) {
	E_ERROR("%s %s is not a directory\n", switch_name, path);

	return FALSE;
    }	

    if ((s.st_mode && S_IROTH) ||
	((s.st_uid == getuid()) && (s.st_mode && S_IRUSR)) ||
	((s.st_gid == getgid()) && (s.st_mode && S_IRGRP))) {
	return TRUE;
    }
    else {
	E_ERROR("%s %s is not readable\n", switch_name, path);

	return FALSE;
    }
#else
    /* WIN32 */

    /* Do no validation for now.  Need to figure out a WIN32 compatible
       way */

    return TRUE;
#endif
}

static int
validate_agc(char *switch_name, void *arg)
{
    if ((strcmp(arg, "max") == 0) || (strcmp(arg, "emax") == 0) || (strcmp(arg, "none") == 0)) {
	return TRUE;
    }
    else {
	E_ERROR("Unknown agc type %s %s\n", switch_name, arg);

	return FALSE;
    }

    assert(FALSE);
}

static int
validate_cmn(char *switch_name, void *arg)
{
    char *op;

    if ((strcmp(arg, "current") == 0) ||
	(strcmp(arg, "none") == 0) ||
	(strcmp(arg, "prior") == 0)) {
	return TRUE;
    }
    else {
	E_ERROR("Unknown CMN type %s %s\n", switch_name, arg);
    }

    assert(FALSE);
}

static int
validate_silcomp(char *switch_name, void *arg)
{
    char *op;

    if ((strcmp(arg, "none") == 0) ||
	(strcmp(arg, "sildelfn") == 0) ||
	(strcmp(arg, "prior") == 0) ||
	(strcmp(arg, "current") == 0)) {
	return TRUE;
    }
    else {
	E_ERROR("Unknown silence compression type %s %s\n", switch_name, arg);
	
	return FALSE;
    }
    
    assert(FALSE);
}

/* defines, parses and (partially) validates the arguments
   given on the command line */

int
train_cmd_ln_parse(int argc, char *argv[])
{
    static arg_def_t defn[] = {
	{ "-moddeffn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "The model definition file for the model inventory to train" },

	{ "-tmatfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "The transition matrix parameter file name"},

	{ "-mixwfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "The mixture weight parameter file name"},

	{ "-meanfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "The mean parameter file name"},

	{ "-varfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "The var parameter file name"},

	{ "-mwfloor",
	  CMD_LN_FLOAT32,
	  CMD_LN_NO_VALIDATION,
	  "0.00001",
	  "Mixing weight smoothing floor" },

	{ "-tpfloor",
	  CMD_LN_FLOAT32,
	  CMD_LN_NO_VALIDATION,
	  "0.0001",
	  "Transition probability smoothing floor" },

	{ "-varfloor",
	  CMD_LN_FLOAT32,
	  CMD_LN_NO_VALIDATION,
	  "0.00001",
	  "The minimum variance"},
	
	{ "-topn",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  "4",
	  "Compute output probabilities based this number of top scoring densities."},

	{ "-dictfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "The content word dictionary" },

	{ "-fdictfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "The filler word dictionary (e.g. SIL, SILb, ++COUGH++)" },

	{ "-ctlfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "The training corpus control file" },

	{ "-nskip",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "The number of utterances to skip at the beginning of a control file" },

	{ "-runlen",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  "-1",	/* until eof */
	  "The number of utterances to process in the (skipped) control file" },

	{ "-part",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Identifies the corpus part number (range 1..NPART)" },

	{ "-npart",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Partition the corpus into this many equal sized subsets" },

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

	{ "-segext",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  "v8_seg",
	  "State segmentation file extension" },

	{ "-segdir",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "State segmentation file root directory" },
	
	{ "-sentdir",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "The sentence transcript file directory"},

	{ "-sentext",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  "sent",
	  "The sentence transcript file extension"},

	{ "-lsnfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "The corpus word transcript file"},

	{ "-accumdir",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "A path where accumulated counts are to be written." },

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
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  "no",
	  "Variance Normalize?"},

	{ "-silcomp",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  "none",
	  "Do silence compression based on {current|prior} utterance"},

	{ "-abeam",
	  CMD_LN_FLOAT64,
	  CMD_LN_NO_VALIDATION,
	  "1e-100",
	  "Evaluate alpha values subject to this beam"},

	{ "-bbeam",
	  CMD_LN_FLOAT64,
	  CMD_LN_NO_VALIDATION,
	  "1e-100",
	  "Evaluate beta values (update reestimation sums) subject to this beam"},

	{ "-varreest",
	  CMD_LN_BOOLEAN,
	  CMD_LN_NO_VALIDATION,
	  "yes",
	  "Reestimate variances"},

	{ "-meanreest",
	  CMD_LN_BOOLEAN,
	  CMD_LN_NO_VALIDATION,
	  "yes",
	  "Reestimate means"},

	{ "-mixwreest",
	  CMD_LN_BOOLEAN,
	  CMD_LN_NO_VALIDATION,
	  "yes",
	  "Reestimate mixing weights"},

	{ "-tmatreest",
	  CMD_LN_BOOLEAN,
	  CMD_LN_NO_VALIDATION,
	  "yes",
	  "Reestimate transition probability matrices"},

	{ "-spkrxfrm",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "A speaker transform to use for SAT modelling"},

	{ "-mllrmult",
	  CMD_LN_BOOLEAN,
	  CMD_LN_NO_VALIDATION,
	  "no",
	  "Reestimate multiplicative term of MLLR adatpation of means"},

	{ "-mllradd",
	  CMD_LN_BOOLEAN,
	  CMD_LN_NO_VALIDATION,
	  "no",
	  "Reestimate shift term of MLLR adaptation of means"},

	{ "-ts2cbfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Tied-state-to-codebook mapping file name" },

	{ "-feat",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "This argument selects the derived feature computation to use."},

	{ "-timing",
	  CMD_LN_BOOLEAN,
	  CMD_LN_NO_VALIDATION,
	  "yes",
	  "Controls whether profiling information is displayed"},
	
	{ "-viterbi",
	  CMD_LN_BOOLEAN,
	  CMD_LN_NO_VALIDATION,
	  "no",
	  "Controls whether Viterbi training is done"},
	
	{ "-2passvar",
	  CMD_LN_BOOLEAN,
	  CMD_LN_NO_VALIDATION,
	  "no",
	  "Reestimate variances based on prior means"},

	{ "-sildelfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "File which specifies frames of background 'silence' to delete" },

	{ "-cb2mllrfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Codebook-to-MLLR-class mapping file name" },

	{ "-spthresh",
	  CMD_LN_FLOAT32,
	  CMD_LN_NO_VALIDATION,
	  "0.0",
	  "State posterior probability floor for reestimation.  States below this are not counted"},
	
	{ "-maxuttlen",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  "0",
	  "Maximum # of frames for an utt ( 0 => no fixed limit )"},
	
	{ "-ckptintv",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Checkpoint the reestimation sums every -chkptintv utts" },
	
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
 * $Log:	train_cmd_ln.c,v $
 * Revision 1.14  97/07/16  11:36:22  eht
 * *** empty log message ***
 * 
 * Revision 1.13  1996/08/06  14:03:47  eht
 * -sildelfn argument to specify silence deletion list
 *
 * Revision 1.12  1996/07/29  16:18:48  eht
 * Make -accumdir optional so that it may be omitted for
 * debugging purposes
 * MLLR command line options
 * -veclen to -ceplen
 * -minvar to -varfloor (now named consistently w/ the other floors)
 * added -2passvar switch to allow reestimation based on prior means
 *
 * Revision 1.11  1996/03/26  14:03:24  eht
 * - Added '-timing' argument
 * - changed doc strings for some arguments
 *
 * Revision 1.10  1996/02/02  17:41:47  eht
 * Add alpha and beta beams
 *
 * Revision 1.9  1996/01/26  18:23:49  eht
 * Reformatted argument specifications
 *
 * Revision 1.8  1995/11/30  20:42:07  eht
 * Add argument for transition matrix reestimation
 * Add argument for state parameter definition file
 *
 *
 */

