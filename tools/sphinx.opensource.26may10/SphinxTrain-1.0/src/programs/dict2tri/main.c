/* ====================================================================
 * Copyright (c) 1994-2000 Carnegie Mellon University.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * This work was supported in part by funding from the Defense Advanced 
 * Research Projects Agency and the National Science Foundation of the 
 * United States of America, and the CMU Sphinx Speech Consortium.
 *
 * THIS SOFTWARE IS PROVIDED BY CARNEGIE MELLON UNIVERSITY ``AS IS'' AND 
 * ANY EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL CARNEGIE MELLON UNIVERSITY
 * NOR ITS EMPLOYEES BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ====================================================================
 *
 */
/*********************************************************************
 *
 * File: main.c
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

#include "parse_cmd_ln.h"

#include <s3/lexicon.h>
#include <s3/model_def_io.h>
#include <s3/ckd_alloc.h>

#include <stdio.h>
#include <string.h>


int
main(int argc, char *argv[])
{
    const char *basephnfn;
    FILE *base_fp;
    const char *dictfn;
    lexicon_t *lex;
    acmod_set_t *tri;
    acmod_id_t p, f, *lctxt, *rctxt, l, b, r, new;
    word_posn_t posn;
    uint32 n_lctxt, n_rctxt;
    uint32 i, l_i, r_i;
    uint32 n_ci;
    lex_entry_t *e;
    const char *no_attrib[2] = { "n/a", NULL };
    const char *filler_attrib[2] = { "filler", NULL };
    char o_f = '\n';
    char b_name[128];

    parse_cmd_ln(argc, argv);

    lex = NULL;
    
    basephnfn = (const char *)cmd_ln_access("-basephnfn");
    base_fp = fopen(basephnfn, "r");
    if (base_fp == NULL) {
	E_FATAL_SYSTEM("Unable to open %s for reading\n",
		       basephnfn);
    }

    /* create a new acoustic model set mapping structure */
    tri = acmod_set_new();

    for (n_ci = 0; fgets(b_name, 128, base_fp) != NULL; n_ci++);
	 
    rewind(base_fp);

    acmod_set_set_n_ci_hint(tri, n_ci);

    for (p = 0; fgets(b_name, 128, base_fp) != NULL; p++) {
	b_name[strlen(b_name)-1] = '\0';
	if ((strcmp(b_name, "SIL") == 0) ||
	    (strcmp(b_name, "SILb") == 0) ||
	    (strcmp(b_name, "SILe") == 0) ||
	    ((b_name[0] == '+') && (b_name[strlen(b_name)-1] == '+'))) {
	    acmod_set_add_ci(tri, b_name, filler_attrib);
	}
	else {
	    acmod_set_add_ci(tri, b_name, no_attrib);
	}
    }
	
    dictfn = (const char *)cmd_ln_access("-dictfn");
    E_INFO("Reading: %s\n", dictfn);
    lex = lexicon_read(lex, dictfn, tri);
    if (lex == NULL) {
	E_FATAL_SYSTEM("Unable to read lexicon file %s", dictfn);
    }

    E_INFO("Determining full triphone list\n");

    /* set the # of CI phones in the new set */
    n_ci = acmod_set_n_ci(tri);

    rctxt = ckd_calloc(n_ci, sizeof(acmod_id_t));
    lctxt = ckd_calloc(n_ci, sizeof(acmod_id_t));

    /* set an upper bound on the # of triphones */
    acmod_set_set_n_tri_hint(tri, 200000);

    /* Allow possibility of sil to the left or right of any word */
    lctxt[0] = acmod_set_name2id(tri, "SIL");
    rctxt[0] = acmod_set_name2id(tri, "SIL");
    n_lctxt = n_rctxt = 1;

    if (*(uint32 *)cmd_ln_access("-btwtri")) {
	for (e = lex->head; e; e = e->next) {
	    f = e->ci_acmod_id[0];
	    l = e->ci_acmod_id[e->phone_cnt-1];
	
	    if (!acmod_set_has_attrib(tri, f, "filler")) {
		for (i = 0; i < n_rctxt; i++)
		    if (f == rctxt[i]) break;

		if (i == n_rctxt) {
		    rctxt[i] = f;
		    ++n_rctxt;
		}
	    }
	    
	    if (!acmod_set_has_attrib(tri, l, "filler")) {
		for (i = 0; i < n_lctxt; i++)
		    if (l == lctxt[i]) break;
		
		if (i == n_lctxt) {
		    lctxt[i] = l;
		    ++n_lctxt;
		}
	    }
	}
    }

    E_INFO("%u word begin left context:", n_lctxt);
    for (i = 0; i < n_lctxt; i++) {
	E_INFOCONT(" %s", acmod_set_id2name(tri, lctxt[i]));
    }
    E_INFOCONT("\n");

    E_INFO("%u word end right context:", n_rctxt);
    for (i = 0; i < n_rctxt; i++) {
	E_INFOCONT(" %s", acmod_set_id2name(tri, rctxt[i]));
    }
    E_INFOCONT("\n");

    for (e = lex->head; e; e = e->next) {
	if (o_f != e->ortho[0]) {
	    o_f = e->ortho[0];
	    E_INFO("Doing %c\n", o_f);
	}
	if (e->phone_cnt == 1) {
	    /* single phone words */
	    b = e->ci_acmod_id[0];
	    if (!acmod_set_has_attrib(tri, b, "filler")) {
		posn = WORD_POSN_SINGLE;
		for (l_i = 0; l_i < n_lctxt; l_i++) {
		    for (r_i = 0; r_i < n_rctxt; r_i++) {
			l = lctxt[l_i];
			r = rctxt[r_i];
			if (acmod_set_tri2id(tri,
					     b, l, r, posn) == NO_ACMOD) {
			    new = acmod_set_add_tri(tri,
						    b, l, r, posn, no_attrib);
			    printf("%s\n", acmod_set_id2name(tri, new));
			}
		    }
		}
	    }
	}
	else {
	    b = e->ci_acmod_id[0];
	    if (!acmod_set_has_attrib(tri, b, "filler")) {
		r = e->ci_acmod_id[1];
		posn = WORD_POSN_BEGIN;
		for (l_i = 0; l_i < n_lctxt; l_i++) {
		    l = lctxt[l_i];
		    if (acmod_set_tri2id(tri,
					 b, l, r, posn) == NO_ACMOD) {
			new = acmod_set_add_tri(tri,
						b, l, r, posn, no_attrib);
			printf("%s\n", acmod_set_id2name(tri, new));
		    }
		}
	    }

	    posn = WORD_POSN_INTERNAL;
	    for (i = 1; i < e->phone_cnt-1; i++) {
		b = e->ci_acmod_id[i];
		if (!acmod_set_has_attrib(tri, b, "filler")) {
		    r = e->ci_acmod_id[i+1];
		    l = e->ci_acmod_id[i-1];
		    if (acmod_set_tri2id(tri,
					 b, l, r, posn) == NO_ACMOD) {
			new = acmod_set_add_tri(tri,
						b, l, r, posn, no_attrib);
			printf("%s\n", acmod_set_id2name(tri, new));
		    }
		}
	    }

	    b = e->ci_acmod_id[e->phone_cnt-1];
	    if (!acmod_set_has_attrib(tri, b, "filler")) {
		l = e->ci_acmod_id[e->phone_cnt-2];
		posn = WORD_POSN_END;
		for (r_i = 0; r_i < n_rctxt; r_i++) {
		    r = rctxt[r_i];
		    if (acmod_set_tri2id(tri,
					 b, l, r, posn) == NO_ACMOD) {
			new = acmod_set_add_tri(tri,
						b, l, r, posn, no_attrib);
			printf("%s\n", acmod_set_id2name(tri, new));
		    }
		}
	    }
	}
    }
    return 0;
}

/*
 * Log record.  Maintained by RCS.
 *
 * $Log$
 * Revision 1.5  2004/07/21  18:30:34  egouvea
 * Changed the license terms to make it the same as sphinx2 and sphinx3.
 * 
 * Revision 1.4  2004/06/17 19:17:22  arthchan2003
 * Code Update for silence deletion and standardize the name for command -line arguments
 *
 * Revision 1.3  2001/04/05 20:02:31  awb
 * *** empty log message ***
 *
 * Revision 1.2  2000/09/29 22:35:14  awb
 * *** empty log message ***
 *
 * Revision 1.1  2000/09/24 21:38:31  awb
 * *** empty log message ***
 *
 * Revision 1.1  97/07/16  11:36:22  eht
 * Initial revision
 * 
 *
 */
