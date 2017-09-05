/*********************************************************************
 *
 * $Header: /usr2/eht/s3/src/libcommon/include/s3/RCS/state_seq.h,v 1.1 1996/06/17 14:29:21 eht Exp $
 *
 * Carnegie Mellon ARPA Speech Group
 *
 * Copyright (c) 1995 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: state_seq.h
 * 
 * Description: 
 * 
 * Author: 
 * 	Eric H. Thayer (eht@cs.cmu.edu)
 *********************************************************************/

#ifndef STATE_SEQ_H
#define STATE_SEQ_H

#include <s3/state.h>
#include <s3/model_inventory.h>
#include <s3/model_def_io.h>

int
state_seq_free(state_t *s,
	       unsigned int n);

state_t *
state_seq_make(uint32 *n_state,
	       acmod_id_t *phone,
	       uint32 n_phone,
	       model_inventory_t *inv,
	       model_def_t *mdef);

void
state_seq_print(state_t *state,
		uint32 n_state,
		model_def_t *mdef);

#endif /* STATE_SEQ_H */ 

/*
 * Log record.  Maintained by RCS.
 *
 * $Log: state_seq.h,v $
 * Revision 1.1  1996/06/17  14:29:21  eht
 * Initial revision
 *
 *
 */
