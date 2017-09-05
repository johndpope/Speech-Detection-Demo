/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libcommon/include/s3/RCS/mk_trans_seq.h,v 1.1 97/07/16 11:39:10 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1996 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: mk_trans_seq.h
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

#ifndef MK_TRANS_SEQ_H
#define MK_TRANS_SEQ_H

#include <s3/prim_type.h>
#include <s3/model_def.h>

int
mk_trans_seq(uint32 **out_tmat_seq,
	     uint32 **out_mstate_seq,
	     uint16 *seg,
	     uint32 n_frame,
	     acmod_id_t *phone,
	     uint32 n_phone,
	     model_def_t *mdef);



#endif /* MK_TRANS_SEQ_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	mk_trans_seq.h,v $
 * Revision 1.1  97/07/16  11:39:10  eht
 * Initial revision
 * 
 *
 */
