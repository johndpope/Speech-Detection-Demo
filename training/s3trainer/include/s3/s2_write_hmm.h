/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libs2io/include/s3/RCS/s2_write_hmm.h,v 1.4 97/07/16 11:39:10 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1994 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: s2_write_hmm.h
 * 
 * Description: 
 * 
 * Author: 
 * 	Eric H. Thayer (eht@cs.cmu.edu)
 *********************************************************************/

#ifndef S2_WRITE_HMM_H
#define S2_WRITE_HMM_H


#include <s3/acmod_set.h>
#include <s3/model_def.h>
#include <s3/prim_type.h>

int
s2_write_hmm(float32 ***tmat,
	     acmod_set_t *phone_set,
	     const char *out_dir_name);

int
s2_write_dhmm(float32 ***tmat,
	      float32 ***mixw,
	      model_def_t *mdef,
	      const char  *out_dir_name);

#endif /* S2_WRITE_HMM_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	s2_write_hmm.h,v $
 * Revision 1.4  97/07/16  11:39:10  eht
 * *** empty log message ***
 * 
 * Revision 1.3  95/10/10  13:10:34  eht
 * Changed to use <s3/prim_type.h>
 * 
 *
 */
