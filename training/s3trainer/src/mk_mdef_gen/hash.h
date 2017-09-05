/********************************************************************
 * Copyright (c) 2000 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 * Hash functions
 * Hash values are from the following set of "good" hash values 
 * 101, 211, 307, 401, 503, 601, 701, 809, 907, 1009, 1201, 1601, 2003, 
 * 2411, 3001, 4001, 5003, 6007, 7001, 8009, 9001, 10007, 12007, 16001, 
 * 20011, 24001, 30011, 40009, 50021, 60013, 70001, 80021, 90001, 100003, 
 * 120011, 160001, 200003, 240007, 300007, 400009, 500009, 600011, 700001, 
 * 800011, 900001
 *
 * Author: Rita Singh
 *********************************************************************/

#ifndef HASH_H
#define HASH_H

#include <s3/prim_type.h>

#define EOLN  -1
#define HASHSIZE 	24001
#define DICTHASHSIZE 	10007
#define PHNHASHSIZE 	101

typedef struct hashelement_t
{
    char      *basephone;
    char      *leftcontext;
    char      *rightcontext;
    char      *wordposition;
    int32     dictcount;
    int32     count;
    struct hashelement_t  *next;
} hashelement_t;


typedef struct dicthashelement_t
{
    char      *word;
    char      **phones;
    int32     nphns;
    struct dicthashelement_t  *next;
} dicthashelement_t;


typedef struct phnhashelement_t
{
    char      *phone;
    int32     count;
    struct phnhashelement_t  *next;
} phnhashelement_t;


hashelement_t *lookup(char *basephone,
		      char *lctxt,
		      char *rctxt,
		      char *wordposn,
		      hashelement_t **tphnhash);

hashelement_t *install(char *basephone,
		       char *lctxt,
		       char *rctxt,
		       char *wordposn,
		       hashelement_t **tphnhash);

void freehash(hashelement_t **hash);


dicthashelement_t *dictlookup(char *word, dicthashelement_t **dicthash);
dicthashelement_t *dictinstall(char *dictword, dicthashelement_t **dicthash);
void freedicthash(dicthashelement_t **dicthash);

phnhashelement_t *phninstall(char *phone, phnhashelement_t **lhash);
void freephnhash(phnhashelement_t **ephnhash);

#endif
