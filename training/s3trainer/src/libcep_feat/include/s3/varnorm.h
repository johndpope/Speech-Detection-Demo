#ifndef _VNORM_H_
#define _VNORM_H_

#include <s3/prim_type.h>

void norm_var (float32 *vec,            /* the data */
                uint32 nvec,    /* number of vectors (frames) */
                uint32 veclen);  /* number of components per vector */

#endif

