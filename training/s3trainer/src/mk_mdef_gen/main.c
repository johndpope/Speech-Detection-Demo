/*********************************************************************
 * Copyright (c) 2000 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 * Desription:
 *    Multi-function routine to generate mdef for context-independent
 *    training, untied training, and all-triphones mdef for state tying.
 * Flow:
 *    if (triphonelist) make CI phone list and CD phone list
 *	  if alltriphones mdef needed, make mdef
 *    if (rawphonelist) Make ci phone list, 
 *        if cimdef needed, make mdef
 *        Generate alltriphones list from dictionary
 *        if alltriphones mdef needed, make mdef
 *    if neither triphonelist or rawphonelist quit
 *    Count triphones and triphone types in transcript
 *    Adjust threshold according to min-occ and maxtriphones
 *    Prune triphone list
 *    Make untied mdef
 *
 *
 * Author: 
 *    Rita Singh (rsingh@cs.cmu.edu) 
 * Date: 08 Dec. 2000
 *********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <s3/s3.h>
#include <s3/cmd_ln.h>
#include <s3/ckd_alloc.h>
#include "parse_cmd_ln.h"
#include "heap.h"
#include "hash.h"
#include "mk_untied.h"

main (int argc, char **argv)
{
    heapelement_t **CDheap=NULL;
    hashelement_t **CDhash=NULL;
    phnhashelement_t **CIhash=NULL;
    dicthashelement_t **dicthash=NULL;
    int32  cilistsize=0, cdheapsize=0, threshold, tph_list_given, ncd;
    char   *phnlist, *incimdef, *triphnlist, *incdmdef;
    char   *lsnfile, *dictfn, *fillerdictfn, **CIlist=NULL, *mdeffn;
    char   *cimdeffn, *alltphnmdeffn, *untiedmdeffn, *countfn;

    parse_cmd_ln(argc,argv);

    /* Test all flags before beginning */
    cimdeffn = (char *)cmd_ln_access("-ocimdef");
    alltphnmdeffn = (char *)cmd_ln_access("-oalltphnmdef");
    untiedmdeffn = (char *)cmd_ln_access("-ountiedmdef");
    countfn = (char *)cmd_ln_access("-ocountfn");

    if (cimdeffn) E_INFO("Will write CI mdef file %s\n",cimdeffn);
    if (alltphnmdeffn) 
	E_INFO("Will write alltriphone mdef file %s\n",alltphnmdeffn);
    if (untiedmdeffn) E_INFO("Will write untied mdef file %s\n",untiedmdeffn);
    if (countfn) E_INFO("Will write triphone counts file %s\n",countfn);

    if (!cimdeffn && !alltphnmdeffn && !untiedmdeffn && !countfn)
	E_FATAL("No output mdef files or count files specified!\n");

    dictfn = (char *)cmd_ln_access("-dictfn");
    fillerdictfn = (char *)cmd_ln_access("-fdictfn");
    lsnfile = (char*)cmd_ln_access("-lsnfn");
    if ((untiedmdeffn || countfn) && (!lsnfile || !dictfn)) {
	E_WARN("Either dictionary or transcript file not given!\n"); 
  	if (untiedmdeffn) E_WARN("Untied mdef will not be made\n");
  	if (countfn) E_WARN("Phone counts will not be generated\n");
	untiedmdeffn = countfn = NULL;
    }

    phnlist = (char *)cmd_ln_access("-phnlstfn");
    triphnlist = (char *)cmd_ln_access("-triphnlstfn");
    incimdef = (char *)cmd_ln_access("-inCImdef");
    incdmdef = (char *)cmd_ln_access("-inCDmdef");
    if (!incdmdef && !incimdef && !phnlist && !triphnlist)
	E_FATAL("No input mdefs or phone list given\n");
    if (triphnlist) {
	if (phnlist) 
	    E_WARN("Both -triphnlist %s and -phnlist given.\n",triphnlist);
	    E_WARN("Ignoring -phnlist %s\n",phnlist);
        phnlist = triphnlist;
    }
    tph_list_given =  (triphnlist || incdmdef) ? 1 : 0;

    if (incdmdef) {
	if (incimdef || phnlist){
	    E_WARN("Using only input CD mdef %s!\n",incdmdef);
	    E_WARN("Using only triphones from input CD mdef %s!\n",incdmdef);
	    if (incimdef) E_WARN("CImdef %s will be ignored\n",incimdef);
	    if (phnlist) E_WARN("phonelist %s will be ignored\n",phnlist);
	    incimdef = phnlist = NULL; 
	}
	make_ci_list_cd_hash_frm_mdef(incdmdef,&CIlist,&cilistsize,
							   &CDhash,&ncd);
    }
    else{
        if (phnlist)
	    make_ci_list_cd_hash_frm_phnlist(phnlist,&CIlist,
						&cilistsize,&CDhash,&ncd);
	if (incimdef) {
	    if (CIlist) ckd_free_2d((void**)CIlist);
	    make_ci_list_frm_mdef(incimdef,&CIlist,&cilistsize);
        }
    }
    if (cimdeffn) 
	make_mdef_from_list(cimdeffn,CIlist,cilistsize,NULL,0,argv[0]);

    if ((alltphnmdeffn && !tph_list_given) || untiedmdeffn || countfn)
	read_dict(dictfn, fillerdictfn, &dicthash);
    else if (!alltphnmdeffn) return 0; /* Nothing to be done */

    if (!tph_list_given){
	if (CDhash) freehash(CDhash);
	make_dict_triphone_list (dicthash, &CDhash);
    }

    if (alltphnmdeffn){
	threshold = -1;
	make_CD_heap(CDhash,threshold,&CDheap,&cdheapsize);
    	make_mdef_from_list(alltphnmdeffn,CIlist,cilistsize,
					CDheap,cdheapsize,argv[0]);
    }
    if (countfn || untiedmdeffn) 
        count_triphones(lsnfile, dicthash, CDhash, &CIhash);
    if (countfn){
	print_counts(countfn,CIhash,CDhash);
    }
    if (untiedmdeffn){
        threshold = find_threshold(CDhash);
        make_CD_heap(CDhash,threshold,&CDheap,&cdheapsize);
        make_mdef_from_list(untiedmdeffn,CIlist,cilistsize,
			    CDheap,cdheapsize,argv[0]);
    }
    return 0;
}
