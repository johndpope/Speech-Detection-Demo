/*********************************************************************
 *
 * $Header: /usr2/eht/s3/src/libcep_feat/RCS/ddcep.c,v 1.1 1996/01/26 18:04:51 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1994 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: ddcep.c
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

static char rcsid[] = "@(#)$Id: ddcep.c,v 1.1 1996/01/26 18:04:51 eht Exp $";

#include "ddcep_frame.h"

#include <s3/feat.h>

void
ddcep_frame(vector_t xcep,
	    vector_t power,
	    vector_t mfcc)
{
    int32 w;
    int32 onefrm;
    int32 k;
    float32 d1;
    float32 d2;

    onefrm = feat_mfcc_len();

    w = S2_SHORT_DIFFW * onefrm;

    d1 = mfcc[w + onefrm] - mfcc[onefrm - w];
    d2 = mfcc[w - onefrm] - mfcc[-onefrm - w];

    power[2] = d1 - d2;

    for (k = 1; k < S2_CEP_VECLEN; k++) {
	d1 = mfcc[k + w + onefrm] - mfcc[k - w + onefrm];
	d2 = mfcc[k + w - onefrm] - mfcc[k - w - onefrm];

	xcep[k-1] = d1 - d2;
    }
}

/*
 * Log record.  Maintained by RCS.
 *
 * $Log: ddcep.c,v $
 * Revision 1.1  1996/01/26  18:04:51  eht
 * Initial revision
 *
 *
 */
