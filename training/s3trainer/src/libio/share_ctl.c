/*********************************************************************
 *
 * $Header: /net/alf19/usr2/eht/s3/src/libio/RCS/share_ctl.c,v 1.1 97/03/17 15:01:49 eht Exp $
 *
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1997 Carnegie Mellon University.
 * All rights reserved.
 *
 *********************************************************************
 *
 * File: share_ctl.c
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

static char rcsid[] = "@(#)$Id: share_ctl.c,v 1.1 97/03/17 15:01:49 eht Exp $";
#include <s3/prim_type.h>
#include <s3/swap.h>
#include <s3/bo_magic.h>
#include <s3/s3.h>

#include <sys_compat/file.h>

static uint32 blk_sz;
static uint32 buf_id;
static int state_fd;

int
share_ctl_set_block_size(uint32 sz)
{
    blk_sz = sz;
}

int
share_ctl_set_buf_id(uint32 id)
{
    buf_id = id;

    return S3_SUCCESS;
}

int
share_ctl_set_state_file(const char *file_name)
{
    state_fd = open(file_name, O_CREAT | O_RDWR, 0755);
    if (state_fd < 0) {
	E_FATAL_SYSTEM("Couldn't open ctl file state file %s\n",
		       file_name);
    }

    return S3_SUCCESS;
}

int
share_ctl_init_state_file()
{
    uint32 magic = BYTE_ORDER_MAGIC;
    uint32 zero = 0;

    if (write(state_fd, &magic, sizeof(uint32)) < 0)
	E_FATAL_SYSTEM("write failed");
    
    if (write(state_fd, &zero, sizeof(uint32)) < 0)
	E_FATAL_SYSTEM("write failed");

    lseek(state_fd, (off_t)0, SEEK_SET);
}

int
share_ctl_next_utts(uint32 *offset, uint32 *run)
{
    uint32 magic;
    uint32 i_offset;
    uint32 o_offset;
    uint32 ns;
    int ret;
    struct flock lck;

    lck.l_type = F_WRLCK;
    lck.l_whence = SEEK_SET;
    lck.l_start = 0;
    lck.l_len = 2 * sizeof(uint32);

    /* Get an exclusive lock on the file */
    if (fcntl(state_fd, F_SETLKW, &lck) < 0) {
	E_FATAL_SYSTEM("file lock failed");
    }

    printf("*** in crit sec ***\n");

    /* read the byte order magic number */
    ret = read(state_fd, &magic, sizeof(uint32));
    if (ret < 0)
	E_FATAL_SYSTEM("read failed");
    else if (ret < sizeof(uint32)) {
	E_FATAL("Expected to read %u bytes, but got %u instead\n",
		sizeof(uint32), ret);
    }
	       

    if (magic != BYTE_ORDER_MAGIC) {
	/* seems to need swap */

	SWAP_INT32(&magic);

	if (magic != BYTE_ORDER_MAGIC) {
	    E_FATAL("Couldn't create magic # by swapping\n");
	}

	ns = TRUE;
    }
    else
	ns = FALSE;


    ret = read(state_fd, &i_offset, sizeof(uint32));
    if (ret < 0)
	E_FATAL_SYSTEM("read failed");
    else if (ret < sizeof(uint32)) {
	E_FATAL("Expected to read %u bytes, but got %u instead\n",
		sizeof(uint32), ret);
    }
    
    if (ns)
	SWAP_INT32(&i_offset);

    o_offset = i_offset + blk_sz;

    printf("i_offset= %u\n", i_offset);

    sleep(30);

    lseek(state_fd, (off_t)0, SEEK_SET);

    if (write(state_fd, &magic, sizeof(uint32)) < 0)
	E_FATAL_SYSTEM("write failed");
    
    if (write(state_fd, &o_offset, sizeof(uint32)) < 0)
	E_FATAL_SYSTEM("write failed");
    
    lck.l_type = F_UNLCK;

    if (fcntl(state_fd, F_SETLK, &lck) < 0) {
	E_FATAL_SYSTEM("file unlock failed");
    }

    lseek(state_fd, (off_t)0, SEEK_SET);

    printf("*** out crit sec ***\n");

    *offset = i_offset;
    *run = blk_sz;

    return S3_SUCCESS;
}

/*
 * Log record.  Maintained by RCS.
 *
 * $Log:	share_ctl.c,v $
 * Revision 1.1  97/03/17  15:01:49  eht
 * Initial revision
 * 
 *
 */
