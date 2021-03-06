/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/param_cnt/RCS/phone_cnt.h,v 1.1 97/07/16 11:38:16 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1996 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: phone_cnt.h
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

#ifndef PHONE_CNT_H
#define PHONE_CNT_H

#include <s3/prim_type.h>
#include <s3/model_def_io.h>
#include <s3/acmod_set.h>

int
phone_cnt(uint32 *cnt,

	  model_def_t *mdef,	/* model definitions */

	  uint16 *seg,		/* Viterbi (CI phone,state) pairs for all frames */
	  uint32 n_frame,

	  acmod_id_t *phone,	/* CI phone sequence (already validated) */
	  char *btw_mark,
	  uint32 n_phone);


#endif /* PHONE_CNT_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	phone_cnt.h,v $
 * Revision 1.1  97/07/16  11:38:16  eht
 * Initial revision
 * 
 *
 */
