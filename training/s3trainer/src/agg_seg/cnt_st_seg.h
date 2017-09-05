/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/agg_seg/RCS/cnt_st_seg.h,v 1.1 97/07/16 11:38:16 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1997 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: cnt_st_seg.h
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

#ifndef CNT_ST_SEG_H
#define CNT_ST_SEG_H


#include <s3/prim_type.h>
#include <s3/model_def_io.h>
#include <s3/lexicon.h>

uint32 *
cnt_st_seg(model_def_t *mdef,
	   lexicon_t *lex);


#endif /* CNT_ST_SEG_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	cnt_st_seg.h,v $
 * Revision 1.1  97/07/16  11:38:16  eht
 * Initial revision
 * 
 *
 */
