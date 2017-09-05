/*********************************************************************
 *
 * $Header: /usr2/eht/s3/src/libcep_feat/include/s3/RCS/agc.h,v 1.2 1996/03/25 15:46:57 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1994 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: agc.h<2>
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

#ifndef AGC_H
#define AGC_H

#include <s3/prim_type.h>

void
agc(float32 *mfcc,
    uint32 n_frame,
    uint32 veclen,
    char *agc_type);

#endif /* AGC_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log: agc.h,v $
 * Revision 1.2  1996/03/25  15:46:57  eht
 * Added ability to set input feature vector size
 *
 * Revision 1.1  1996/01/26  18:33:28  eht
 * Initial revision
 *
 *
 */
