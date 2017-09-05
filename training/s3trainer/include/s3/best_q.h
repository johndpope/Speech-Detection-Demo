/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libcommon/include/s3/RCS/best_q.h,v 1.1 97/07/16 11:39:10 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1997 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: best_q.h
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

#ifndef BEST_Q_H
#define BEST_Q_H

#include <s3/prim_type.h>
#include <s3/quest.h>

float64
best_q(float32 ****mixw,
/* ADDITION FOR CONTINUOUS_TREES 21 May 98 */
       float32 ****means,
       float32 ****vars,
       uint32  *veclen,
/* END ADDITION FOR CONTINUOUS_TREES */
       uint32 n_model,
       uint32 n_state,
       uint32 n_stream,
       uint32 n_density,
       float32 *stwt,
       uint32 **dfeat,
       uint32 n_dfeat,
       quest_t *all_q,
       uint32 n_all_q,
       pset_t *pset,
       uint32 *id,
       uint32 n_id,
       float32 ***dist,
/* ADDITION FOR CONTINUOUS_TREES 21 May 98 */
       float64 node_wt_ent,  /* Weighted entropy of node */
/* END ADDITION FOR CONTINUOUS_TREES */
       quest_t **out_best_q);

#endif /* BEST_Q_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	best_q.h,v $
 * Revision 1.1  97/07/16  11:39:10  eht
 * Initial revision
 * 
 *
 */
