/*********************************************************************
 *
 * $Header: /usr2/eht/s3/src/libcommon/include/s3/RCS/get_time.h,v 1.2 1995/10/09 20:55:35 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1994 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: get_time.h
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

#ifndef GET_TIME_H
#define GET_TIME_H

#include <s3/prim_type.h>

int
get_time(uint32 *sec, float32 *frac_sec);

#endif /* GET_TIME_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log: get_time.h,v $
 * Revision 1.2  1995/10/09  20:55:35  eht
 * Changes needed for prim_type.h
 *
 * Revision 1.1  1995/09/15  15:24:33  eht
 * Initial revision
 *
 *
 */
