/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libs2io/include/s3/RCS/int32_io.h,v 1.1 97/07/16 11:39:10 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1996 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: int32_io.h
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

#ifndef INT32_IO_H
#define INT32_IO_H

#include <stdio.h>
#include <s3/prim_type.h>

int32
read_int32 (FILE *stream);

int
write_int32 (FILE *stream,
	     int32 word);

int
read_int32_array (FILE *stream,
		  int32 *base,
		  int length);

int
write_int32_array (FILE *stream,
		   int32 *base,
		   int length);

#endif /* INT32_IO_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	int32_io.h,v $
 * Revision 1.1  97/07/16  11:39:10  eht
 * Initial revision
 * 
 *
 */
