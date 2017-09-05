/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/agg_seg/RCS/agg_phn_seg.h,v 1.1 97/07/16 11:38:16 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1994 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: agg_phn_seg.h
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

#ifndef AGG_PHN_SEG_H
#define AGG_PHN_SEG_H

#include <s3/lexicon.h>
#include <s3/model_def_io.h>
#include <s3/segdmp.h>

int
agg_phn_seg(lexicon_t *lex,
	    acmod_set_t *acmod_set,
	    segdmp_type_t type);

#endif /* AGG_PHN_SEG_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	agg_phn_seg.h,v $
 * Revision 1.1  97/07/16  11:38:16  eht
 * Initial revision
 * 
 *
 */