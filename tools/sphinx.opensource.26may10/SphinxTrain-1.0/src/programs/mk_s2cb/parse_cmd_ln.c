/* ====================================================================
 * Copyright (c) 1995-2000 Carnegie Mellon University.  All rights 
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
 *********************************************************************
 *
 * File: parse_cmd_ln.c
 * 
 * Traceability: 
 * 
 * Description: 
 * 
 * Author: 
 * 	$Author: egouvea $
 *********************************************************************/

#include <stdlib.h>
#include <s3/common.h>
#include <s3/s3.h>

#include <sys/stat.h>
#include <sys/types.h>

#include "s3/cmd_ln.h"
#include "parse_cmd_ln.h"

int32 parse_cmd_ln(int argc, char *argv[])
{
  uint32      isHelp;
  uint32      isExample;

  const char helpstr[]=
"Description: \n\
\n\
Convert s3 means and variances to s2 codebook format. \n\
cepstrum, delta, delta-delta and power cepstrum basename could \n\
be changed by uers. ";

  const char examplestr[]=
"Example: \n\
\n\
mk_s2cb -meanfn s3mean -varfn s3var -cbdir s2dir -varfloor 0.00001 ";

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

	{ "-meanfn",
	      CMD_LN_STRING,
	      CMD_LN_NO_VALIDATION,
	      CMD_LN_NO_DEFAULT,
	      "A SPHINX-III mean density parameter file name" },
	{ "-varfn",
	      CMD_LN_STRING,
	      CMD_LN_NO_VALIDATION,
	      CMD_LN_NO_DEFAULT,
	      "A SPHINX-III variance density parameter file name" },
	{ "-cbdir",
	      CMD_LN_STRING,
	      CMD_LN_NO_VALIDATION,
	      CMD_LN_NO_DEFAULT,
	      "A directory containing SPHINX-II 1PD codebooks" },
	{ "-varfloor",
	      CMD_LN_FLOAT32,
	      CMD_LN_NO_VALIDATION,
	      CMD_LN_NO_DEFAULT,
	      "Minimum variance value" },
	{ "-cepcb",
	      CMD_LN_STRING,
	      CMD_LN_NO_VALIDATION,
	      "cep.256",
	      "Basename of the cepstrum codebook" },
	{ "-dcepcb",
	      CMD_LN_STRING,
	      CMD_LN_NO_VALIDATION,
	      "d2cep.256",
	      "Basename of the difference cepstrum codebook" },
	{ "-powcb",
	      CMD_LN_STRING,
	      CMD_LN_NO_VALIDATION,
	      "p3cep.256",
	      "Basename of the power codebook" },
	{ "-2dcepcb",
	      CMD_LN_STRING,
	      CMD_LN_NO_VALIDATION,
	      "xcep.256",
	      "Basename of the 2nd order difference cepstrum codebook" },

	{ "-meanext",
	      CMD_LN_STRING,
	      CMD_LN_NO_VALIDATION,
	      "vec",
	      "Mean codebook extension" },

	{ "-varext",
	      CMD_LN_STRING,
	      CMD_LN_NO_VALIDATION,
	      "var",
	      "Variance codebook extension" },

	{ NULL,
	      CMD_LN_UNDEF,
	      CMD_LN_NO_VALIDATION,
	      CMD_LN_NO_DEFAULT,
	      NULL }
    };

    cmd_ln_define(defn);

    if (argc == 1) {
	cmd_ln_print_definitions();
	exit(1);
    }

    cmd_ln_parse(argc, argv);

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
      exit(1);
    }
    if(!isHelp && !isExample){
      cmd_ln_print_configuration();
    }

    return 0;
}


/*
 * Log record.  Maintained by RCS.
 *
 * $Log$
 * Revision 1.6  2004/11/29  01:43:48  egouvea
 * Replaced handling of help or example so that user gets an INFO message instead of a scarier FATAL_ERROR
 * 
 * Revision 1.5  2004/08/10 08:08:26  arthchan2003
 * Sphinx 3 to Sphinx 2 conversion tools' helps and examples
 *
 * Revision 1.4  2004/07/21 18:30:36  egouvea
 * Changed the license terms to make it the same as sphinx2 and sphinx3.
 *
 * Revision 1.3  2001/04/05 20:02:31  awb
 * *** empty log message ***
 *
 * Revision 1.2  2001/02/20 00:28:35  awb
 * *** empty log message ***
 *
 * Revision 1.1  2000/11/22 21:23:18  awb
 * *** empty log message ***
 *
 * Revision 1.1  1995/08/17  01:56:01  eht
 * Initial revision
 *
 *
 */
