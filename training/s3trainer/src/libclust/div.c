/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libclust/RCS/div.c,v 1.1 97/07/16 11:36:22 eht Exp Locker: eht $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1997 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: div.c
 * 
 * Description: 
 *    This file contains a collection of entropy and divergence
 *    measures of discrete
 *    distributions.
 *
 *    This package expects that its arguments are acceptable to the
 *    log() package.  See individual pre-condition sections for the
 *    details for a given function.
 *
 * Author: 
 * 	Eric Thayer (eht@cs.cmu.edu)
 *********************************************************************/

static char rcsid[] = "@(#)$Id: div.c,v 1.1 97/07/16 11:36:22 eht Exp Locker: eht $";

#include <math.h>

#include <s3/div.h>

#include <assert.h>

static float64 ln2lg2 = 1.442695041;	/* conversion factor for bits-of-info */

float64
wt_ent_inc(float32 **a,
	   float64 *a_dnom,
	   float32 **b,
	   float64 *b_dnom,
	   float32 **a_b,
	   uint32 n_stream,
	   uint32 n_density)
{
    int32 j;
    float64 wtentdec;

    for (j = 0, wtentdec = 0; j < n_stream; j++) {
	wtentdec += a_dnom[j] * div_d(a[j], a_b[j], n_density);
	wtentdec += b_dnom[j] * div_d(b[j], a_b[j], n_density);
    }

    return wtentdec;
}


/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	metric.c,v $
 * Revision 1.1  97/07/16  11:36:22  eht
 * Initial revision
 * 
 *
 */











/*
 * Description:
 *	This routine computes the entropy of a given discrete
 *	pdf.
 *
 * Arguments:
 *	float32 *p
 *		A discrete pdf
 *	uint32   n
 *		The length of argument p.
 *
 * Returns:
 *	The entropy of the pdf p in bits.
 *
 * Pre-conditions:
 *	p[i] must be a valid arg to log() for all i in 0..(n-1)
 */
float64
ent_d(float32 *p, uint32 n)
{
    uint32 k;
    float64 y;

    for (k = 0, y = 0; k < n; k++) {
	assert(p[k] != 0);
	
	/* if (p[k] > 1e-8) No need to floor so high */
	if (p[k] > 0)
	    y -= p[k] * log(p[k]);
    }

    return y * ln2lg2;
}
/*
 * Description:
 *	This routine computes the entropy of a given discrete model
 *	state that may use multiple independent input streams.
 *
 * Arguments:
 *	float32 **p
 *		A discrete pdf for a model state
 *	uint32   f
 *		The # of independent feature streams
 *	uint32   n
 *		The # of codewords
 *
 * Returns:
 *	The entropy of the discrete state output probability p in bits.
 *
 * Pre-conditions:
 *	p[j][k] must be a valid arg to log() for all j in 0..(f-1),
 *		k in 0..(n-1)
 *
 */

float64
ent_d_state(float32 **p, uint32 f, uint32 n)
{
    uint32 j;
    float64 y;

    for (j = 0, y = 0; j < f; j++) {
	y += ent_d(p[j], n);
    }

    return y;
}

/*
 * Description:
 *	This routine computes the directed divergence (similarity metric)
 *	of two discrete pdf's.
 *
 * Arguments:
 *	float32 *p
 *		A discrete pdf
 *	float32 *q
 *		A discrete pdf
 *	uint32   n
 *		The length of arguments p and q.
 *
 * Returns:
 *	The directed divergence of p and q.
 *
 * Pre-conditions:
 *	p[i]/q[i] must be a valid arg to log() for all i in 0..(n-1)
 */
float64
ddiv_d(float32 *p, float32 *q, uint32 n)
{
    uint32 k;
    float64 y;

    for (k = 0, y = 0; k < n; k++) {
	assert(q[k] != 0);

        if (p[k] > 0)
	    y += p[k] * log(p[k] / q[k]);
    }

    return y * ln2lg2;
}

/*
 * Description:
 *	This routine computes a symmetric divergence (similarity metric)
 *	of two discrete pdf's.
 *
 * Arguments:
 *	float32 *p
 *		A discrete pdf
 *	float32 *q
 *		A discrete pdf
 *	uint32   n
 *		The length of arguments p and q.
 *
 * Returns:
 *	A symmetric divergence of p and q.
 *
 * Pre-conditions:
 *	p[i]/q[i] must be a valid arg to log() for all i in 0..(n-1)
 */
float64
div_d(float32 *p, float32 *q, uint32 n)
{
    return ddiv_d(p, q, n) + ddiv_d(q, p, n);
}

/*
 * Description:
 *	This routine computes a symmetric divergence (similarity metric)
 *	of two discrete output probabilities.
 *
 * Arguments:
 *	float32 *p
 *		A discrete pdf
 *	float32 *q
 *		A discrete pdf
 *	uint32   f
 *		The # of independent feature streams
 *	uint32   n
 *		The # of codewords
 *
 * Returns:
 *	A symmetric divergence of output probabilities p and q.
 *
 * Pre-conditions:
 *	p[j][k]/q[j][k] must be a valid arg to log() for all j in 0..(f-1),
 *		and k in 0..(n-1),
 */
float64
div_d_state(float32 **p, float32 **q, uint32 f, uint32 n)
{
    uint32 j;
    float64 y;

    for (j = 0, y = 0; j < f; j++) {
	y += div_d(p[j], q[j], n);
    }

    return y;
}

/*
 * Description:
 *	This routine computes the directed divergence (similarity metric)
 *	of two discrete output probabilities.
 *
 * Arguments:
 *	float32 *p
 *		A discrete pdf
 *	float32 *q
 *		A discrete pdf
 *	uint32   f
 *		The # of independent feature streams
 *	uint32   n
 *		The # of codewords
 *
 * Returns:
 *	The directed divergence of output probabilities p and q.
 *
 * Pre-conditions:
 *	p[j][k]/q[j][k] must be a valid arg to log() for all j in 0..(f-1),
 *		and k in 0..(n-1),
 */
float64
ddiv_d_state(float32 **p, float32 **q, uint32 f, uint32 n)
{
    uint32 j;
    float64 y;

    for (j = 0, y = 0; j < f; j++) {
	y += ddiv_d(p[j], q[j], n);
    }

    return y;
}

/* ADDITION FOR CONTINUOUS_TREE HMM 19 May 98 - 
   Entropy/likelihood function for continuous gaussians PDFs */

/*
 * Description:
 *      This routine computes the entropy of a given gaussian pdf
 *      pdf.
 *
 * Arguments:
 *      float32 *mean
 *              Means - actually not used
 *      float32 *var
 *              Variances - diagonal only
 *      uint32   n
 *              The length of mean/var vectors
 *
 * Returns:
 *      The entropy of the pdf p in bit
 *
 */

#define HAFLN2PI  0.9189385332

float64
ent_cont(float32 *mean, float32 *var, uint32 n)
{
    uint32 k;
    float64 y;

    for (k = 0, y = 0; k < n; k++) {
        assert(var[k] != 0);
        y += log(var[k]);
    }
    y *= 0.5;
    y += (float64)n*(HAFLN2PI + 0.5);

    return(-y * ln2lg2); /* Convert to log base 2 */
}

/* END ADDITION FOR CONTINUOUS_TREE */

/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	div.c,v $
 * Revision 1.1  97/07/16  11:36:22  eht
 * Initial revision
 * 
 *
 */
