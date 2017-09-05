/*********************************************************************
 *
 * $Header: /usr2/eht/s3/src/libs2io/include/s3/RCS/s2_mixing_weights.h,v 1.2 1995/10/10 13:10:34 eht Exp $
 *
 * Carnegie Mellon ARPA Speech Group
 *
 * Copyright (c) 1995 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: s2_mixing_weights.h
 * 
 * Traceability: 
 * 
 * Description: 
 * 
 * Author: 
 * 	$Author: eht $
 *********************************************************************/

#ifndef S2_MIXING_WEIGHTS_H
#define S2_MIXING_WEIGHTS_H

#include <s3/prim_type.h>

int32
s2_mixing_weights(float32 ***out_mixw,
		  const char *hmm_file_name);


#endif /* S2_MIXING_WEIGHTS_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log: s2_mixing_weights.h,v $
 * Revision 1.2  1995/10/10  13:10:34  eht
 * Changed to use <s3/prim_type.h>
 *
 * Revision 1.1  1995/08/15  13:46:15  eht
 * Initial revision
 *
 *
 */
