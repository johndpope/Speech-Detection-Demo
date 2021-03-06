/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libio/include/s3/RCS/segdmp.h,v 1.3 97/07/16 11:39:10 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1994 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: segdmp.h
 * 
 * Description: 
 * 
 * Author: 
 * 	Eric H. Thayer (eht@cs.cmu.edu)
 *********************************************************************/

#ifndef SEGDMP_H
#define SEGDMP_H

#include <s3/prim_type.h>
#include <s3/acmod_set.h>
#include <s3/vector.h>

typedef enum {
    SEGDMP_TYPE_VQ,
    SEGDMP_TYPE_MFCC,
    SEGDMP_TYPE_FEAT
} segdmp_type_t;

/*
 * Segment dump creation/write
 */
int
segdmp_open_write(const char **dirs,		/* dump directories */
		  const char *dmp_fn,		/* dump file name */
		  const char *idx_fn,		/* index file name */
		  uint32 n_id,		/* # of id's */
		  uint32 *n_seg,		/* # of segments per id */
		  uint32 **n_frame,		/* # of frames per segment */
		  segdmp_type_t data_type);

int
segdmp_close(void);

/*
 * Append frames to the segment dump
 */

int
segdmp_add_vq(acmod_id_t p,
	      unsigned char *ccode,
	      unsigned char *dcode,
	      unsigned char *pcode,
	      unsigned char *ddcode,
	      uint32 n_frame);

int
segdmp_add_mfcc(acmod_id_t p,
		vector_t *mfcc,
		uint32 n_frame,
		uint32 veclen);

int
segdmp_add_feat(acmod_id_t p,
		vector_t **feat,
		uint32 n_frame);

/*
 * Segment dump reading
 */
int
segdmp_open_read(const char **dirs,		/* dump directories */
		 const char *dmp_fn,		/* dump file name */
		 const char *idx_fn,		/* index file name */
		 uint32 *n_id,			/* # of id's */
		 segdmp_type_t *data_type);

/*
 * Get data from the segment dump
 */

int
segdmp_next_vq(acmod_id_t p,
	       unsigned char **out_ccode,
	       unsigned char **out_dcode,
	       unsigned char **out_pcode,
	       unsigned char **out_ddcode,
	       uint32 *out_len);

int
segdmp_next_mfcc(acmod_id_t p,
		 vector_t **out_mfcc,
		 uint32 *out_n_frame,
		 uint32 *out_veclen);

int
segdmp_next_feat(acmod_id_t p,
		 vector_t ***out_feat,	/* use feat_*() routines to find out other dims */
		 uint32 *out_n_frame);

/*
 * Segment dump state query calls
 */

uint32
segdmp_n_seg(uint32 id);

/*
 * Misc
 */
void
segdmp_set_bufsz(uint32 sz_in_meg);


#endif /* SEGDMP_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	segdmp.h,v $
 * Revision 1.3  97/07/16  11:39:10  eht
 * *** empty log message ***
 * 
 * Revision 1.2  96/07/29  16:33:40  eht
 * n_stream and vecsize determined at initialization time
 * 
 * Revision 1.1  1996/03/25  15:52:35  eht
 * Initial revision
 *
 *
 */
