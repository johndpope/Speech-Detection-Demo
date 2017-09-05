/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libcommon/RCS/mk_phone_seq.c,v 1.1 97/07/16 11:36:22 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1996 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: mk_phone_seq.c
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

static char rcsid[] = "@(#)$Id: mk_phone_seq.c,v 1.1 97/07/16 11:36:22 eht Exp $";

#include <s3/mk_phone_seq.h>

#include <s3/mk_wordlist.h>
#include <s3/mk_phone_list.h>
#include <s3/cvt2triphone.h>
#include <s3/corpus.h>
#include <s3/ckd_alloc.h>
#include <s3/err.h>
#include <s3/s3.h>

#include <stdio.h>

int
mk_phone_seq(acmod_id_t **out_phone,
	     uint32 *out_n_phone,
	     char *trans,
	     acmod_set_t *acmod_set,
	     lexicon_t *lex)
{
    char **word;
    uint32 n_word;
    uint32 n_phone;
    acmod_id_t *phone, p;
    char *btw_mark;

    word = mk_wordlist(trans, &n_word);
    phone = mk_phone_list(&btw_mark, &n_phone, word, n_word, lex);
    if (phone == NULL) {
	E_WARN("Unable to produce CI phones for utt %s.\n", corpus_utt());
    }

    cvt2triphone(acmod_set, phone, btw_mark, n_phone);

    ckd_free(btw_mark);
    ckd_free(word);

    *out_phone = phone;
    *out_n_phone = n_phone;

    return S3_SUCCESS;
}

/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	mk_phone_seq.c,v $
 * Revision 1.1  97/07/16  11:36:22  eht
 * Initial revision
 * 
 *
 */
