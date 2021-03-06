/* ====================================================================
 * Copyright (c) 2006 Carnegie Mellon University.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * This work was supported in part by funding from the Defense Advanced 
 * Research Projects Agency and the National Science Foundation of the 
 * United States of America, and the CMU Sphinx Speech Consortium.
 *
 * THIS SOFTWARE IS PROVIDED BY CARNEGIE MELLON UNIVERSITY ``AS IS'' AND 
 * ANY EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL CARNEGIE MELLON UNIVERSITY
 * NOR ITS EMPLOYEES BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ====================================================================
 *
 */
/*********************************************************************
 *
 * File: parse_cmd_ln.c
 * 
 * Description: 
 * 	These routines define and parse the command line for lda_train.
 *
 * Author: 
 * 	David Huggins-Daines <dhuggins@cs.cmu.edu>
 *********************************************************************/

#include "parse_cmd_ln.h"

#include <s3/cmd_ln.h>
#include <s3/s3.h>

#include <sys_compat/misc.h>

#include <stdio.h>
#include <assert.h>


/* defines, parses and (partially) validates the arguments
   given on the command line */

int
parse_cmd_ln(int argc, char *argv[])
{
  uint32      isHelp;
  uint32      isExample;

  const char helpstr[] = 
"Description: This program trains an LDA transformation from a training set.";

  const char examplestr[]=
"Example:\n\
\n\
lda_train -ceplen 13 -feat 1s_c_d_dd -dim 29 -cepdir cep -stsegdir stseg \n\
\t-ctlfn foo.ctl -cmn current -agc none\n";

    static arg_def_t defn[] = {
	{ "-help",
	  CMD_LN_BOOLEAN,
	  CMD_LN_NO_VALIDATION,
	  "no",
	  "Shows the usage of the tool"},

	{ "-example",
	  CMD_LN_BOOLEAN,
	  CMD_LN_NO_VALIDATION,
	  "no",
	  "Shows example of how to use the tool"},

	{ "-ctlfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Control file of the training corpus"},
	{ "-nskip",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "# of lines to skip in the control file"},
	{ "-runlen",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "# of lines to process in the control file (after any skip)"},
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
	{ "-moddeffn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Model definition file (to define the states in the segmentation files)." },
	{ "-cepdir",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Root directory of the training corpus cepstrum files."},
	{ "-cepext",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  FEAT_DEFAULT_FEATURE_EXTENSION,
	  "Extension of the training corpus cepstrum files."},
	{ "-segdir",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Root directory of the state segmentation files"},
	{ "-segext",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  "v8_seg",
	  "Extension of the state segmentation files"},
	{ "-cmn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  "current",
	  "Controls cepstral mean normalization."},
	{ "-varnorm",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  "no",
	  "Controls variance normalization."},
	{ "-silcomp",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  "none",
	  "Do silence compression based on {current|prior} utterance"},
	{ "-agc",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  "none",
	  "Controls automatic gain control."},
	{ "-feat",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  FEAT_DEFAULT_FEATURE_TYPE,
	  "Controls which feature extraction algorithm is used."},
	{ "-ceplen",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  FEAT_DEFAULT_CEP_LENGTH,
	  "# of components in cepstrum vector"},
	{ "-cepwin",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  "0",
	  "sliding window of features to concatenate (for -feat 1s_c ONLY)"},
	{ "-outfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Output file containing LDA transformation matrix"},
	{ "-outcovfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Output file containing class covariances (for MLLT)"},
	{ "-outcountfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Output file containing class counts (for MLLT)"},
	{ NULL, CMD_LN_UNDEF, CMD_LN_NO_VALIDATION, CMD_LN_NO_DEFAULT, NULL }
    };

    cmd_ln_define(defn);

    if (argc == 1) {
	cmd_ln_print_definitions();
	exit(1);
    }

    cmd_ln_parse(argc, argv);

    if (cmd_ln_validate() == FALSE) {
	/* one or more command line arguments were
	   deemed invalid */
	E_FATAL("Unable to validate command line\n");
    }

    isHelp    = *(uint32 *) cmd_ln_access("-help");
    isExample    = *(uint32 *) cmd_ln_access("-example");


    if(isHelp){
      printf("%s\n\n",helpstr);
    }

    if(isExample){
      printf("%s\n\n",examplestr);
    }

    if(isHelp || isExample){
      E_INFO("User asked for help or example.\n");
      exit(0);
    }
    if(!isHelp && !isExample){
      cmd_ln_print_configuration();
    }


    return 0;
}
