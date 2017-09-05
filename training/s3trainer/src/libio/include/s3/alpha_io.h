/*********************************************************************
 *
 * $Header: /usr2/eht/s3/src/libio/include/s3/RCS/alpha_io.h,v 1.1 1996/01/26 17:54:20 eht Exp $
 *
 * Carnegie Mellon ARPA Speech Group
 *
 * Copyright (c) 1995 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: mixw_param_io.h
 * 
 * Traceability: 
 * 
 * Description: 
 * 
 * Author: 
 * 	$Author: eht $
 *********************************************************************/

#ifndef MIXW_PARAM_IO_H
#define MIXW_PARAM_IO_H

#define MIXW_PARAM_VERSION "0.1"

#include <s3/prim_type.h>

int32
mixw_param_write(const char *file_name,
		 char *comment,
		 float32 ***mixw,
		 uint32 n_mixw,
		 uint32 n_feat,
		 uint32 n_cw);

int32
mixw_param_read(char **out_comment,
		float32 ****out_mixw,
		uint32 *out_n_mixw,
		uint32 *out_n_feat,
		uint32 *out_n_cw,
		const char *file_name);

#endif /* MIXW_PARAM_IO_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log: alpha_io.h,v $
 * Revision 1.1  1996/01/26  17:54:20  eht
 * Initial revision
 *
 * Revision 1.2  1995/10/10  13:10:34  eht
 * Changed to use <s3/prim_type.h>
 *
 * Revision 1.1  1995/08/15  13:46:15  eht
 * Initial revision
 *
 *
 */
