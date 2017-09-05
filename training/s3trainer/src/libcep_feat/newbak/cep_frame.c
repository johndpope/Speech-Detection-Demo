/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libcep_feat/RCS/cep_frame.c,v 1.2 1996/08/05 12:53:31 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1994 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: cep_frame.c
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

static char rcsid[] = "@(#)$Id: cep_frame.c,v 1.2 1996/08/05 12:53:31 eht Exp $";

#include "cep_frame.h"

#include <s3/err.h>

#include <string.h>

static uint32 cepsize = 12;

void
cep_frame_set_size(uint32 sz)
{
    cepsize = sz;
}

void
cep_frame(vector_t cep,
	  vector_t pow,
	  vector_t mfcc)
{
    memcpy(cep, &mfcc[1], cepsize * sizeof(float32));

    pow[0] = mfcc[0];
}

/*
 * Log record.  Maintained by RCS.
 *
 * $Log: cep_frame.c,v $
 * Revision 1.2  1996/08/05  12:53:31  eht
 * Include <string.h> for prototype
 *
 * Revision 1.1  1996/01/26  18:04:51  eht
 * Initial revision
 *
 *
 */
