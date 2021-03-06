/*********************************************************************
 *
 * $Header: /usr2/eht/s3/src/libcep_feat/RCS/live_norm.h,v 1.1 1996/01/30 17:09:59 eht Exp $
 *
 * Carnegie Mellon ARPA Speech Group
 *
 * Copyright (c) 1995 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: live_norm.h
 * 
 * Traceability: 
 * 
 * Description: 
 * 
 * Author: 
 * 	$Author: eht $
 *********************************************************************/

#ifndef LIVE_NORM_H
#define LIVE_NORM_H

#include <s3/prim_type.h>

void mean_norm_init(uint32 vlen);

void mean_norm_acc_sub(float32 *vec);

void mean_norm_update(void);

#endif /* LIVE_NORM_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log: live_norm.h,v $
 * Revision 1.1  1996/01/30  17:09:59  eht
 * Initial revision
 *
 * Revision 1.2  1995/10/10  12:35:52  eht
 * Changed to use <s3/prim_type.h>
 *
 * Revision 1.1  1995/08/15  13:45:04  eht
 * Initial revision
 *
 *
 */
