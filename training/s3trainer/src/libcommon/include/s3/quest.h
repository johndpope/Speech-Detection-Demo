/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libcommon/include/s3/RCS/quest.h,v 1.2 97/07/16 11:39:10 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1996 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: quest.h
 * 
 * Description: 
 * 	This file defines the data structures used to implement
 *	the state-tying decision tree questions.
 *
 * Author: 
 * 	Eric Thayer (eht@cs.cmu.edu)
 *********************************************************************/

#ifndef QUEST_H
#define QUEST_H

#include <s3/prim_type.h>
#include <s3/acmod_set.h>

#include <stdio.h>


typedef struct {
    char *name;		/* name of the phone set (e.g. sonor, vowel, etc.) */
    acmod_id_t *phone;	/* list of member phones */
    uint32 n_phone;	/* cardinality of the set */
    /*
     * Exactly one of the following must be non-NULL for all questions
     */
    uint32 *member;	/* member[p] == TRUE if p is in the phone[] list */
    uint32 *posn;	/* posn[pn] == TRUE if position pn satisfies the query */
} pset_t;

/*
 * Simple Question
 *
 * Simple questions are of the form:
 *	(context[-1] \isin SONOR)
 *	(context[1] \isin LIQUID)
 * 	etc.
 *
 * where SONOR and LIQUID are phone sets derived,
 * for instance, linguistically.
 */
typedef struct {
    int32 ctxt;		/* .
			   .
			   -2 => two phones to left
			   -1 => one phone to left
			    1 => one phone to right
			    2 => two phones to right
			   .
			   .
			   */
    uint32 neg;		/* TRUE if question q is to be negated */
    uint32 pset;	/* phone set id (index into master list) */

    /*
     * Exactly one of the following must be non-NULL for all questions
     */
    uint32 *member;	/* member[phone] is true if phone is a
			   member of the phone class */
    uint32 *posn;	/* posn[pn] == TRUE if position pn satisfies the query */
} quest_t;

/*
 * Compound question:
 *
 * Compound questions are boolean expressions in
 * standard sum of products form (where + == OR and * == AND).
 * where the boolean variables are instances of simple
 * questions above.
 *
 * For some compound question Q, Q.conj_q[i] is the
 * the ith term in the sum.  Q.conj_q[i][j] is the
 * jth simple question in the ith term of the
 * product.  The # of terms in each product is
 * found by Q.prod_len[i] and the total # of terms
 * in the sum is Q.sum_len.
 */
typedef struct {
    quest_t **conj_q;
    uint32 *prod_len;
    uint32 sum_len;
} comp_quest_t;

/*
 * Public Fns
 */

int
s3parse_comp_quest(pset_t *pset,
		   uint32 n_pset,
		   comp_quest_t *q,
		   char *q_str);

void
parse_compound_q(comp_quest_t *q,
		 char *q_str);

char *
parse_conj(quest_t **term,
	   uint32 *n_simple_q,
	   char *q_str);

void
print_comp_quest(FILE *fp,
		 pset_t *pset,
		 comp_quest_t *q);

void
print_quest(FILE *fp,
	    pset_t *pset,
	    quest_t *q);

uint32
cnt_q_term(char *q_str);

int
eval_comp_quest(comp_quest_t *q,
		uint32 *feat,
		uint32 n_feat);

int
eval_quest(quest_t *q,
	   uint32 *feat,
	   uint32 n_feat);

/* simplify the conjunctions of
 * a composite question by removing
 * sets that are supersets of other terms
 * in the conjunction.
 */

int
simplify_comp_quest(comp_quest_t *q,
		    uint32 n_phone);

/*
 * Private Fns
 */

static void
parse_simple_q(quest_t *q,
	       char *q_str);

#endif /* QUEST_H */ 


/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	quest.h,v $
 * Revision 1.2  97/07/16  11:39:10  eht
 * *** empty log message ***
 * 
 * Revision 1.1  96/03/25  15:50:59  eht
 * Initial revision
 * 
 *
 */
