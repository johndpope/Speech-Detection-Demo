/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libio/include/s3/RCS/s3io.h,v 1.1 97/07/16 11:39:10 eht Exp Locker: eht $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1996 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: s3io.h
 * 
 * Description: 
 *    Open, read and write files conforming to S3 conventions
 *
 * Author: 
 *    Eric Thayer
 *********************************************************************/

#ifndef S3IO_H
#define S3IO_H


#include <s3/prim_type.h>

#include <stdio.h>
#include <stdlib.h>

FILE *
s3open(const char *file_name,
       const char *mode,
       uint32 *swap);

void
s3clr_fattr(void);

void 
s3get_fattr(char ***out_attrib,
	    char ***out_value);

char *
s3get_gvn_fattr(char *attrib);

void
s3add_fattr(char *attrib,
	    char *value,
	    int dup);

/* read/write calls.  Patterned after fread/fwrite */
size_t
s3read(void *pointer,
       size_t size,
       size_t num_items,
       FILE *stream,
       uint32 swap,
       uint32 *chksum);

size_t
s3write(const void *pointer,
	size_t size,
	size_t num_items,
	FILE *stream,
	uint32 *chksum);


int
s3close(FILE *fp);

/* Array read/write calls */
int32
s3read_3d(void ****arr,
	  size_t e_sz,
	  uint32 *d1,
	  uint32 *d2,
	  uint32 *d3,
	  FILE *fp,
	  uint32 swap,
	  uint32 *chksum);
int32
s3read_intv_3d(void ****arr,
	       size_t e_sz,
	       uint32 s,
	       uint32 e,
	       uint32 *d1,
	       uint32 *d2,
	       uint32 *d3,
	       FILE *fp,
	       uint32 swap,
	       uint32 *chksum);

int32
s3write_3d(void ***arr,
	   size_t e_sz,
	   uint32 d1,
	   uint32 d2,
	   uint32 d3,
	   FILE *fp,
	   uint32 *chksum);

int32
s3read_2d(void ***arr,
	  size_t e_sz,
	  uint32 *d1,
	  uint32 *d2,
	  FILE *fp,
	  uint32 swap,
	  uint32 *chksum);

int32
s3write_2d(void **arr,
	   size_t e_sz,
	   uint32 d1,
	   uint32 d2,
	   FILE *fp,
	   uint32 *chksum);

int32
s3read_1d(void **arr,
	  size_t e_sz,
	  uint32 *d1,
	  FILE *fp,
	  uint32 swap,
	  uint32 *chksum);

int32
s3write_1d(void *arr,
	   size_t e_sz,
	   uint32 d1,
	   FILE *fp,
	   uint32 *chksum);

#endif /* S3IO_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	s3io.h,v $
 * Revision 1.1  97/07/16  11:39:10  eht
 * Initial revision
 * 
 *
 */
