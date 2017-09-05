/*********************************************************************
 *
 * $Header: /usr2/eht/s3/src/libcommon/include/s3/RCS/was_added.h,v 1.1 1996/01/26 18:29:54 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1994 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: was_added.h
 * 
 * Description: 
 * 
 * Author: 
 * 	Eric H. Thayer (eht@cs.cmu.edu)
 *********************************************************************/

#ifndef WAS_ADDED_H
#define WAS_ADDED_H

#include <s3/prim_type.h>

/*
 * Linked list element that contains as data
 * the source tied parameter id
 */
typedef struct pair_str {
    uint32 src_id;
    struct pair_str *next;
} pair_t;

#ifndef NULL
#define NULL (void *)0
#endif

#ifndef TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE	0
#endif

pair_t **
init_was_added(uint32 n_dest);

int
was_added(pair_t **l,
	  uint32 src);

#endif /* WAS_ADDED_H */ 

/*
 * Log record.  Maintained by RCS.
 *
 * $Log: was_added.h,v $
 * Revision 1.1  1996/01/26  18:29:54  eht
 * Initial revision
 *
 *
 */
