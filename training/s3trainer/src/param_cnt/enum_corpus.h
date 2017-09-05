/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/param_cnt/RCS/enum_corpus.h,v 1.1 97/07/16 11:38:16 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1996 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: enum_corpus.h
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

#ifndef ENUM_CORPUS_H
#define ENUM_CORPUS_H

#include <s3/lexicon.h>
#include <s3/model_def_io.h>
#include <s3/prim_type.h>
#include "cnt_fn.h"

int
enum_corpus(lexicon_t *lex,
	    model_def_t *mdef,
	    uint32 *cnt,
	    cnt_fn_t cnt_fn);

#endif /* ENUM_CORPUS_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	enum_corpus.h,v $
 * Revision 1.1  97/07/16  11:38:16  eht
 * Initial revision
 * 
 *
 */
