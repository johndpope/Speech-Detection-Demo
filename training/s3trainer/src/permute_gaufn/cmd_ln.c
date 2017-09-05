/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/cp_parm/RCS/cmd_ln.c,v 1.1 97/07/16 11:36:22 eht Exp $
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

static char rcsid[] = "@(#)$Id: cmd_ln.c,v 1.1 97/07/16 11:36:22 eht Exp $";

#include "cmd_ln.h"

#include <s3/cmd_ln.h>
#include <s3/err.h>

#include <stdlib.h>

int
parse_cmd_ln(int argc, char *argv[])
{
#include "cmd_ln_defn.h"

    cmd_ln_define(defn);

    if (argc == 1) {
	cmd_ln_print_definitions();
	exit(1);
    }

    cmd_ln_parse(argc, argv);

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
