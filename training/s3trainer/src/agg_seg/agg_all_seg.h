/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/agg_seg/RCS/agg_all_seg.h,v 1.1 97/07/16 11:38:16 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1997 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: agg_all_seg.h
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

#ifndef AGG_ALL_SEG_H
#define AGG_ALL_SEG_H

#include <s3/prim_type.h>
#include <s3/segdmp.h>

int
agg_all_seg(segdmp_type_t type,
	    const char *dmpfn,
	    uint32 stride);

#endif /* AGG_ALL_SEG_H */ 

/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	agg_all_seg.h,v $
 * Revision 1.1  97/07/16  11:38:16  eht
 * Initial revision
 * 
 *
 */
