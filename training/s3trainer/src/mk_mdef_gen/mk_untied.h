#ifndef MK_UNTIED_H
#define MK_UNTIED_H


#include <s3/s3.h>
#include "heap.h"
#include "hash.h"

int32 make_ci_list_frm_mdef(char  *mdeffile,
                            char  ***CIlist,
                            int32 *cilistsize);

int32 make_ci_list_cd_hash_frm_phnlist(char  *phnlist,
                                    char  ***CIlist,
                                    int32 *cilistsize,
                                    hashelement_t ***CDhash,
                                    int32 *NCDphones);

int32 make_ci_list_cd_hash_frm_mdef(char  *mdeffile,
                                    char  ***CIlist,
                                    int32 *cilistsize,
                                    hashelement_t ***CDhash,
                                    int32 *NCDphones);

int32  read_dict(char *dictfile, char *fillerdictfile,
		 dicthashelement_t ***dicthash);

int32 make_mdef_from_list(char *mdeffile,
			char **CIlist, 
                        int32 cilistsize,
                        heapelement_t **CDheap,
                        int32 cdheapsize,
                        char  *pgm);

int32 make_dict_triphone_list (dicthashelement_t **dicthash,
                          hashelement_t ***triphonehash);

int32 make_CD_heap(hashelement_t  **triphonehash,
                   int32  threshold,
                   heapelement_t ***CDheap,
                   int32  *cdheapsize);

int32 find_threshold(hashelement_t  **triphonehash);

int32  count_triphones (char *transfile,
                        dicthashelement_t **dicthash,
                        hashelement_t **tphnhash,
			phnhashelement_t ***CIhash);

int32   print_counts(char *countfn, phnhashelement_t  **CIhash,
                     hashelement_t **CDhash);
#endif
