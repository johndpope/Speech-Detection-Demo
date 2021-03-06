/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libio/include/s3/RCS/s3ts2cb_io.h,v 1.1 97/07/16 11:39:10 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1996 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: s3ts2cb_io.h
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

#ifndef S3TS2CB_IO_H
#define S3TS2CB_IO_H

#include <s3/prim_type.h>

int
s3ts2cb_read(const char *fn,
	     uint32 **out_ts2cb,
	     uint32 *out_n_ts,
	     uint32 *out_n_cb);

int
s3ts2cb_write(const char *fn,
	      uint32 *ts2cb,
	      uint32 n_ts,
	      uint32 n_cb);
#endif /* S3TS2CB_IO_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	s3ts2cb_io.h,v $
 * Revision 1.1  97/07/16  11:39:10  eht
 * Initial revision
 * 
 *
 */
