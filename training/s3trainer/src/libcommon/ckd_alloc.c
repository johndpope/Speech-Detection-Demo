/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libcommon/RCS/ckd_alloc.c,v 1.8 97/07/16 11:36:22 eht Exp $
 *
 * Carnegie Mellon ARPA Speech Group
 *
 * Copyright (c) 1994 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * file: ckd_alloc.c
 * 
 * traceability: 
 * 
 * description: 
 * 
 * author: 
 * 
 *********************************************************************/

static char rcsid[] = "@(#)$Id: ckd_alloc.c,v 1.8 97/07/16 11:36:22 eht Exp $";

#include <s3/ckd_alloc.h>
#include <s3/s3.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *__ckd_salloc(char *string,
		   char *caller_file, int caller_line)
{
    char *out;

    out = malloc(strlen(string) + 1);
    if (out == NULL) {
	E_FATAL("malloc failed for string %s from %s(%d)\n",
		string, caller_file, caller_line);
    }

    strcpy(out, string);

    return out;
}


void *__ckd_calloc(size_t n_elem, size_t elem_size,
		   char *caller_file, int caller_line)
{
    void *mem=NULL;

    mem = calloc(n_elem, elem_size);
    if (mem == NULL) {
	E_FATAL("Calloc failed from %s(%d)\n",
		caller_file, caller_line);
    }

    return mem;
}

void *__ckd_malloc(size_t size,
		   char *caller_file, int caller_line)
{
    void *mem = malloc(size);

    if (mem == NULL) {
	E_FATAL("malloc failed %s(%d)\n",
		caller_file, caller_line);
    }

    return mem;
}


void *__ckd_realloc(void *ptr, size_t new_size,
		  char *caller_file, int caller_line)
{
    void *mem;

    mem = realloc(ptr, new_size);

    if (mem == NULL) {
	E_FATAL("realloc failed %s(%d)\n",
		caller_file, caller_line);
    }

    return mem;
}

void __ckd_free(void *ptr, char *file, int line)
{
    free(ptr);
}

void **
__ckd_calloc_2d(size_t d1,
		size_t d2,
		size_t elem_size,
		char *file,
		int line)
{
    void *store;
    void **out;
    size_t i, j;
    
    store = calloc(d1 * d2, elem_size);
    if (store == NULL) {
	E_FATAL("ckd_calloc_2d failed for caller at %s(%d) at %s(%d)\n",
		file, line, __FILE__, __LINE__);
    }
    
    out = calloc(d1, sizeof(void *));
    if (out == NULL) {
	E_FATAL("ckd_calloc_2d failed for caller at %s(%d) at %s(%d)\n",
		file, line, __FILE__, __LINE__);
    }
    
    for (i = 0, j = 0; i < d1; i++, j += d2) {
	out[i] = &((char *)store)[j*elem_size];
    }

    return out;
}

void
__ckd_free_2d(void **arr, char *file, int line)
{
    __ckd_free(arr[0], file, line);

    __ckd_free(arr, file, line);
}

void ***
__ckd_calloc_3d(size_t d1,
		size_t d2,
		size_t d3,
		size_t elem_size,
		char *file,
		int line)
{
    void *store;
    void **tmp1;
    void ***out;
    size_t i, j;
    
    store = calloc(d1 * d2 * d3, elem_size);
    if (store == NULL) {
	E_FATAL("ckd_calloc_3d failed for caller at %s(%d) at %s(%d)\n",
		file, line, __FILE__, __LINE__);
    }
    
    tmp1 = calloc(d1 * d2, sizeof(void *));
    if (tmp1 == NULL) {
	E_FATAL("ckd_calloc_3d failed for caller at %s(%d) at %s(%d)\n",
		file, line, __FILE__, __LINE__);
    }

    out = calloc(d1, sizeof(void **));
    if (out == NULL) {
	E_FATAL("ckd_calloc_3d failed for caller at %s(%d) at %s(%d)\n",
		file, line, __FILE__, __LINE__);
    }
    
    for (i = 0, j = 0; i < d1*d2; i++, j += d3) {
	tmp1[i] = &((char *)store)[j*elem_size];
    }

    for (i = 0, j = 0; i < d1; i++, j += d2) {
	out[i] = &tmp1[j];
    }

    return out;
}

void
__ckd_free_3d(void ***arr, char *file, int line)
{
    __ckd_free(arr[0][0], file, line);

    __ckd_free(arr[0], file, line);
    __ckd_free(arr, file, line);
}

void ****
__ckd_calloc_4d(size_t d1,
		size_t d2,
		size_t d3,
		size_t d4,
		size_t elem_size,
		char *file,
		int line)
{
    void *store;
    void **tmp1;
    void ***tmp2;
    void ****out;
    size_t i, j;

    store = calloc(d1 * d2 * d3 * d4, elem_size);
    if (store == NULL) {
	E_FATAL("ckd_calloc_4d failed for caller at %s(%d) at %s(%d)\n",
		file, line, __FILE__, __LINE__);
    }
    
    tmp1 = calloc(d1 * d2 * d3, sizeof(void *));
    if (tmp1 == NULL) {
	E_FATAL("ckd_calloc_4d failed for caller at %s(%d) at %s(%d)\n",
		file, line, __FILE__, __LINE__);
    }

    tmp2 = ckd_calloc(d1 * d2, sizeof(void **));
    if (tmp2 == NULL) {
	E_FATAL("ckd_calloc_4d failed for caller at %s(%d) at %s(%d)\n",
		file, line, __FILE__, __LINE__);
    }

    out = ckd_calloc(d1, sizeof(void ***));
    if (out == NULL) {
	E_FATAL("ckd_calloc_4d failed for caller at %s(%d) at %s(%d)\n",
		file, line, __FILE__, __LINE__);
    }
    
    for (i = 0, j = 0; i < d1*d2*d3; i++, j += d4) {
	tmp1[i] = &((char *)store)[j*elem_size];
    }

    for (i = 0, j = 0; i < d1*d2; i++, j += d3) {
	tmp2[i] = &tmp1[j];
    }

    for (i = 0, j = 0; i < d1; i++, j += d2) {
	out[i] = &tmp2[j];
    }

    return out;
}

void
__ckd_free_4d(void ****arr, char *file, int line)
{
    /* free the underlying store */
    __ckd_free(arr[0][0][0], file, line);

    /* free the access overhead */
    __ckd_free(arr[0][0], file, line);
    __ckd_free(arr[0], file, line);
    __ckd_free(arr, file, line);
}


/* Layers a 3d array access structure over a preallocated storage area */
void ***
__ckd_alloc_3d_ptr(int d1,
		   int d2,
		   int d3,
		   void *store,
		   size_t elem_size,
		   char *file,
		   int line)
{
    void **tmp1;
    void ***out;
    int i, j;
    
    tmp1 = __ckd_calloc(d1 * d2, sizeof(void *), file, line);

    out  = __ckd_calloc(d1, sizeof(void **), file, line);
    
    for (i = 0, j = 0; i < d1*d2; i++, j += d3) {
	tmp1[i] = &((char *)store)[j*elem_size];
    }
    
    for (i = 0, j = 0; i < d1; i++, j += d2) {
	out[i] = &tmp1[j];
    }
    
    return out;
}

void **
__ckd_alloc_2d_ptr(int d1,
		   int d2,
		   void *store,
		   size_t elem_size,
		   char *file,
		   int line)
{
    void **out;
    int i, j;
    
    out = (void **)__ckd_calloc(d1, sizeof(void *), file, line);
    
    for (i = 0, j = 0; i < d1; i++, j += d2) {
	out[i] = &((char *)store)[j*elem_size];
    }

    return out;
}

/*
 * Log record.  Maintained by CVS.
 *
 * $Log:	ckd_alloc.c,v $
 * Revision 1.8  97/07/16  11:36:22  eht
 * *** empty log message ***
 * 
 * Revision 1.7  96/06/17  14:30:51  eht
 * Added allocation functions for multidimensioned arrays
 * 
 * Revision 1.6  1996/03/06  20:33:56  eht
 * Include some function prototypes
 *
 * Revision 1.5  1996/02/02  17:49:04  eht
 * Add ckd_salloc() function.
 *
 * Revision 1.4  1995/10/09  15:02:03  eht
 * Changed ckd_alloc interface to get rid of __FILE__, __LINE__ arguments
 *
 * Revision 1.3  1995/10/05  12:56:42  eht
 * Get rid of debugging stuff since there are malloc packages
 * out there that have tons of this stuff.
 *
 * Revision 1.2  1995/06/02  14:52:54  eht
 * Use pwp's error printing package
 *
 * Revision 1.1  1995/02/13  15:47:16  eht
 * Initial revision
 *
 *
 */
