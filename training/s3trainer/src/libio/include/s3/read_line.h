/*********************************************************************
 *
 * $Header: /usr2/eht/s3/src/libio/include/s3/RCS/read_line.h,v 1.3 1995/10/24 18:49:34 eht Exp $
 *
 * Carnegie Mellon ARPA Speech Group
 *
 * Copyright (c) 1995 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: read_line.h
 * 
 * Traceability: 
 * 
 * Description: 
 * 
 * Author: 
 * 	$Author: eht $
 *********************************************************************/

#ifndef READ_LINE_H
#define READ_LINE_H

#include <s3/prim_type.h>

#include <sys/types.h>

#include <stdio.h>

char *
read_line(char *buf,
	  size_t len,
	  uint32 *n_read,
	  FILE *fp);
	  
#endif /* READ_LINE_H */ 

/*
 * Log record.  Maintained by RCS.
 *
 * $Log: read_line.h,v $
 * Revision 1.3  1995/10/24  18:49:34  eht
 * Get size_t type
 *
 * Revision 1.2  1995/10/10  13:10:34  eht
 * Changed to use <s3/prim_type.h>
 *
 * Revision 1.1  1995/08/15  13:46:15  eht
 * Initial revision
 *
 *
 */
