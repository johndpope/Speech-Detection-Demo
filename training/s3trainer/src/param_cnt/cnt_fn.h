/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/param_cnt/RCS/cnt_fn.h,v 1.1 97/07/16 11:38:16 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1996 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: cnt_fn.h
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

#ifndef CNT_FN_H
#define CNT_FN_H

#include <s3/prim_type.h>
#include <s3/model_def_io.h>
#include <s3/acmod_set.h>


typedef int (*cnt_fn_t)(uint32 *cnt,	/* observation counts */

			model_def_t *mdef,	/* model definitions */
			
			uint16 *seg,	/* Viterbi (CI phone,state) pairs for all frames */
			uint32 n_frame,
			
			acmod_id_t *phone,	/* CI phone sequence (must already be validated) */
			char *btw_mark,
			uint32 n_phone);

#endif /* CNT_FN_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	cnt_fn.h,v $
 * Revision 1.1  97/07/16  11:38:16  eht
 * Initial revision
 * 
 *
 */
