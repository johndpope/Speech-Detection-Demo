/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libio/include/s3/RCS/bcomment_io.h,v 1.4 1996/07/29 16:33:40 eht Exp $
 *
 * Carnegie Mellon ARPA Speech Group
 *
 * Copyright (c) 1995 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: bcomment_io.h
 * 
 * Traceability: 
 * 
 * Description: 
 * 
 * Author: 
 * 	$Author: eht $
 *********************************************************************/

#ifndef BCOMMENT_IO_H
#define BCOMMENT_IO_H

#include <s3/prim_type.h>

#include <stdio.h>

int32
bcomment_read(const char **out_comment,
	      FILE *fp);

int32
bcomment_write(FILE *fp,
	       const char *comment);


#endif /* BCOMMENT_IO_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log: bcomment_io.h,v $
 * Revision 1.4  1996/07/29  16:33:40  eht
 * added const attrib
 *
 * Revision 1.3  1996/01/26  17:55:01  eht
 * Use the 'const' specification for the input comment
 *
 * Revision 1.2  1995/10/10  13:10:34  eht
 * Changed to use <s3/prim_type.h>
 *
 * Revision 1.1  1995/08/15  13:46:15  eht
 * Initial revision
 *
 *
 */
