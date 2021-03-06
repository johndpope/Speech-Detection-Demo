/*********************************************************************
 *
 * $Header: /usr2/eht/s3/src/agg_seg/RCS/mk_seg.h,v 1.1 1996/03/04 16:01:37 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1994 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: mk_seg.h
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

#ifndef MK_SEG_H
#define MK_SEG_H

#include <s3/prim_type.h>
#include <s3/acmod_set.h>

int
mk_seg(acmod_set_t *acmod_set,
       uint16 *seg,
       uint32 n_frame,
       acmod_id_t *phone,
       uint32 *start,
       uint32 *len,
       uint32 n_phone);

#endif /* MK_SEG_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log: mk_seg.h,v $
 * Revision 1.1  1996/03/04  16:01:37  eht
 * Initial revision
 *
 *
 */
