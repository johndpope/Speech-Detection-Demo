/*********************************************************************
 *
 * $Header: /usr2/eht/s3/src/libcep_feat/RCS/s2_dcep.h,v 1.2 1996/03/25 15:36:54 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1994 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: s2_dcep.h
 * 
 * Description: 
 * 	Computes the dcep feature:
 *		 dcep: < (cep[t + 2] - cep[t - 2]), (cep[t + 4] - cep[t - 4]) >
 * Author: 
 * 	Eric H. Thayer (eht@cs.cmu.edu)
 *********************************************************************/

#ifndef S2_DCEP_H
#define S2_DCEP_H

#include <s3/vector.h>
#include <s3/prim_type.h>

void
s2_dcep_set_veclen(uint32 veclen);

void
s2_short_dcep_feat(vector_t **out,
		   vector_t *mfcc,
		   uint32 n_frame);

void
s2_long_dcep_feat(vector_t **out,
		  vector_t *mfcc,
		  uint32 n_frame);

#endif /* S2_DCEP_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log: s2_dcep.h,v $
 * Revision 1.2  1996/03/25  15:36:54  eht
 * Allow for settable input feature length
 *
 * Revision 1.1  1996/01/30  17:09:59  eht
 * Initial revision
 *
 *
 */
