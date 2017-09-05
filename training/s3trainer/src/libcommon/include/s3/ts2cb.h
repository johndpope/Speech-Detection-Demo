/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libcommon/include/s3/RCS/ts2cb.h,v 1.1 97/07/16 11:39:10 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1997 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: ts2cb.h
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

#ifndef TS2CB_H
#define TS2CB_H

#include <s3/prim_type.h>

#define CONT_LABEL	".cont."
#define SEMI_LABEL	".semi."

uint32 *
semi_ts2cb(uint32 n_ts);

uint32 *
cont_ts2cb(uint32 n_ts);

#endif /* TS2CB_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	ts2cb.h,v $
 * Revision 1.1  97/07/16  11:39:10  eht
 * Initial revision
 * 
 *
 */
