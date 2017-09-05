/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libclust/RCS/merge_den.c,v 1.1 97/07/16 11:36:22 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1997 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: merge_den.c
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

static char rcsid[] = "@(#)$Id: merge_den.c,v 1.1 97/07/16 11:36:22 eht Exp $";
#include <s3/merge_den.h>

#include <string.h>

void
merge_d(float32 ***opdf,
	uint32 *i2o,

	float32 ***ipdf,
	uint32 n_ipdf,

	uint32 n_stream,
	uint32 n_cw)
{
    uint32 o, i, j, k;

    for (i = 0; i < n_ipdf; i++) {
	o = i2o[i];

	for (j = 0; j < n_stream; j++) {
	    for (k = 0; k < n_cw; k++) {
		opdf[o][j][k] += ipdf[i][j][k];
	    }
	}
    }
}

void
interpolate_d(float32 ***opdf,
	      uint32 n_opdf,
	      float32 ***ipdf,
	      uint32 n_ipdf,
	      float32 *wt,
	      uint32 n_wt,
	      uint32 *i2wt,
	      uint32 *i2o,
	      uint32 n_stream,
	      uint32 n_codeword)
{
    uint32 o, i, j, k;
    float32 _wt;
    
    for (i = 0; i < n_ipdf; i++) {
	o = i2o[i];
	_wt = i2wt[i];
	for (j = 0; j < n_stream; j++) {
	    for (k = 0; k < n_codeword; k++) {
		opdf[o][j][k] += _wt * ipdf[i][j][k];
	    }
	}
    }
}

/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	merge_den.c,v $
 * Revision 1.1  97/07/16  11:36:22  eht
 * Initial revision
 * 
 *
 */
