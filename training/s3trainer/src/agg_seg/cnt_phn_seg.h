/*********************************************************************
 *
 * $Header$
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1997 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: cnt_phn_seg.h
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

#ifndef CNT_PHN_SEG_H
#define CNT_PHN_SEG_H


#include <s3/prim_type.h>
#include <s3/model_def_io.h>
#include <s3/lexicon.h>

int
cnt_phn_seg(model_def_t *mdef,
	    lexicon_t *lex,
	    uint32 **out_n_seg,
	    uint32 ***out_n_frame_per);

#endif /* CNT_PHN_SEG_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	cnt_phn_seg.h,v $
 * Revision 1.1  97/07/25  11:40:43  eht
 * Initial revision
 * 
 * Revision 1.1  97/07/16  11:38:16  eht
 * Initial revision
 * 
 *
 */
