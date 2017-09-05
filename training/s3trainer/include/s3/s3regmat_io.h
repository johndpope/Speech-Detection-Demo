/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libio/include/s3/RCS/s3regmat_io.h,v 1.1 97/07/16 11:39:10 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1996 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: s3regmat_io.h
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

#ifndef S3REGMAT_IO_H
#define S3REGMAT_IO_H

#define REGMATCNT_FILE_VERSION	"1.0"

#include <s3/prim_type.h>

int
s3regmatcnt_read(const char *fn,
		 float32 *****regr,
		 float32 ******regl,
		 uint32 *n_class,
		 uint32 *n_feat,
		 const uint32 **veclen,
		 uint32 *mllr_mult,
		 uint32 *mllr_add);

int
s3regmatcnt_write(const char *fn,
		  float32 ****regr,
		  float32 *****regl,
		  uint32 n_class,
		  uint32 n_feat,
		  const uint32 *veclen,
		  uint32 mllr_mult,
		  uint32 mllr_add);


#endif /* S3REGMAT_IO_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	s3regmat_io.h,v $
 * Revision 1.1  97/07/16  11:39:10  eht
 * Initial revision
 * 
 *
 */
