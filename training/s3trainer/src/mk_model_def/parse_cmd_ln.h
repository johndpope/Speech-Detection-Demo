/*********************************************************************
 *
 * $Header: /usr2/eht/s3/src/mk_model_def/RCS/parse_cmd_ln.h,v 1.2 1995/10/10 13:10:11 eht Exp $
 *
 * Carnegie Mellon ARPA Speech Group
 *
 * Copyright (c) 1995 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: parse_cmdln.h
 * 
 * Description: 
 * 
 * Author: 
 * 	Eric H. Thayer (eht@cs.cmu.edu)
 *********************************************************************/

#ifndef PARSE_CMDLN_H
#define PARSE_CMDLN_H

#include <s3/prim_type.h>

int32
parse_cmd_ln(int argc,
	     char *argv[]);

#endif /* PARSE_CMDLN_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log: parse_cmd_ln.h,v $
 * Revision 1.2  1995/10/10  13:10:11  eht
 * Changed to use <s3/prim_type.h>
 *
 * Revision 1.1  1995/06/02  20:31:53  eht
 * Initial revision
 *
 *
 */
