/*********************************************************************
 *
 * $Header: /usr2/eht/s3/src/libio/include/s3/RCS/topo_read.h,v 1.2 1995/10/10 13:10:34 eht Exp $
 *
 * Carnegie Mellon ARPA Speech Group
 *
 * Copyright (c) 1995 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: topo_read.h
 * 
 * Traceability: 
 * 
 * Description: 
 * 
 * Author: 
 * 	$Author: eht $
 *********************************************************************/

#ifndef TOPO_READ_H
#define TOPO_READ_H

#include <s3/prim_type.h>

int32
topo_read(float32 ***tmat,
	  uint32 *n_state_pm,
	  const char *topo_file_name);

#endif /* TOPO_READ_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log: topo_read.h,v $
 * Revision 1.2  1995/10/10  13:10:34  eht
 * Changed to use <s3/prim_type.h>
 *
 * Revision 1.1  1995/08/15  13:46:15  eht
 * Initial revision
 *
 *
 */
