/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libio/include/s3/RCS/s3cb2mllr_io.h,v 1.1 97/07/16 11:39:10 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1996 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: s3cb2mllr_io.h
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

#ifndef S3CB2MLLR_IO_H
#define S3CB2MLLR_IO_H

#include <s3/prim_type.h>

int
s3cb2mllr_read(const char *fn,
	       uint32 **out_cb2mllr,
	       uint32 *out_n_cb,
	       uint32 *out_n_mllr);

int
s3cb2mllr_write(const char *fn,
		uint32 *cb2mllr,
		uint32 n_cb,
		uint32 n_mllr);

#endif /* S3CB2MLLR_IO_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	s3cb2mllr_io.h,v $
 * Revision 1.1  97/07/16  11:39:10  eht
 * Initial revision
 * 
 *
 */
