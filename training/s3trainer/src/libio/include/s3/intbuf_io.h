/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libio/include/s3/RCS/intbuf_io.h,v 1.2 1996/07/29 16:33:40 eht Exp $
 *
 * Carnegie Mellon ARPA Speech Group
 *
 * Copyright (c) 1995 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: floatbuf_io.h
 * 
 * Description: 
 * 
 * Author: 
 * 	Eric H. Thayer (eht@cs.cmu.edu)
 *********************************************************************/

#ifndef INTBUF_IO_H
#define INTBUF_IO_H

#include <s3/prim_type.h>
#include <s3/s3.h>

#include <stdio.h>

int32
uint32buf_write(FILE *fp,
		const uint32 *b,
		uint32 n);
int32
uint32buf_read(uint32 **out_b,
	       uint32 *out_n,
	       FILE *fp);

#endif /* INTBUF_IO_H */ 

/*
 * Log record.  Maintained by RCS.
 *
 * $Log: intbuf_io.h,v $
 * Revision 1.2  1996/07/29  16:33:40  eht
 * added const attrib
 *
 * Revision 1.1  1996/01/26  17:55:01  eht
 * Initial revision
 *
 *
 */
