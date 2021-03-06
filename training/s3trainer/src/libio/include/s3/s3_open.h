/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libio/include/s3/RCS/s3_open.h,v 1.2 1996/07/29 16:33:40 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1994 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: s3_open.h
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

#ifndef S3_OPEN_H
#define S3_OPEN_H

#include <s3/prim_type.h>
#include <stdio.h>

FILE *
s3_open_bin_read(const char *file_name,
		 const char *in_version,
		 const char **out_comment);

FILE *
s3_open_bin_write(const char *file_name,
		  const char *version,
		  const char *comment);

#endif /* S3_OPEN_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log: s3_open.h,v $
 * Revision 1.2  1996/07/29  16:33:40  eht
 * added const attrib
 *
 * Revision 1.1  1996/01/26  17:59:47  eht
 * Initial revision
 *
 *
 */
