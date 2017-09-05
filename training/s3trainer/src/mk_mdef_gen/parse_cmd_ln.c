/*********************************************************************
 * Carnegie Mellon ARPA Speech Group
 *
 * Copyright (c) 2000 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 * Command line arguments for mk_mdef_gen
 * Derived from EHT's parse_cmd_ln routines
 *
 * Author: Rita Singh
 *********************************************************************/

#include <s3/cmd_ln.h>

#include "parse_cmd_ln.h"

#include <stdlib.h>

parse_cmd_ln(int argc, char *argv[])
{
    static arg_def_t defn[] = {
	{ "-phnlstfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "List of phones"},
	{ "-inCImdef",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Input CI model definition file.\n\t\t\tIf given -phnlstfn ignored\n"},
	{ "-triphnlstfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "A SPHINX-III triphone file name.\n\t\t\tIf given -phnlstfn, -incimdef ignored\n" },
	{ "-inCDmdef",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Input CD model definition file.\n\t\t\tIf given -triphnfn, -phnlstfn, -incimdef ignored\n"},
	{ "-dictfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Dictionary"},
	{ "-fdictfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Filler dictionary"},
	{ "-lsnfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Transcripts file"},
	{ "-n_state_pm",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  "3",
	  "No. of states per HMM"},
	{ "-ocountfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Output phone and triphone counts file"},
	{ "-ocimdef",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Output CI model definition file"},
	{ "-oalltphnmdef",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Output all triphone model definition file"},
	{ "-ountiedmdef",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Output untied model definition file"},
	{ "-minocc",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  "1",
	  "Min occurances of a triphone must occur for inclusion in mdef file"},
	{ "-maxtriphones",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  "100000",
	  "Max. number of triphones desired in mdef file"},
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


