/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libio/include/s3/RCS/gauden_io.h,v 1.6 97/07/16 11:39:10 eht Exp $
 *
 * Carnegie Mellon ARPA Speech Group
 *
 * Copyright (c) 1995 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: gauden_io.h
 * 
 * Description: 
 * 
 * Author: 
 * 	Eric H. Thayer (eht@cs.cmu.edu)
 *********************************************************************/

#ifndef GAUDEN_IO_H
#define GAUDEN_IO_H

#include <s3/prim_type.h>
#include <s3/s3.h>
#include <s3/common.h>

#define GAUDEN_PARAM_VERSION	"0.1"
#define GAUDEN_COUNTS_VERSION	"0.3"
#define GAUDEN_DCOUNTS_VERSION	"0.1"

int32
gauden_bundle(const char *dir,
	      const char *ext,
	      vector_t ****out_param,
	      uint32 n_mgau,
	      uint32 *out_n_feat,
	      uint32 *out_n_density,
	      const uint32 **veclen);

int32
gauden_unbundle(const char *dir,
		const char *ext,
		vector_t ***out,
		uint32 n_mgau,
		uint32 n_feat,
		uint32 n_density,
		const uint32 *veclen);

int32
gauden_write(const char *file_name,
	     const char *comment,
	     vector_t ***out,
	     uint32 n_mgau,
	     uint32 n_feat,
	     uint32 n_density,
	     const uint32 *veclen);

int32
gauden_read(const char **out_comment,
	    vector_t ****out_param,
	    uint32 *out_n_mgau,
	    uint32 *out_n_feat,
	    uint32 *out_n_density,
	    uint32 **out_veclen,
	    const char *file_name);

int32
gauden_counts_write(const char *file_name,
		    const char *comment,
		    vector_t ***wt_mean,
		    vector_t ***wt_var,
		    int32 pass2var,
		    float32 ***dnom,
		    uint32 n_mgau,
		    uint32 n_feat,
		    uint32 n_density,
		    const uint32 *veclen);

int32
gauden_counts_read(const char **out_comment,
		   vector_t ****out_wt_mean,
		   vector_t ****out_wt_var,
		   int32 *pass2var,
		   float32 ****out_dnom,
		   uint32 *out_n_mgau,
		   uint32 *out_n_feat,
		   uint32 *out_n_density,
		   uint32 **out_veclen,
		   const char *file_name);

int32
gauden_dcount_read(const char **out_comment,
		   float32 ****out_dnom,
		   uint32 *out_n_mgau,
		   uint32 *out_n_feat,
		   uint32 *out_n_density,
		   const char *file_name);

int32
gauden_dcount_write(const char *file_name,
		    const char *comment,
		    float32 ***dnom,
		    uint32 n_mgau,
		    uint32 n_feat,
		    uint32 n_density);

#endif /* GAUDEN_IO_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	gauden_io.h,v $
 * Revision 1.6  97/07/16  11:39:10  eht
 * *** empty log message ***
 * 
 * Revision 1.5  1996/07/29  16:33:40  eht
 * added const attrib
 * added bundle/unbundle functions
 *
 * Revision 1.4  1996/06/20  17:41:50  eht
 * Changed make string constant include "const" in the function definition/declaration
 *
 * Revision 1.3  1996/01/26  17:55:01  eht
 * Use the const specification for the veclen (it shouldn't be written to inside this module)
 *
 * Revision 1.2  1995/10/10  13:10:34  eht
 * Changed to use <s3/prim_type.h>
 *
 * Revision 1.1  1995/08/15  13:46:15  eht
 * Initial revision
 *
 *
 */
