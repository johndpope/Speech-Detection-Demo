/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/mk_mllr_class/RCS/cmd_ln.c,v 1.1 97/03/07 09:01:35 eht Exp Locker: eht $
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
 * 
 * Author: 
 * 
 *********************************************************************/

static char rcsid[] = "@(#)$Id: cmd_ln.c,v 1.1 97/03/07 09:01:35 eht Exp Locker: eht $";

#include "cmd_ln.h"

#include <s3/cmd_ln.h>
#include <s3/err.h>

int
parse_cmd_ln(int argc, char *argv[])
{
    static arg_def_t defn[] = {
	{ "-nmap",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "# of codebook -> MLLR class mappings" },

	{ "-nclass",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "# of MLLR classes to map into" },

	{ "-cb2mllrfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Codebook-to-MLLR mapping file to create" },
	  
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
 * Revision 1.1  97/03/07  09:01:35  eht
 * Initial revision
 * 
 *
 */
