/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libcep_feat/RCS/del_sil_seg.h,v 1.1 97/07/16 11:38:16 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1996 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: del_sil_seg.h
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

#ifndef DEL_SIL_SEG_H
#define DEL_SIL_SEG_H

#include <s3/prim_type.h>
#include <s3/vector.h>

uint32
del_sil_seg(vector_t *mfcc,
	    uint32 n_frame,
	    uint32 *del_b,
	    uint32 *del_e,
	    uint32 n_del);

#endif /* DEL_SIL_SEG_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	del_sil_seg.h,v $
 * Revision 1.1  97/07/16  11:38:16  eht
 * Initial revision
 * 
 *
 */
