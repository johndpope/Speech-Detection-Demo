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
	{ "-igaufn",
	  CMD_LN_STRING,
	  CMD_LN_NO_DEFAULT,
	  "Input Gaussian density parameter file" },
	  
	{ "-infeat",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	"The feature of the input gaufn (1_s_c_d_dd or s3_1x39)"},

	{ "-ogaufn",
	  CMD_LN_STRING,
	  CMD_LN_NO_DEFAULT,
	  "Second input and Output Gaussian density parameter file" },
	  
	{ "-outfeat",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	"The feature of the output gaufn (1_s_c_d_dd or s3_1x39)"},

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
