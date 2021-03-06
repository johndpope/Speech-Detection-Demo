/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/init_gau/RCS/mk_sseq.h,v 1.2 97/07/16 11:38:16 eht Exp $
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
 * $Log:	mk_sseq.h,v $
 * Revision 1.2  97/07/16  11:38:16  eht
 * *** empty log message ***
 * 
 * Revision 1.1  1996/06/06  19:05:46  eht
 * Initial revision
 *
 * Revision 1.1  1996/01/30  17:11:47  eht
 * Initial revision
 *
 *
 */
