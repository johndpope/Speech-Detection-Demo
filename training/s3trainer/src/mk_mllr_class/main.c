/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/mk_mllr_class/RCS/main.c,v 1.1 97/03/07 09:01:29 eht Exp Locker: eht $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1994 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: main.c
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

static char rcsid[] = "@(#)$Id: main.c,v 1.1 97/03/07 09:01:29 eht Exp Locker: eht $";

#include "cmd_ln.h"

#include <s3/cmd_ln.h>
#include <s3/err.h>
#include <s3/s3cb2mllr_io.h>
#include <s3/ckd_alloc.h>
#include <s3/s3.h>

#include <stdio.h>

main(int argc, char *argv[])
{
    int32 i;
    int32 n_map;
    int32 n_class;
    uint32 *mllr_map;
    char line[128];

    parse_cmd_ln(argc, argv);

    if (cmd_ln_access("-nmap")) {
	n_map = *(int32 *)cmd_ln_access("-nmap");
    }
    else {
	E_FATAL("Specify # of state -> MLLR class mappings using -nmap\n");
    }
    if (cmd_ln_access("-nclass")) {
	n_class = *(int32 *)cmd_ln_access("-nclass");
    }
    else {
	E_FATAL("Specify # of MLLR class mappings using -nclass\n");
    }
    if (cmd_ln_access("-cb2mllrfn") == NULL) {
	E_FATAL("Specify output file using -cb2mllrfn\n");
    }

    mllr_map = (uint32 *)ckd_calloc(n_map, sizeof(uint32));

    for (i = 0; i < n_map; i++) {
	if (fgets(line, 128, stdin) == NULL) {
	    E_FATAL("Ran out of mappings at %d, but expected %d\n",
		    i, n_map);
	}
	mllr_map[i] = atoi(line);
    }
    if (fgets(line, 128, stdin) != NULL) {
	E_WARN("Expected EOF after %d mappings, but still more data\n", n_map);
    }
    
    if (s3cb2mllr_write((const char *)cmd_ln_access("-cb2mllrfn"),
			mllr_map,
			n_map,
			n_class) != S3_SUCCESS) {
	exit(1);
    }
}

/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	main.c,v $
 * Revision 1.1  97/03/07  09:01:29  eht
 * Initial revision
 * 
 *
 */
