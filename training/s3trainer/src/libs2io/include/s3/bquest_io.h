/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libs2io/include/s3/RCS/bquest_io.h,v 1.1 96/03/25 15:54:29 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1994 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: bquest_io.h
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

#ifndef BQUEST_IO_H
#define BQUEST_IO_H

#include <s3/acmod_set.h>
#include <s3/prim_type.h>

typedef struct {
    char *qname;
    acmod_id_t *phone;
    uint32 n_phone;
} quest_t;

quest_t *
read_quest_file(const char *file_name,
		acmod_set_t *acmod_set,
		uint32 *n_quest);

#endif /* BQUEST_IO_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	bquest_io.h,v $
 * Revision 1.1  1996/03/25 15:54:29  eht
 * Initial revision
 *
 *
 */
