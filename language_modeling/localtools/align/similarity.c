#include <string.h>

float similarity(wd1,wd2)
char *wd1, *wd2;
{
float a,b,c,d,e;
float llcs_score(), prop_sim(), sim_left(), sim_right();
char  ref[WC], hyp[WC];

strcpy(ref,wd1);
strcpy(hyp,wd2);

strip(ref);
strip(hyp);

a = llcs_score(ref,hyp);
b = prop_sim(ref,hyp);
c = sim_left(ref,hyp);
d = sim_right(ref,hyp);
e = ave_5(a,a,b,c,d);

if (dbug) printf("%-s\t%-s (%4.2f) <== (%4.2f %4.2f %4.2f %4.2f)\n",
			ref,hyp,e,a,b,c,d);
return(e);
}

/****************************************************************************/

float llcs_score(wd1,wd2)
char *wd1, *wd2;
{
int c = 0;
int found = TRUE;
int i,j, len1, len2;
float final;

len1 = strlen(wd1);
len2 = strlen(wd2);
while (found) {
	found = FALSE;
	for (i=0; ((i <= (len1 - (c+1))) && (!found)); i++)
		for (j=0; ((j <= len2 - (c+1)) && (!found)); j++)
			if (strncmp(wd1+i,wd2+j,c+1)==0) {
				found = TRUE;
				++c;
			}
}
final = ((float) c) / ((float) min(len1,len2));
final -= (wt * fabs(len1-len2));
return(final);
}

/****************************************************************************/

float prop_sim(ref,tst)
char *ref, *tst;
{
float ratio,final,p,q;
int   found,i,j,high,low,score,tst_len,ref_len,p_round,q_round,last_found;

ref_len = strlen(ref);
tst_len = strlen(tst);

ratio = ((float) ref_len) / ((float) tst_len);
score = 0;
last_found = -1;
for (i=0; i<tst_len; i++) {
	if (debug_similarity) printf("i = %3d:  ", i);
	found = FALSE;
	p = ratio * (((float) i) + 0.5);
	q = ratio *  ((float) i);
	if (debug_similarity) printf("<%3.1f,%3.1f>  ",q,p);
	p_round = sround(p);
	q_round = sround(q);
	if (debug_similarity) printf("<%2d,%2d>  ", q_round, p_round);
	low = max(0,q_round);
	low = max(low,last_found+1);
	high = min(ref_len-1,p_round);
	if (debug_similarity) printf("<%2d,%2d>    ", low, high);
	for (j=low; ((j<=high) && (! found)); j++)
		if ((*(tst+i))==(*(ref+j))) {
			if (debug_similarity) printf("%c == %c  ", (*(tst+i)), (*(ref+j)));
			found = TRUE;
			++score;
			last_found = j;
		}
		else {
			if (debug_similarity) printf("%c <> %c  ", (*(tst+i)), (*(ref+j)));
		}
	if (debug_similarity) printf("\n");
}

if (debug_similarity) printf("\nscore = %d\n", score);
final = ((float) score) / (((float) (tst_len+ref_len)) / 2.0);
final -= (wt * fabs(ref_len - tst_len));
return(final);
}

/***************************************************************************/

float sim_left(ref,tst)
char *ref, *tst;
{
float final;
int   found,i,j,high,low,score,tst_len,ref_len,last_found;

ref_len = strlen(ref);
tst_len = strlen(tst);

score = 0;
last_found = -1;
for (i=0; i<tst_len; i++) {
	if (debug_similarity) printf("i = %3d:  ", i);
	found = FALSE;
	low = max(max(0,i-1),last_found+1);
	high = min(ref_len-1,i+1);
	if (debug_similarity) printf("<%2d,%2d>    ", low, high);
	for (j=low; ((j<=high) && (! found)); j++)
		if ((*(tst+i))==(*(ref+j))) {
			if (debug_similarity) printf("%c == %c  ", (*(tst+i)), (*(ref+j)));
			found = TRUE;
			++score;
			last_found = j;
		}
		else {
			if (debug_similarity) printf("%c <> %c  ", (*(tst+i)), (*(ref+j)));
		}
	if (debug_similarity) printf("\n");
}

if (debug_similarity) printf("\nl_score = %d\n", score);
final = ((float) score) / (((float) (tst_len+ref_len)) / 2.0);
final -= (wt * fabs(ref_len - tst_len));
return(final);
}

/**************************************************************************/

float sim_right(ref,tst)
char *ref, *tst;
{
char  ref_copy[WC], tst_copy[WC];
float sim_left();

strcpy(ref_copy,ref);
strcpy(tst_copy,tst);

reverse(ref_copy);
reverse(tst_copy);

return(sim_left(ref_copy,tst_copy)); 
}

/****************************************************************************/
/* Strips out characters '+' and '-' from input string s for the comparison */

strip(s)
char *s;
{
char temp[WC];
char *a = temp;
char *b = s;
char old[WC];

strcpy(old,s);
strcpy(temp,s);

do {
	if ((*a != '+') && (*a != '-')) *b++ = *a;
} while (*a++);

if (debug_similarity) printf("\"%s\" STRIPPED TO \"%s\"\n",old,s);
}

/***************************************************************************/

reverse(s)
char s[];
{
int c, i, j;

for (i=0,j=strlen(s)-1; i<j; i++,j--) {
	c = s[i];
	s[i] = s[j];
	s[j] = c;
}
}

