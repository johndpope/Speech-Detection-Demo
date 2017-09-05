/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/mk_flat/RCS/parse_cmd_ln.c,v 1.2 97/07/16 11:36:22 eht Exp $
 *
 * Carnegie Mellon ARPA Speech Group
 *
 * Copyright (c) 1995 Carnegie Mellon University.
 * All rights reserved.
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
 * 	$Author: eht $
 *********************************************************************/

static char rcsid[] = "@(#)$Id: parse_cmd_ln.c,v 1.2 97/07/16 11:36:22 eht Exp $";

#include <s3/cmd_ln.h>

#include "parse_cmd_ln.h"

#include <stdlib.h>

parse_cmd_ln(int argc, char *argv[])
{
    static arg_def_t defn[] = {
	{ "-moddeffn",
	      CMD_LN_STRING,
	      CMD_LN_NO_VALIDATION,
	      CMD_LN_NO_DEFAULT,
	      "A SPHINX-III model definition file name" },
	{ "-mixwfn",
	      CMD_LN_STRING,
	      CMD_LN_NO_VALIDATION,
	      CMD_LN_NO_DEFAULT,
	      "An output SPHINX-III mixing weight file name" },
	{ "-topo",
	      CMD_LN_STRING,
	      CMD_LN_NO_VALIDATION,
	      CMD_LN_NO_DEFAULT,
	      "A template model topology matrix file" },
	{ "-tmatfn",
	      CMD_LN_STRING,
	      CMD_LN_NO_VALIDATION,
	      CMD_LN_NO_DEFAULT,
	      "An output SPHINX-III transition matrix file name" },
	{ "-nstream",
	      CMD_LN_INT32,
	      CMD_LN_NO_VALIDATION,
	      "4",
	      "Number of independent observation streams" },
	{ "-ndensity",
	      CMD_LN_INT32,
	      CMD_LN_NO_VALIDATION,
	      "256",
	      "Number of densities per mixture density" },
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

    cmd_ln_print_configuration();
}

/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	parse_cmd_ln.c,v $
 * Revision 1.2  97/07/16  11:36:22  eht
 * *** empty log message ***
 * 
 * Revision 1.1  95/06/02  20:31:23  eht
 * Initial revision
 * 
 *
 */
