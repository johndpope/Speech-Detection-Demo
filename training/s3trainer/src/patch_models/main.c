/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/cp_parm/RCS/main.c,v 1.1 97/07/16 11:36:22 eht Exp Locker: eht $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1997 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: main.c
 * 
 * Description: 
 * 	Copies parameters from an input file to and output file
 *
 * Author: 
 *     Eric H. Thayer (eht@cs.cmu.edu)
 *********************************************************************/

static char rcsid[] = "@(#)$Id: main.c,v 1.1 97/07/16 11:36:22 eht Exp Locker: eht $";

#include "cmd_ln.h"
#include <s3/s3mixw_io.h>
#include <s3/s3gau_io.h>
#include <s3/s3tmat_io.h>
#include <s3/gauden.h>
#include <s3/ckd_alloc.h>
#include <s3/feat.h>
#include <s3/cmd_ln.h>

#include <s3/s3.h>

#include <stdio.h>

static int rd_parm(void);
static int cp_parm(void);
static int wr_parm(void);


static float32 ***imixw;
static uint32 n_mixw_i;
static float32 ***omixw;
static uint32 n_mixw_o;

static uint32 n_stream;
static uint32 n_density;

static vector_t ***igau;
static uint32 n_cb_i;
static vector_t ***ogau;
static uint32 n_cb_o;
static const uint32 *veclen;

static float32 ***itmat;
static uint32 n_tmat_i;
static float32 ***otmat;
static uint32 n_tmat_o;
static uint32 n_state_pm;



static int 
rd_mixw(const char *fn)
{
    if (s3mixw_read(fn,
		    &imixw,
		    &n_mixw_i,
		    &n_stream,
		    &n_density) != S3_SUCCESS)
	return S3_ERROR;

    if (s3mixw_read((const char *)cmd_ln_access("-omixwfn"),
		    &omixw,
		    &n_mixw_o,
		    &n_stream,
		    &n_density) != S3_SUCCESS)
	return S3_ERROR;

    return S3_SUCCESS;
}

static int 
cp_mixw(uint32 o,
	uint32 i)
{
    uint32 j, k;

    printf("mixw %u <= %u\n", o, i);

    for (j = 0; j < n_stream; j++) {
	for (k = 0; k < n_density; k++) {
	    omixw[o][j][k] = imixw[i][j][k];
	}
    }

    return S3_SUCCESS;
}

static int 
wr_mixw(const char *fn)
{
    if (s3mixw_write(fn,
		     omixw,
		     n_mixw_o,
		     n_stream,
		     n_density) != S3_SUCCESS)
	return S3_ERROR;

    ckd_free_3d((void ***)omixw);
    ckd_free_3d((void ***)imixw);

    return S3_SUCCESS;
}


static int
rd_tmat(const char *fn)
{
    if (s3tmat_read(fn,
		    &itmat,
		    &n_tmat_i,
		    &n_state_pm) != S3_SUCCESS)
	return S3_ERROR;

    if (s3tmat_read((const char *)cmd_ln_access("-otmatfn"),
		    &otmat,
		    &n_tmat_o,
		    &n_state_pm) != S3_SUCCESS)
	return S3_ERROR;

    return S3_SUCCESS;
}

static int
cp_tmat(uint32 o,
	uint32 i)
{
    uint32 j, k;

    printf("tmat %u <= %u\n", o, i);

    for (j = 0; j < n_state_pm-1; j++) {
	for (k = 0; k < n_state_pm; k++) {
	    otmat[o][j][k] = itmat[i][j][k];
	}
    }

    return S3_SUCCESS;
}

static int
wr_tmat(const char *fn)
{
    if (s3tmat_write(fn,
		     otmat,
		     n_tmat_o,
		     n_state_pm) != S3_SUCCESS)
	return S3_ERROR;

    ckd_free_3d((void ***)otmat);
    ckd_free_3d((void ***)itmat);

    return S3_SUCCESS;
}


static int
rd_gau(const char *fn)
{
    if (s3gau_read(fn,
		   &igau,
		   &n_cb_i,
		   &n_stream,
		   &n_density,
		   &veclen) != S3_SUCCESS)
	return S3_ERROR;

    if (s3gau_read((const char *)cmd_ln_access("-ogaufn"),
		   &ogau,
		   &n_cb_o,
		   &n_stream,
		   &n_density,
		   &veclen) != S3_SUCCESS)
	return S3_ERROR;

    return S3_SUCCESS;
}

static int
cp_gau(uint32 o,
       uint32 i)
{
    uint32 j, k, l;

    printf("gau %u <= %u\n", o, i);

    for (j = 0; j < n_stream; j++) {
	for (k = 0; k < n_density; k++) {
	    for (l = 0; l < veclen[j]; l++) {
		ogau[o][j][k][l] = igau[i][j][k][l];
	    }
	}
    }
    return S3_SUCCESS;
}

static int
wr_gau(const char *fn)
{
    if (s3gau_write(fn,
		    (const vector_t ***)ogau,
		    n_cb_o,
		    n_stream,
		    n_density,
		    veclen) != S3_SUCCESS)
	return S3_ERROR;

    gauden_free_param(ogau);
    gauden_free_param(igau);

    return S3_SUCCESS;
}

static int
rd_parm()
{
    if (cmd_ln_access("-imixwfn"))
	rd_mixw((const char *)cmd_ln_access("-imixwfn"));

    if (cmd_ln_access("-igaufn")) 
	rd_gau((const char *)cmd_ln_access("-igaufn"));

    if (cmd_ln_access("-itmatfn")) 
	rd_tmat((const char *)cmd_ln_access("-itmatfn"));

    return S3_SUCCESS;
}

static int
cp_parm()
{
    FILE *fp;
    uint32 i, o;

    fp = fopen((char *)cmd_ln_access("-cpopsfn"), "r");
    if (fp == NULL) {
	E_ERROR_SYSTEM("Unable to open cpops file");

	return S3_ERROR;
    }
    while (fscanf(fp, "%u %u", &o, &i) == 2) {
	if (omixw) {
	    cp_mixw(o, i);
	}
	if (ogau) {
	    cp_gau(o, i);
	}
	if (otmat) {
	    cp_tmat(o, i);
	}
    }
    fclose(fp);

    return S3_SUCCESS;
}

static int
wr_parm()
{
    if (omixw) {
	wr_mixw((const char *)cmd_ln_access("-omixwfn"));
    }
    if (ogau) {
	wr_gau((const char *)cmd_ln_access("-ogaufn"));
    }
    if (otmat) {
	wr_tmat((const char *)cmd_ln_access("-otmatfn"));
    }
    return S3_SUCCESS;
}

main(int argc, char *argv[])
{
    parse_cmd_ln(argc, argv);

    rd_parm();
    cp_parm();
    wr_parm();

    return 0;
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
