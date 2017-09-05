/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libcommon/RCS/enum_subset.c,v 1.1 97/07/16 11:36:22 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1997 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: enum_subset.c
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

static char rcsid[] = "@(#)$Id: enum_subset.c,v 1.1 97/07/16 11:36:22 eht Exp $";

#include <s3/enum_subset.h>
#include <s3/s3.h>

int
next_subset(uint32 *s,
	    uint32 n,
	    uint32 r)
{
    int i; 
 
    i = r-1; 
 
    for (i = r-1; (i >= 0) && (s[i] == n-r+i); i--); 
 
    if (i < 0) 
        return FALSE; 
 
    s[i]++; 
 
    for (++i; i < r; i++) 
        s[i] = s[i-1] + 1; 
 
    return TRUE;
}


/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	enum_subset.c,v $
 * Revision 1.1  97/07/16  11:36:22  eht
 * Initial revision
 * 
 *
 */
