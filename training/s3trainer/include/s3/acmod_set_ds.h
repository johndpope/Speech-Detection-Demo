/*********************************************************************
 *
 * $Header: /usr2/eht/s3/src/libcommon/include/s3/RCS/acmod_set_ds.h,v 1.2 1995/10/09 20:55:35 eht Exp $
 *
 * Carnegie Mellon ARPA Speech Group
 *
 * Copyright (c) 1995 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: acmod_set_ds.h
 * 
 * Description: 
 *	This header file defines the data structures used by the
 * acmod_set (acoustic model set) module and its clients.
 * 
 * Author: 
 * 	Eric H. Thayer (eht@cs.cmu.edu)
 *********************************************************************/

#ifndef ACMOD_SET_DS_H
#define ACMOD_SET_DS_H

#include <s3/prim_type.h>

typedef unsigned char ci_acmod_id_t;

#define NO_CI_ACMOD	(0xff)
#define MAX_CI_ACMOD	(0xfe)

typedef uint32 acmod_id_t;

#define NO_ACMOD	(0xffffffff)	/* The ID representing no acoustic
					 * model. */
#define MAX_ACMOD	(0xfffffffe)	/* The max ID possible */

typedef enum {
    WORD_POSN_BEGIN = 0,	/* beginning phone of word */
    WORD_POSN_END = 1,		/* ending phone of word */
    WORD_POSN_SINGLE = 2,	/* single phone word (i.e. begin & end) */
    WORD_POSN_INTERNAL = 3,	/* internal phone of word */
    WORD_POSN_UNDEFINED = 4	/* undefined value, used for initial conditions, etc */
} word_posn_t;

#define N_WORD_POSN	4	/* total # of word position indicators (excluding undefined) */

/* the following map must be consistent w/ the types above.  Unpredictable
 * behaviour w/ result if not.  */

#define WORD_POSN_CHAR_MAP	"besiu"	/* b == begin, e == end, s == single-phone
					   i == word-internal, u == undefined */

/* acoustic model record (describes triphones, diphones, etc.) */
typedef struct {
    ci_acmod_id_t base;		/* The base phone associated w/ this phone */

    ci_acmod_id_t left_context;	/* The left context associated w/ this phone,
				 * if there is no specified left context, this
				 * is set to NO_ACMOD */

    ci_acmod_id_t right_context; /* The right context associated w/ this phone,
				  * This can be any CI phone id or NO_ACMOD.
				  * NO_ACMOD indicates to specified right context */

    word_posn_t posn;		 /* Word position indicator (see above) */

    /* The name is generatable from the information above */

    const char **attrib;	/* A NULL terminated list of C strings which
				 * represent various attributes of the acoustic model.
				 * For instance, "ci", "non_speech", "triphone"
				 * model. */

} acmod_t;

/* ci_acmod record (represents base phones and filler words) */
typedef struct {
    const char *name;		/* A C string representation for the acoustic model */

    const char **attrib;	/* A NULL terminated list of C strings which
				 * represent various attributes of the acoustic model.
				 * For instance, "ci", "non_speech", "triphone"
				 * model. */
    
    ci_acmod_id_t id;		/* The ID of this acoustic model */
} ci_acmod_t;

#include <s3/itree.h>

typedef struct {
    ci_acmod_t *ci;		/* base phone and filler model list.
				 * The base phone set are used to compose
				 * triphones, diphones, etc.  They are also
				 * acoustic models themselves that represent
				 * phones trained with no regard to their
				 * left and right contexts.
				 *
				 * filler models are used to represent non-speech
				 * signals such as silence (actually stationary
				 * background noise), coughs, door slams, etc. */
    uint32 n_ci;	/* # of CI acoustic models */
    uint32 max_n_ci;

    acmod_t *multi;		/* The remaining acoustic models.  Includes only
				 * triphones at the moment. */
    uint32 n_multi;
    uint32 max_n_multi;

    uint32 next_id;	/* The ID which would be assigned to the next
				 * new acoustic model */
    
    itree_t **multi_idx;	/* A quick index for mapping a multiphone to
				 * an ID. */

    char **attrib;		/* A NULL terminated list of C strings which
				 * represent all possible attributes of the phones in
				 * a phone set.
				 * For instance, "base", "non_speech", "triphone" */
    
    uint32 *n_with;	/* The number of phones having each attribute. */
} acmod_set_t;

#endif /* ACMOD_SET_DS_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log: acmod_set_ds.h,v $
 * Revision 1.2  1995/10/09  20:55:35  eht
 * Changes needed for prim_type.h
 *
 * Revision 1.1  1995/09/08  19:13:52  eht
 * Initial revision
 *
 * Revision 1.1  95/08/15  13:44:14  13:44:14  eht (Eric Thayer)
 * Initial revision
 * 
 *
 */


