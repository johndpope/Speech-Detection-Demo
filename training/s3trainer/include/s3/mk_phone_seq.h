/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libcommon/include/s3/RCS/mk_phone_seq.h,v 1.1 97/07/16 11:39:10 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1996 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: mk_phone_seq.h
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

#ifndef MK_PHONE_SEQ_H
#define MK_PHONE_SEQ_H

#include <s3/acmod_set.h>
#include <s3/prim_type.h>
#include <s3/lexicon.h>

int
mk_phone_seq(acmod_id_t **out_phone,
	     uint32 *out_n_phone,
	     char *trans,
	     acmod_set_t *acmod_set,
	     lexicon_t *lex);

#endif /* MK_PHONE_SEQ_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	mk_phone_seq.h,v $
 * Revision 1.1  97/07/16  11:39:10  eht
 * Initial revision
 * 
 *
 */
