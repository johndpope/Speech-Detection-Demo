/*--------------------------------------------------------------*
 * DECLS.H - external/common declarations to make lint happy
 *--------------------------------------------------------------*
 * HISTORY
 * 08-Jan-90  Jeff Rosenfeld (jdr) at Carnegie-Mellon University
 *	Added declarations for some functions from libcs.
 *
 * 03-Apr-89  Jeff Rosenfeld (jdr) at Carnegie-Mellon University
 *	Created.
 */

/* has its own safeguard against multiple inclusions */
#include <sys/types.h>

extern void *malloc(), *calloc(), *realloc();
extern void *valloc();
extern char *salloc();

extern off_t lseek();

extern char *getenv();

/* declaration for functions in libcs.a */
extern FILE *fopenp();
extern char *getname();
extern char *nxtarg();
