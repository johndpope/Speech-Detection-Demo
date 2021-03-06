/*********************************************************************
 *
 * $Header: /usr2/eht/s3/src/tiestate/RCS/main.c,v 1.1 97/07/16 11:36:22 eht Exp Locker: eht $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1994 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: main.c
 * 
 * Description: 
 * 	Tie states according to senone decision trees
 *
 * Author: 
 * 	Eric Thayer (eht@cs.cmu.edu)
 *********************************************************************/

static char rcsid[] = "@(#)$Id: main.c,v 1.1 97/07/16 11:36:22 eht Exp Locker: eht $";

#include "cmd_ln.h"

#include <s3/model_def_io.h>
#include <s3/dtree.h>
#include <s3/pset_io.h>
#include <s3/ckd_alloc.h>
#include <s3/cmd_ln.h>
#include <s3/err.h>
#include <s3/s3.h>

#include <sys_compat/file.h>

#include <assert.h>


int
init(model_def_t **out_imdef,
     pset_t **out_pset,
     uint32 *out_n_pset,
     dtree_t ****out_tree,
     uint32 *out_n_seno)
{
    model_def_t *imdef;
    model_def_t *omdef;
    uint32 p, s;
    uint32 n_ci, n_state;
    char fn[MAXPATHLEN+1];
    const char *a_fn;
    FILE *fp;
    dtree_t ***tree, *tr;
    pset_t *pset;
    uint32 n_pset;
    uint32 n_seno;
    const char *treedir;
    uint32 ts_id;

    a_fn = (const char *)cmd_ln_access("-imoddeffn");
    if (a_fn == NULL)
	E_FATAL("Specify -imoddeffn\n");
    if (model_def_read(&imdef, a_fn) != S3_SUCCESS) {
	return S3_ERROR;
    }
    *out_imdef = imdef;

    a_fn = (const char *)cmd_ln_access("-psetfn");
    E_INFO("Reading: %s\n", a_fn);
    *out_pset = pset = read_pset_file(a_fn, imdef->acmod_set, &n_pset);
    *out_n_pset = n_pset;

    n_ci = acmod_set_n_ci(imdef->acmod_set);

    treedir = (const char *)cmd_ln_access("-treedir");
    tree = (dtree_t ***)ckd_calloc(n_ci, sizeof(dtree_t **));
    *out_tree = tree;

    ts_id = imdef->n_tied_ci_state;
    for (p = 0, n_seno = 0; p < n_ci; p++) {
	if (!acmod_set_has_attrib(imdef->acmod_set, p, "filler")) {
	    n_state = imdef->defn[p].n_state;

	    tree[p] = (dtree_t **)ckd_calloc(n_state, sizeof(dtree_t *));

	    for (s = 0; s < n_state-1; s++) {
		E_INFO("%s-%u: offset %u\n",
		       acmod_set_id2name(imdef->acmod_set, p), s, ts_id);

		sprintf(fn, "%s/%s-%u.dtree",
			treedir, acmod_set_id2name(imdef->acmod_set, p), s);
		fp = fopen(fn, "r");
		if (fp == NULL) {
		    E_FATAL_SYSTEM("Unable to open %s for reading", fn);
		}
		tree[p][s] = tr = read_final_tree(fp, pset, n_pset);

		label_leaves(&tr->node[0], &ts_id);

		fclose(fp);

		n_seno += cnt_leaf(&tr->node[0]);
	    }
	}
    }

    assert(n_seno == (ts_id - imdef->n_tied_ci_state));

    E_INFO("n_seno= %u\n", ts_id);

    *out_n_seno = n_seno;

    return S3_SUCCESS;
}

main(int argc, char *argv[])
{
    model_def_t *imdef;
    model_def_t *omdef;
    pset_t *pset;
    uint32 n_pset;
    dtree_t ***tree;
    uint32 n_seno;
    uint32 n_ci;
    uint32 n_acmod;
    uint32 p;
    uint32 s;
    model_def_entry_t *idefn, *odefn;
    acmod_id_t b, l, r;
    word_posn_t wp;
    
    parse_cmd_ln(argc, argv);

    if (init(&imdef, &pset, &n_pset, &tree, &n_seno) != S3_SUCCESS)
	exit(1);

    omdef = (model_def_t *)ckd_calloc(1, sizeof(model_def_t));

    omdef->acmod_set = imdef->acmod_set; /* same set of acoustic models */

    omdef->n_total_state = imdef->n_total_state;

    omdef->n_tied_ci_state = imdef->n_tied_ci_state;
    omdef->n_tied_state = imdef->n_tied_ci_state + n_seno;

    omdef->n_tied_tmat = imdef->n_tied_tmat;

    omdef->defn = (model_def_entry_t *)ckd_calloc(imdef->n_defn,
						  sizeof(model_def_entry_t));

    /*
     * Define the context-independent models
     */
    n_ci = acmod_set_n_ci(imdef->acmod_set);
    for (p = 0; p < n_ci; p++) {
	idefn = &imdef->defn[p];
	odefn = &omdef->defn[p];
	
	odefn->p    = idefn->p;
	odefn->tmat = idefn->tmat;

	odefn->state = ckd_calloc(idefn->n_state, sizeof(uint32));
	odefn->n_state = idefn->n_state;

	for (s = 0; s < idefn->n_state; s++) {
	    if (idefn->state[s] == NO_ID)
		odefn->state[s] = NO_ID;
	    else {
		odefn->state[s] = idefn->state[s];
	    }
	}
    }

    /*
     * Define the rest of the models
     */
    n_acmod = acmod_set_n_acmod(omdef->acmod_set);
    for (; p < n_acmod; p++) {
	b = acmod_set_base_phone(omdef->acmod_set, p);

	assert(p != b);

	idefn = &imdef->defn[p];
	odefn = &omdef->defn[p];

	odefn->p    = idefn->p;
	odefn->tmat = idefn->tmat;

	odefn->state = ckd_calloc(idefn->n_state, sizeof(uint32));
	odefn->n_state = idefn->n_state;
	for (s = 0; s < idefn->n_state; s++) {
	    if (idefn->state[s] == NO_ID)
		/* Non-emitting state */
		odefn->state[s] = NO_ID;
	    else {
		/* emitting state: find the tied state */
		acmod_set_id2tri(omdef->acmod_set,
				 &b, &l, &r, &wp,
				 p);
/*
		fprintf(stderr, "%s %u ",
			acmod_set_id2name(omdef->acmod_set, p), s);
*/

		odefn->state[s] = tied_state(&tree[b][s]->node[0],
					     b, l, r, wp,
					     pset);

/*
		fprintf(stderr, "\t-> %u\n", odefn->state[s]);

		fprintf(stderr, "\n");
*/
	    }
	}
    }

    if (model_def_write(omdef, (const char *)cmd_ln_access("-omoddeffn")) != S3_SUCCESS) {
	exit(1);
    }
}

/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	main.c,v $
 * Revision 1.1  97/07/16  11:36:22  eht
 * Initial revision
 * 
 *
 */
