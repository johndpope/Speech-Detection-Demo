/*********************************************************************
 *
 * $Header: /usr2/eht/s3/src/agg_seg/RCS/add_seg.h,v 1.1 1996/03/04 16:01:37 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1994 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: add_seg.h
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

#ifndef ADD_SEG_H
#define ADD_SEG_H

#include <s3/acmod_set.h>
#include <s3/prim_type.h>

int
seg_init(uint32 n);

int
seg_add(acmod_id_t p,
	unsigned char *ccode,
	unsigned char *dcode,
	unsigned char *pcode,
	unsigned char *ddcode,
	uint32 len);


#endif /* ADD_SEG_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log: add_seg.h,v $
 * Revision 1.1  1996/03/04  16:01:37  eht
 * Initial revision
 *
 *
 */
