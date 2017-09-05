/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libcommon/include/s3/RCS/mk_sseq.h,v 1.2 1996/07/29 16:40:50 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1994 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: mk_sseq.h
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

#ifndef MK_SSEQ_H
#define MK_SSEQ_H

#include <s3/prim_type.h>
#include <s3/acmod_set.h>
#include <s3/model_def_io.h>

uint32 *
mk_sseq(uint16 *seg,
	uint32 n_frame,
	acmod_id_t *phone,
	uint32 n_phone,
	model_def_t *mdef);

#endif /* MK_SSEQ_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log: mk_sseq.h,v $
 * Revision 1.2  1996/07/29  16:40:50  eht
 * get rid of acmod_set argument as that it is now inside of
 * mdef
 *
 * Revision 1.1  1996/06/06  18:56:41  eht
 * Initial revision
 *
 * Revision 1.1  1996/01/30  17:11:47  eht
 * Initial revision
 *
 *
 */
