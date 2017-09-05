/*********************************************************************
 *
 * $Header: /usr2/eht/s3/src/libcommon/RCS/n_words.c,v 1.3 1995/10/12 17:41:30 eht Exp $
 *
 * Carnegie Mellon ARPA Speech Group
 *
 * Copyright (c) 1995 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: n_words.c
 * 
 * Description: 
 *	This routine counts the number of space separated strings
 *	(i.e. words) in a given string.
 * 
 * Author: 
 * 	Eric H. Thayer (eht@cs.cmu.edu)
 *********************************************************************/

static char rcsid[] = "@(#)$Id: n_words.c,v 1.3 1995/10/12 17:41:30 eht Exp $";

#include <s3/n_words.h>

#include <ctype.h>


uint32
n_words(char *str)
{
    int n;

    n = 0;
    for (;;) {
	while (*str && isspace(*str)) str++;
	if (*str) {
	    while (!isspace(*str) && *str) str++;
		++n;
	}
	else break;
    }

    return n;
}

/*
 * Log record.  Maintained by RCS.
 *
 * $Log: n_words.c,v $
 * Revision 1.3  1995/10/12  17:41:30  eht
 * Updated comments
 *
 * Revision 1.2  1995/10/09  20:56:36  eht
 * Changes needed for prim_type.h
 *
 * Revision 1.1  1995/06/02  14:52:54  eht
 * Initial revision
 *
 *
 */
