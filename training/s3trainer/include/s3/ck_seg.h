/*********************************************************************
 *
 * $Header: /usr2/eht/s3/src/libcommon/include/s3/RCS/ck_seg.h,v 1.1 1996/03/04 15:55:43 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1994 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: ck_seg.h
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

#ifndef CK_SEG_H
#define CK_SEG_H

#include <s3/acmod_set.h>

int
ck_seg(acmod_set_t *acmod_set,
       acmod_id_t *ci_phone,
       uint32 n_phone,
       uint16 *seg,
       uint32 n_frame,
       const char *utt_name);


#endif /* CK_SEG_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log: ck_seg.h,v $
 * Revision 1.1  1996/03/04  15:55:43  eht
 * Initial revision
 *
 * Revision 1.1  1996/01/30  17:11:47  eht
 * Initial revision
 *
 *
 */
