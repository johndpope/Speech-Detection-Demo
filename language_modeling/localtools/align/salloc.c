/* SALLOC - allocate space for the string argument and retuen a copy
 *-------------------------------------------------------------------*
 * HISTORY
 * Jun 23, 1989 Fil Alleva, faa@cs.cmu.edu
 *	Fixed typo/bug that caused malloc to allocate a buffer that
 *	with 2 less bytes than required.
 */
char *
salloc (str)
char *str;
{
    char *ret;
    ret = (char *) malloc (strlen(str)+1);
    
    strcpy (ret, str);
    return ret;
}
