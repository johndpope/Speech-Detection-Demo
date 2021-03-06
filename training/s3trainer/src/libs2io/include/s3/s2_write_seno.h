/*********************************************************************
 *
 * $Header: /usr2/eht/s3/src/libs2io/include/s3/RCS/s2_write_seno.h,v 1.1 1995/10/12 17:43:36 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1994 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: s2_write_seno.h
 * 
 * Description: 
 * 
 * Author: 
 * 	Eric H. Thayer (eht@cs.cmu.edu)
 *********************************************************************/

#ifndef S2_WRITE_SENO_H
#define S2_WRITE_SENO_H

#include <s3/prim_type.h>
#include <s3/acmod_set.h>

float32 ***
s2_write_seno_3(float ***new_mixw,		/* the sphinx-3 mixture weights we are saving */
		acmod_set_t *acmod_set,		/* the phone set of the model inventory */
		uint32 *cluster_offset,	/* number of senones before each base phone cluster */
		const char *seno_dir,		/* the directory containing the .ccode, .d2code, etc. files */
		uint32 **in_smap,	/* An initial global state sharing map */
		uint32 *state_of);	/* the model state id's for each shared state */

#endif /* S2_WRITE_SENO_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log: s2_write_seno.h,v $
 * Revision 1.1  1995/10/12  17:43:36  eht
 * Initial revision
 *
 *
 */
