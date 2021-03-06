/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libcommon/RCS/ts2cb.c,v 1.1 97/07/16 11:36:22 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1997 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: ts2cb.c
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

static char rcsid[] = "@(#)$Id: ts2cb.c,v 1.1 97/07/16 11:36:22 eht Exp $";

#include <s3/ts2cb.h>
#include <s3/ckd_alloc.h>

uint32 *
semi_ts2cb(uint32 n_ts)
{
    return ckd_calloc(n_ts, sizeof(uint32));
    
}

uint32 *
cont_ts2cb(uint32 n_ts)
{
    uint32 *out, i;

    out = ckd_calloc(n_ts, sizeof(uint32));
    for (i = 0; i < n_ts; i++) {
	out[i] = i;
    }

    return out;
}



/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	ts2cb.c,v $
 * Revision 1.1  97/07/16  11:36:22  eht
 * Initial revision
 * 
 *
 */
