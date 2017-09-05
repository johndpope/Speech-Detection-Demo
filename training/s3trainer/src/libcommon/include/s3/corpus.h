/*********************************************************************
 *
 * $Header: /usr2/eht/s3/src/libcommon/include/s3/RCS/corpus.h,v 1.2 1995/10/09 20:55:35 eht Exp $
 *
 * Carnegie Mellon ARPA Speech Group
 *
 * Copyright (c) 1995 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: corpus.h
 * 
 * Traceability: 
 * 
 * Description: 
 * 
 * Author: 
 * 	Eric H. Thayer (eht@cs.cmu.edu)
 *********************************************************************/

#ifndef CORPUS_H
#define CORPUS_H

#include <s3/prim_type.h>

/* MFCC directory/extension configuration
   functions */
int
corpus_set_mfcc_dir(const char *root);

int
corpus_set_mfcc_ext(const char *ext);

/* sent file configuration functions */
int
corpus_set_sent_dir(const char *root);

int
corpus_set_sent_ext(const char *ext);

/* Corpus LSN file configuration functions */
int
corpus_set_lsn_filename(const char *filename);


/* Control file configuration functions */
int
corpus_set_ctl_filename(const char *filename);

int
corpus_set_interval(uint32 n_skip,
		    uint32 run_len);

int
corpus_set_partition(uint32 r,
		     uint32 of_s);

uint32
corpus_get_begin();

/* Initialization function to be called after
   configuration functions */

int
corpus_init(void);

/* data access/info functions */
int
corpus_next_utt();
char *
corpus_utt_brief_name();
int
corpus_get_sent(char **trans);
int
corpus_get_mfcc(float **mfc, uint32 *n_coeff);

#endif /* CORPUS_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log: corpus.h,v $
 * Revision 1.2  1995/10/09  20:55:35  eht
 * Changes needed for prim_type.h
 *
 * Revision 1.1  1995/10/09  20:30:32  eht
 * Initial revision
 *
 * Revision 1.4  1995/09/08  19:13:52  eht
 * Updated to remove references to pset module and add references
 * to acmod_set module
 *
 * Revision 1.3  1995/08/29  12:21:00  eht
 * Interface change (part 2)
 *
 * Revision 1.2  1995/08/29  12:16:43  eht
 * Interface change
 *
 * Revision 1.1  1995/08/15  13:44:14  eht
 * Initial revision
 *
 *
 */
