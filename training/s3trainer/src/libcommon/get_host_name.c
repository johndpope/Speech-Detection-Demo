/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libcommon/RCS/get_host_name.c,v 1.1 97/07/16 11:36:22 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1996 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: get_host_name.c
 * 
 * Description: 
 * 
 * Author: 
 * 	
 *********************************************************************/

static char rcsid[] = "@(#)$Id: get_host_name.c,v 1.1 97/07/16 11:36:22 eht Exp $";

#include <s3/get_host_name.h>

#include <sys_compat/misc.h>
#include <string.h>

const char *
get_host_name()
{
#if !defined(WIN32)
    static char hn[MAXHOSTNAMELEN];
    char *dot;

    if (gethostname(hn, MAXHOSTNAMELEN) < 0)
	return "<hostname_error>";
    else {
	dot = strchr(hn, '.');
	if (dot != NULL) {
	    *dot = '\0';
	}

	return hn;
    }
#else
    return "WIN32(N/A)";
#endif    
}


/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	get_host_name.c,v $
 * Revision 1.1  97/07/16  11:36:22  eht
 * Initial revision
 * 
 *
 */
