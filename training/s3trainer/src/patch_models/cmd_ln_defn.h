/*********************************************************************
 *
 * $Header: $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1998 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: cmd_ln_defn.h
 * 
 * Description: 
 * 	Command line argument definition
 *
 * Author: 
 *      Eric Thayer (eht@cs.cmu.edu)
 *********************************************************************/

#ifndef CMD_LN_DEFN_H
    static arg_def_t defn[] = {
	{ "-cpopsfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_DEFAULT,
	  "Copy operation file with rows <targetid> <sourceid>" },

	{ "-imixwfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_DEFAULT,
	  "Input mixing weight file" },
	{ "-omixwfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_DEFAULT,
	  "Second input and Output mixing weight file" },
	  
	{ "-itmatfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_DEFAULT,
	  "Input transition matrix file" },
	{ "-otmatfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_DEFAULT,
	  "Second input and Output transition matrix file" },

	{ "-igaufn",
	  CMD_LN_STRING,
	  CMD_LN_NO_DEFAULT,
	  "Input Gaussian density parameter file" },
	{ "-ogaufn",
	  CMD_LN_STRING,
	  CMD_LN_NO_DEFAULT,
	  "Second input and Output Gaussian density parameter file" },

    { NULL, CMD_LN_UNDEF, CMD_LN_NO_DEFAULT, NULL }
    };
#define CMD_LN_DEFN_H

#endif /* CMD_LN_DEFN_H */ 

/*
 * Log record.  Maintained by RCS.
 *
 * $Log: $
 *
 */
