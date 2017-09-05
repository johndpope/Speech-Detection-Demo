/* ------------------ HEAP.H ---------------------*/
#ifndef HEAP_H
#define HEAP_H

#include <stdlib.h>
#include <s3/prim_type.h>

#define MAXSWAPS 1000 	/* Dont expect more than a 1000 swaps */

typedef struct heapelement_t {
    char *basephone;
    char *leftcontext;
    char *rightcontext;
    char *wordposition;
    int32 count;
    int32 heapid;
} heapelement_t;

#define PARENTID(X)     ((int32)((X)-1)/2)
#define LEFTCHILD(X)    ((X)*2+1)
#define RIGHTCHILD(X)   ((X)*2+2)
#define SWAP(A,B)  {heapelement_t *tmp; int32 x,y; \
                    x = (A)->heapid; y = (B)->heapid; \
                    tmp = (A); A = B; B = tmp; \
                    (A)->heapid = x; (B)->heapid = y;}
int32 insert (heapelement_t ***heap, int32 heapsize, heapelement_t *newelement);
heapelement_t *yanktop (heapelement_t ***heap, int32 heapsize, int32 *newsize);
void free_heap (heapelement_t **heap, int32 heapsize); 
void free_heapelement (heapelement_t *heapel);

#endif
