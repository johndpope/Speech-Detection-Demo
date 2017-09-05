/*********************************************************************
 *
 * $Header: /usr2/eht/s3/src/libcommon/include/s3/RCS/cvt2triphone.h,v 1.3 1996/03/25 15:50:37 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1994 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: cvt2triphone.h
 * 
 * Description: 
 * 
 * Author: 
 * 	Eric H. Thayer (eht@cs.cmu.edu)
 *********************************************************************/

#ifndef CVT2TRIPHONE_H
#define CVT2TRIPHONE_H

#include <s3/acmod_set.h>
#include <s3/prim_type.h>

int
cvt2triphone(acmod_set_t *acmod_set,
	     acmod_id_t *phone,
	     char *btw_mark,
	     uint32 n_phone);

#endif /* CVT2TRIPHONE_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log: cvt2triphone.h,v $
 * Revision 1.3  1996/03/25  15:50:37  eht
 * Added include of <prim_type.h> since they are used here
 *
 * Revision 1.2  1995/12/01  20:46:21  eht
 * include <s3/acmod_set.h> to define admod_*_t
 *
 * Revision 1.1  1995/12/01  20:42:16  eht
 * Initial revision
 *
 *
 */
