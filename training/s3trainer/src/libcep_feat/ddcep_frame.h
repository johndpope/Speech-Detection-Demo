/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libcep_feat/RCS/ddcep_frame.h,v 1.3 1996/08/05 12:57:06 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1994 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: ddcep_frame.h
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

#ifndef DDCEP_FRAME_H
#define DDCEP_FRAME_H

#include <s3/vector.h>

uint32
ddcep_frame_window_size(void);

void
ddcep_frame(vector_t xcep,
	    vector_t power,
	    vector_t mfcc);

#endif /* DDCEP_FRAME_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log: ddcep_frame.h,v $
 * Revision 1.3  1996/08/05  12:57:06  eht
 * Changed declaration of function from fun(); to fun(void); so that it is
 * a proper prototype.
 *
 * Revision 1.2  1996/01/30  17:09:59  eht
 * *** empty log message ***
 *
 * Revision 1.1  1996/01/08  19:49:28  eht
 * Initial revision
 *
 *
 */
