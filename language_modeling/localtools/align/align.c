/*
	String Alignment and Scoring Program
	Written by Stan Janet, July 1987

	Compile using: 'cc -o align align.c' or 'cc -O -o align align.c'

	To be added: synonyn table, threshold for min
	pct. of similarity triggering a switch, command-line
	switch defeat, 'semantically OK' flag, ...
HISTORY
	28-Sep-94 (rkm@cs.cmu.edu)
		Added "&& (c != '\n')" condition to is_SNOR test in
		read_lexicon().

        31-Oct-89 (faa@cs.cmu.edu)
	        Changed is_SNOR() macro to return true on everything but
		blank space.

	11-Jun-89 (faa@cs.cmu.edu)
		Pulled c[SL][SL] and bp[SL][SL] off of the stack
		in align so that I could compile with a larger
		value for SL.
	
*/

#include <stdio.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "pconf.h"

/****************** CONSTANTS ******************************************/

#define LEXICON_FILE	"/usr/hmm/align/LEXICON"
#define OK		0
#define SUB		1
#define INS		2
#define DEL		3

#define MAX_penalty	1000000
#define SUB_penalty	100
#define INS_penalty	75
#define DEL_penalty	75

#define SPL		1
#define MRG		2

#define TRUE 		1
#define FALSE 		0

#define SL 		600		/* words in a sentence 		    */
#define WC		400		/* characters in a word 	    */
#define SC		SL * (WC + 1)	/* characters in a sentence 	    */
#define STL		100000		/* split table length 		    */
#define sent_STL	4000		/* sentence split table length      */
#define SLL		3 * (WC + 1)	/* split file line length 	    */
#define TL		100000		/* general table length		    */
#define SMSL		2		/* split/merge sequence length	    */
#define MAXWORD		200		/* characters in a lexicon word     */
#define LEX_SIZE	100000		/* words in lexicon 		    */
#define BUFFERSIZE	LEX_SIZE * (MAXWORD + 1)

#define show_lexicon		FALSE		/* debugging flags */
#define pr			FALSE
#define dbug2			FALSE
#define dbug_lexicon		FALSE
#define attempt_realignment	TRUE

#define ZERO			0
#define ONE			1

#define E_bad_ref_file		1		/* Error codes */
#define E_bad_hyp_file		2
#define E_unsorted_lexicon	3
#define E_bad_SM_file		4
#define E_bad_summary_file	5
#define E_too_many_chars	6
#define E_both_sents_long	7
#define E_ref_sent_long		8
#define E_hyp_sent_long		9
#define E_bad_lexicon_file	11
#define E_illegal_char		12
#define E_bad_invoc		13
#define E_bad_hom_file		14
#define E_time_pairs		15

#define wt			0.04
#define debug_similarity	FALSE

/************************ MACROS ***************************************/

#define sround(x)		((int) (((x)<0) ? ((x)-0.5) : ((x)+0.5)))
#define swap_ints(x,y)		{ int _t_ = (y); y = (x); x = _t_; }
#define square(x)		((x) * (x))
#define min(x,y)		(((x)<(y)) ? (x) : (y))
#define max(x,y)		(((x)>(y)) ? (x) : (y))
#define pct(x,y)		((y==0) ? (0.0) : (100.0 * (((float) x)/((float) y))))
#if 0
#define is_SNOR(c)		((isupper(c)||isdigit(c)|| \
				 (c=='+')||(c=='-')) ? TRUE : FALSE)
#endif
/*
 * This is_SNOR accepts anything BUT blank space
 */
#define is_SNOR(c)		((c != ' ')&&(c != '\t')&&(c != '\0'))
#define ave_5(a,b,c,d,e)	(((a)+(b)+(c)+(d)+(e)) / 5.0)
#define fabs(x)			(((x)>=0.0) ? (x) : (-(x)))
#define swap_strings(s1,s2)	{ char _temp_wd[WC]; \
				strcpy(_temp_wd,s1); \
				strcpy(s1,s2); \
				strcpy(s2,_temp_wd); }
#define swap_ptrs(p1,p2)	{ char *_temp; \
				_temp = *p1; \
				*p1  = *p2; \
				*p2  = _temp; }
/*
#define string_to_upper_case(s) { char *_ch; \
				for (_ch=(s); *_ch; _ch++) \
				  if (islower(*_ch)) *_ch = toupper(*_ch); }
#define string_to_lower_case(s) { char *_ch; \
				for (_ch=(s); *_ch; _ch++) \
				  if (isupper(*_ch)) *_ch = tolower(*_ch); }
*/
#define string_to_upper_case(s) { char *_ch; \
				for (_ch=(s); *_ch; _ch++) \
				  if ('a'<=*_ch&&*_ch<='z') *_ch -= 32; }
#define string_to_lower_case(s) { char *_ch; \
				for (_ch=(s); *_ch; _ch++) \
				  if ('A'<=*_ch&&*_ch<='Z') *_ch += 32; }

#define fill_string(buf,ct,str) { int _m; \
				strcpy(buf,"\0"); \
				for (_m=0; _m<ct; _m++) \
				  strcat(buf,str); }

/************************** EXTERNAL VARIABLES **************************/

				/* COUNTERS OVER CURRENT SENTENCE */
int sent_corr;
int sent_sub;
int sent_ins;
int sent_del;
int sent_err;
int sent_aligned_wds;
int sent_ref_wds;
int sent_hyp_wds;
int ref_unknown;
int hyp_unknown;
int merges;
int merge_candidates;
int splits;
int split_candidates;
int switches;

					/* COUNTERS OVER SET OF SENTENCES */
int total_corr			= 0;
int total_sub			= 0;
int total_ins			= 0;
int total_del			= 0;
int total_err			= 0;
int total_aligned_wds		= 0;
int total_splits		= 0;
int total_split_candidates	= 0;
int total_merges		= 0;
int total_merge_candidates	= 0;
int total_ref_wds		= 0;
int total_hyp_wds		= 0;
int total_ref_unknown		= 0;
int total_hyp_unknown		= 0;
int total_switches		= 0;
int total_sentences		= 0;

					/* COUNTERS OVER SET OF SENTENCES */
int serr			= 0;	/* Sentences with 1+ error */
int scorr			= 0;	/* Sentences correct */
int ssub			= 0;	/* Sentences with 1+ substitution */
int sins			= 0;	/* Sentences with 1+ insertion */
int sdel			= 0;	/* Sentences with 1+ deletion */

					/* SPLIT/MERGE TABLE -- predeclared
					   and read in from a file */
int  split_merge_TL;				/* length of table */
char split_merge_word[STL][SL];			/* word that may be split/merged */
char split_merge_sequence[STL][SMSL][SL];	/* corresponding sequence of words */
int  split_occs[STL];				/* counts occurences of splits */
int  merge_occs[STL];				/* counts occurences of merges */

					/* SPLIT/MERGE CANDIDATE TABLE --
					   created as candidates are found;
					   the follwoing variables correspond
					   to SPLIT/MERGE table variables */
int  SMC_TL;
char SMC_word[STL][SL];
char SMC_sequence[STL][SMSL][SL];
int  SC_occs[STL];
int  MC_occs[STL];
					/* SENTENCE'S SPLIT/MERGE TABLE --
					   Splits, split candidates, merges,
					   and merge candidates are printed
					   for each sentence */
int  sent_SMC_TL;
char sent_SMC_word[sent_STL][WC];
char sent_SMC_sequence[sent_STL][SMSL][WC];
int  sent_SMC_type[sent_STL];
int  sent_SMC_found[sent_STL];

					/* LEXICON LIST */
char *word[LEX_SIZE];			/* pointers to the words */
char lexicon_buffer[BUFFERSIZE];	/* buffer holding the words */
int  lexicon_present;			/* TRUE if file "LEXICON" exists */
int  lexicon_size;			/* size of lexicon */
int  lexicon_sorted;			/* TRUE if lexicon file words are
					   sorted-- they are expected to be */

					/* HOMOPHONE TABLE */
int  homophone_TL;			/* table length */
char hom1[TL][WC];			/* first word of pair */
char hom2[TL][WC];			/* second word of pair */

					/* CONFUSION PAIR TABLE */
char confusion_ref[TL][WC];		/* misrecognized words (reference) */
char confusion_hyp[TL][WC];		/* substituted words (hypothesis) */
int  confused_pairs = 0;		/* table length */
int  confusion_count[TL];		/* count of each pairs occurences */
int  in_a_split[TL];
int  in_splits[TL];
int  in_a_merge[TL];
int  in_merges[TL];

					/* TABLES to store inserted, deleted,
					   misrecognized, and substituted
					   words */
char insertion[TL][WC], deletion[TL][WC], misrec[TL][WC], subst[TL][WC];
int  inserted_words=0, deleted_words=0, misrec_words=0, subst_words=0;
int  insertion_count[TL], deletion_count[TL], misrec_count[TL], subst_count[TL];

char ref_sent_code_string[WC];
char hyp_sent_code_string[WC];

char *gap = "*";
						/* FLAGS -- all FALSE by
						   default, set true by
						   characters 'D','R','U',
						   and 'H' on command line
						   following reference and
						   hypothesis file names */
int dbug = FALSE;
int read_times = FALSE;
int use_times = FALSE;
int HC = FALSE;

/************************************************************************/
/************************************************************************/

main(int argc,char **argv)
{
int  n1;			/* words in reference sentence */
int  n2;			/* words in hypothesis sentence */
char *a1[SL];			/* vector of unaligned reference words */
char *a2[SL];			/* vector of unaligned hypothesis words */
char ref_unaligned[SC];		/* unaligned reference string */
char hyp_unaligned[SC];		/* unaligned hypothesis string */

char *ref_wds[2*SL];		/* vector of aligned reference words */
char *hyp_wds[2*SL];		/* vector of aligned hypothesis words */
char ref_aligned[SC];		/* aligned reference string */
char hyp_aligned[SC];		/* aligned hypothesis string */

int  comparisons[2*SL];		/* vectors of results of alignment */
int  part_of_split[2*SL];
int  part_of_merge[2*SL];

char ref_time_line[SC];		/* unaligned reference begin/end times */
char hyp_time_line[SC];		/* unaligned hypothesis begin/end times */
int  ref_begin[SL];		/* vector of reference begin times */
int  ref_end[SL];		/* vector of reference end times */
int  hyp_begin[SL];		/* vector of hypothesis begin times */
int  hyp_end[SL];		/* vector of hypothesis end times */

FILE *ref_sents;
FILE *hyp_sents;
FILE *fopen();
static char *HypFile = "match.log";
static char *RefFile = "correct.log";
static char *LexFile = "LEXICON";
static char *HomFile = "HOMOPHONES";
static char *SMFile  = "SPLITS_AND_MERGES";

static config_t params[] = {
	"HypFile", "Identifies the hypothesis file produced by SPHINX",
	"-hyp", STRING, (caddr_t)&HypFile,

	"RefFile", "Identifies the reference file produced by SPHINX",
	"-def", STRING, (caddr_t)&RefFile,

	"LexFile", "Identifies the list of words used in the run",
	"-lex", STRING, (caddr_t)&LexFile,

	"HomoFile", "Identifies the file which defines the homophones",
	"-hom", STRING, (caddr_t)&HomFile,

	"SMFile", "Identifies the file which defines the splits and merges",
	"-sm", STRING, (caddr_t)&SMFile,

	"Debug", "Turns on debugging",
	"-debug", BOOL, (caddr_t)&dbug,

	"ReadTimes", "Turns on read times",
	"-rt", BOOL, (caddr_t)&read_times,

	"UseTimes", "Turns on use times",
	"-ut", BOOL, (caddr_t)&use_times,

	"HomoEq", "Turns on homophones are equivalent",
	"-heq", BOOL, (caddr_t)&HC,

	0, 0, 0, NOTYPE, 0
};

pconf(argc, argv, params, 0, 0, 0);

if (dbug) printf("Debug ON\n");
if (dbug && read_times) printf("Reading times\n");
if (dbug && use_times) printf("Using times\n");
if (dbug && HC) printf("Homophones are equivalent\n");

lexicon_size	= read_lexicon(LexFile);
homophone_TL	= read_homophones(HomFile);
split_merge_TL	= set_split_merge_table(SMFile);

if ((ref_sents = fopen(RefFile,"r"))==NULL)
	print_err(E_bad_ref_file,0,RefFile,"");
if ((hyp_sents = fopen(HypFile,"r"))==NULL)
	print_err(E_bad_hyp_file,0,HypFile,"");

init_tables();
while (TRUE) {

	strcpy(ref_unaligned,"\0");
	strcpy(hyp_unaligned,"\0");
	strcpy(ref_aligned,"\0");	/* prepare aligned */
	strcpy(hyp_aligned,"\0");	/* 	result strings */
	sent_SMC_TL = 0;		/* erase last sent's splits, merges */

	if ((fgets(ref_unaligned, SC, ref_sents)  == NULL) ||
	    (fgets(hyp_unaligned, SC, hyp_sents) == NULL)) break;
	parse(ref_unaligned,&n1,a1,ref_sent_code_string);
	if (lexicon_present) check_lexicon(n1,a1,&ref_unknown);
	parse(hyp_unaligned,&n2,a2,hyp_sent_code_string);
	if (lexicon_present) check_lexicon(n2,a2,&hyp_unknown);

	if (read_times || use_times) {
		fgets(ref_time_line,SC,ref_sents);
		parse_times(ref_time_line,ref_begin,ref_end,n1);
		fgets(hyp_time_line,SC,hyp_sents);
		parse_times(hyp_time_line,hyp_begin,hyp_end,n2);
	}

	sent_err = align(a1,n1,a2,n2,ref_aligned,hyp_aligned,
				ref_wds,hyp_wds,comparisons,
				part_of_split,part_of_merge);
	update_total_counts();
	store_word_confusions(ref_wds,hyp_wds,comparisons,
				part_of_split,part_of_merge);
	print_summary(ref_aligned,hyp_aligned);
}

print_file_summary(RefFile,HypFile);
if (dbug) {
	printf("\nEND OF DATA\n");
	printf("Total of %d sentences processed\n\n", total_sentences);
}
}

/*****************************************************************/
/* Sets optional flags by looking at command line arguments */

set_flags(int argc, char **argv)
{
int a;

for (a=3; a<argc; a++)
	if (strcmp(argv[a],"D")==0) dbug = TRUE;
	else if (strcmp(argv[a],"R")==0) read_times = TRUE;
	else if (strcmp(argv[a],"U")==0) use_times = TRUE;
	else if (strcmp(argv[a],"H")==0) HC = TRUE;

}

/*****************************************************************/
/* Prints various error messages to stderr. Some error code numbers
   cause the program to halt. */

print_err(int code,int val,char *s1,char *s2)
{
    fflush(stdout);

switch (code) {
	case E_bad_ref_file:
		fprintf(stderr,"Error: could not open reference sentence file \"%s\".\n", s1);
		fprintf(stderr,"Halting.\n");
		exit(-1);
		break;

	case E_bad_hyp_file:
		fprintf(stderr,"Error: could not open hypothesis sentence file \"%s\".\n", s1);
		fprintf(stderr,"Halting.\n");
		exit(-1);
		break;

	case E_unsorted_lexicon:
		fprintf(stderr,"Warning: lexicon is not sorted as expected.\n");
		fprintf(stderr,"         (\"%s\" was encountered before \"%s\")\n", s1, s2);
		break;

	case E_bad_SM_file:
		fprintf(stderr,"Warning: could not open split/merge file %s\n",s1);
		break;

	case E_bad_summary_file:
		fprintf(stderr,"Cannot open file %s for file totals.\n",s1);
		break;

	case E_too_many_chars:
		fprintf(stderr,"A sentence has been read in incompletely.\n");
		fprintf(stderr,"Increase parameter SC which is currently %d.\n",SC);
		break;

	case E_both_sents_long:
		fprintf(stderr,"Both sentences contain too many words.\n");
		fprintf(stderr,"Increase parameter SL by at least %d.\n",val);
		exit(-1);
		break;

	case E_ref_sent_long:
		fprintf(stderr,"Sentence one contains too many words.\n");
		fprintf(stderr,"Increase parameter SL by at least %d.\n",val);
		exit(-1);
		break;

	case E_hyp_sent_long:
		fprintf(stderr,"Sentence two contains too many words.\n");
		fprintf(stderr,"Increase parameter SL by at least %d.\n",val);
		exit(-1);
		break;

	case E_bad_lexicon_file:
		fprintf(stderr,"Cannot open lexicon file \"%s\".\n",s1);
		break;

	case E_illegal_char:
		fprintf(stderr,"Illegal character %c (%d) encountered reading lexicon file \"%s\".\n", val, val, s1);
		fprintf(stderr,"Lexicon file should consist only of upper case letters, digits,\n");
		fprintf(stderr,"plus and minus symbols, blanks, tabs, and newlines.\n");
		fprintf(stderr,"Terminating execution.\n");
		exit(-1);
		break;

	case E_bad_invoc:
		fprintf(stderr,"Usage: \"adv r h\" where:\n");
		fprintf(stderr, "\t\tr is the reference sentence file,\n");
		fprintf(stderr, "\t\th is the hypothesis sentence file\n");
		fprintf(stderr, "Invocation with argc = %d is illegal\n",val);
		fprintf(stderr, "Halting\n");
		exit(-1);
		break;

	case E_bad_hom_file:
		fprintf(stderr,"Warning: could not open homophones file %s\n",s1);
		break;

	case E_time_pairs:
		fprintf(stderr,"Error: %d pairs of beginning/end times expected\n",val);
		fprintf(stderr,"Offending line:\n");
		fprintf(stderr,"\t%s\n",s1);
		exit(-1);
		break;

	default: fprintf(stderr,"PROC print_err called with invalid code %d\n", code);
		break;
}
fflush(stderr);
}

/*****************************************************************/
/* Reads in from external file a set of acceptable splits and merges.
   It is assumed that any word that can be split into two words can
   be merged from those same two words. */

int set_split_merge_table(char *filename)
{
FILE *split_merge_file;
FILE *fopen();
int  done;
char line[3*(WC+1)];
int  length = 0;

if ((split_merge_file = fopen(filename,"r"))==NULL)
	print_err(E_bad_SM_file,0,"SPLITS_AND_MERGES","");
else {
	done = FALSE;
	while (! done) {
		if (fgets(line,SLL,split_merge_file)==NULL)
			done = TRUE;
		else if (sscanf(line,"%s%s%s",split_merge_word[length],
				split_merge_sequence[length][0],
				split_merge_sequence[length][1])==3)
			++length;
		else
			done = TRUE;
	}
	fclose(split_merge_file);
	if (dbug) printf("split_merge_TL = %d\n", length);
}
return(length);
}

/*****************************************************************/
/* Counts pertaining to the entire file are incremented by counts
   pertaining to each individual reference/hypothesis sentence pair. */

update_total_counts()
{
++total_sentences;
total_ref_wds 		+= sent_ref_wds;
total_hyp_wds 		+= sent_hyp_wds;
total_ref_unknown 	+= ref_unknown;
total_hyp_unknown 	+= hyp_unknown;
total_corr 		+= sent_corr;
total_sub 		+= sent_sub;
total_ins 		+= sent_ins;
total_del 		+= sent_del;
total_err 		+= sent_err;
total_aligned_wds 	+= sent_aligned_wds;
total_splits 		+= splits;
total_split_candidates	+= split_candidates;
total_merges 		+= merges;
total_merge_candidates	+= merge_candidates;
total_switches 		+= switches;

if (sent_err) serr++; else scorr++;
if (sent_sub) ssub++;
if (sent_ins) sins++;
if (sent_del) sdel++;
}
/*****************************************************************/
/* Prints the lexicon words */

print_lexicon()
{
int b;
printf("\nLEXICON SIZE  = %d\n", lexicon_size);
for (b=0; b<lexicon_size; b++)
	printf("Word %4d = \"%s\"\n",b,word[b]);
printf("\n");
}

/*****************************************************************/
/* Called for each reference/hypothesis sentence pair. Prints the
   aligned strings, important counts and percentages associated
   with them, and split and merged strings found in the pair. */

print_summary(char *ref_aligned,char *hyp_aligned)
{
int i,j;

if (dbug) printf("\n  -  -  -  -  -  -  -  -  -  -  -  -  -\n");
printf("\n");
printf("REF: %s %s\n", ref_aligned,
       (*ref_sent_code_string) ? ref_sent_code_string : hyp_sent_code_string);
printf("HYP: %s %s\n", hyp_aligned,
       (*ref_sent_code_string) ? ref_sent_code_string : hyp_sent_code_string);
printf("\n");
printf("SENTENCE %d  %s\n", total_sentences,
       (*ref_sent_code_string) ? ref_sent_code_string : hyp_sent_code_string);
printf("Correct          = %5.1f%% %4d   (%6d)\n", pct(sent_corr,sent_ref_wds), sent_corr, total_corr);
/* KFL - too much output 
printf("Substitutions    = %5.1f%% %4d   (%6d)\n", pct(sent_sub,sent_ref_wds), sent_sub, total_sub);
printf("Deletions        = %5.1f%% %4d   (%6d)\n", pct(sent_del,sent_ref_wds), sent_del, total_del);
printf("Insertions       = %5.1f%% %4d   (%6d)\n", pct(sent_ins,sent_ref_wds), sent_ins, total_ins);
*/
/* printf("Errors 2         = %5.1f%% %4d\n", pct(sent_ref_wds-sent_corr,sent_ref_wds), sent_ref_wds-sent_corr); */
printf("Errors           = %5.1f%% %4d   (%6d)\n", pct(sent_err,sent_ref_wds), sent_err, total_err);
/* KFL  - too much output
printf("\n");
printf("Ref. words       =        %4d   (%6d)\n", sent_ref_wds, total_ref_wds);
printf("Hyp. words       =        %4d   (%6d)\n", sent_hyp_wds, total_hyp_wds);
printf("Aligned words    =        %4d   (%6d)\n", sent_aligned_wds, total_aligned_wds);
if (lexicon_present) {
printf("Ref. unknown     =        %4d   (%6d)\n", ref_unknown, total_ref_unknown);
printf("Hyp. unknown     =        %4d   (%6d)\n", hyp_unknown, total_hyp_unknown);
}
printf("Splits           =        %4d   (%6d)\n", splits, total_splits);
printf("Split candidates =        %4d   (%6d)\n", split_candidates, total_split_candidates);
printf("Merges           =        %4d   (%6d)\n", merges, total_merges);
printf("Merge candidates =        %4d   (%6d)\n", merge_candidates, total_merge_candidates);
printf("\n");
*/
for (i=0; i<sent_SMC_TL; i++)
	switch(sent_SMC_type[i]) {
	      case SPL: printf("   S%c   ",sent_SMC_found[i] ? ' ' : 'C');
			printf("%s ==> ",sent_SMC_word[i]);
			for (j=0; j<SMSL; j++)
				printf("%s ",sent_SMC_sequence[i][j]);
			printf("\n");
			break;
	      case MRG: printf("   M%c   ",sent_SMC_found[i] ? ' ' : 'C');
			for (j=0; j<SMSL; j++)
				printf("%s ",sent_SMC_sequence[i][j]);
			printf("==> %s\n",sent_SMC_word[i]);
			break;
	}
if (dbug) printf("Switches         =        %4d   (%6d)\n", switches, total_switches);
printf("\n============================================================================\n");
}

/*****************************************************************/
/* Prints summary statistics from the processing of the entire/*
   data files to an EXTERNAL file.
   KFL - modified to write to stdout. */

print_file_summary(char *ref,char *hyp)
{
printf("\n\n---------- SUMMARY ----------\n");
printf("Reference file:  %s\n",ref);
printf("Hypothesis file: %s\n",hyp);
if (HC)
	printf("Substitutions of homophones were not counted as errors.\n");
if (attempt_realignment)
	printf("Some extra alignment was attempted.\n");
printf("\n\n");
printf("SENTENCE RECOGNITION PERFORMANCE:\n");
printf("sentences                          %4d\n", total_sentences);
printf("  correct                  %5.1f%% (%4d)\n", pct(scorr,total_sentences), scorr);
printf("  with error(s)            %5.1f%% (%4d)\n", pct(serr,total_sentences), serr);
printf("    with substitution(s)   %5.1f%% (%4d)\n", pct(ssub,total_sentences), ssub);
printf("    with insertion(s)      %5.1f%% (%4d)\n", pct(sins,total_sentences), sins);
printf("    with deletion(s)       %5.1f%% (%4d)\n", pct(sdel,total_sentences), sdel);
printf("\n\n\nWORD RECOGNITION PERFORMANCE:\n");
printf("Correct          = %5.1f%% (%6d)\n", pct(total_corr,total_ref_wds), total_corr);
printf("Substitutions    = %5.1f%% (%6d)\n", pct(total_sub,total_ref_wds), total_sub);
printf("Deletions        = %5.1f%% (%6d)\n", pct(total_del,total_ref_wds), total_del);
printf("Insertions       = %5.1f%% (%6d)\n", pct(total_ins,total_ref_wds), total_ins);
printf("Errors           = %5.1f%% (%6d)\n", pct(total_err,total_ref_wds), total_err);
printf("\n");
printf("Ref. words       = %6d\n", total_ref_wds);
printf("Hyp. words       = %6d\n", total_hyp_wds);
printf("Aligned words    = %6d\n", total_aligned_wds);
if (lexicon_present) {
	printf("Ref. unknown     = %6d\n", total_ref_unknown);
	printf("Hyp. unknown     = %6d\n", total_hyp_unknown);
}
printf("Splits           = %6d\n", total_splits);
printf("Split candidates = %6d\n", total_split_candidates);
printf("Merges           = %6d\n", total_merges);
printf("Merge candidates = %6d\n", total_merge_candidates);
if (dbug) printf("Switches         = %6d\n", total_switches);
printf("\n");
printf("WORD ACCURACY=  %6.3f%% (%5d/%5d)  ERRORS= %6.3f%% (%5d/%5d)\n",
       pct(total_corr,total_ref_wds), total_corr, total_ref_wds,
       pct(total_err,total_ref_wds), total_err, total_ref_wds);

print_confusion_pairs();
print_inserted_words();
print_deleted_words();
print_misrecognized_words();
print_substituted_words();
print_split_and_merge_words();

printf("\n\n");
return;
}

/*****************************************************************/
/* Prints confusion pairs in descending order by occurence */

print_confusion_pairs()
{
int i;
int long_ref = 0;
int long_hyp = 0;

sort_conf_by_count(confusion_ref,confusion_hyp,confusion_count,
			confused_pairs,
			in_a_split,in_splits,
			in_a_merge,in_merges);
printf("\n\nCONFUSION PAIRS (%d):\n\n", confused_pairs);
for (i=0; i<confused_pairs; i++) {
	long_ref = max(long_ref,strlen(confusion_ref[i]));
	long_hyp = max(long_hyp,strlen(confusion_hyp[i]));
}
for (i=0; i<confused_pairs; i++)
	printf(
		"  %4d %*s %-*s\n",
/*		"  %4d %*s %-*s     %3d %3d %3d %3d\n", */
		confusion_count[i],
		long_ref, confusion_ref[i],
		long_hyp, confusion_hyp[i] /* ,
		in_a_split[i], in_splits[i],
		in_a_merge[i], in_merges[i] */
		);
if (confused_pairs > 0)
	printf("------\n  %4d\n\n",total_sub);
}

/*****************************************************************/
/* Prints inserted words in descending order by occurence */

print_inserted_words()
{
int i;

sort_table_by_count(insertion,insertion_count,inserted_words);
printf("\n\nINSERTED WORDS (%d):\n\n", inserted_words);
for (i=0; i<inserted_words; i++)
	printf("  %4d    %-*s\n", insertion_count[i], WC, insertion[i]);
if (inserted_words > 0)
	printf("------\n  %4d\n",total_ins);
}

/*****************************************************************/
/* Prints deleted words in descending order by occurence */

print_deleted_words()
{
int i;

sort_table_by_count(deletion,deletion_count,deleted_words);
printf("\n\nDELETED WORDS (%d):\n\n", deleted_words);
for (i=0; i<deleted_words; i++)
	printf("  %4d    %-*s\n", deletion_count[i], WC, deletion[i]);
if (deleted_words > 0)
	printf("------\n  %4d\n",total_del);
}

/*****************************************************************/
/* Prints misrecognized words in descending order by occurence */

print_misrecognized_words()
{
int i;

sort_table_by_count(misrec,misrec_count,misrec_words);
printf("\n\nMISRECOGNIZED WORDS (%d):\n\n", misrec_words);
for (i=0; i<misrec_words; i++)
	printf("  %4d    %-*s\n", misrec_count[i], WC, misrec[i]);
if (misrec_words > 0)
	printf("------\n  %4d\n",total_sub);
}

/*****************************************************************/
/* Prints substituted words in descending order by occurence */

print_substituted_words()
{
int i;

sort_table_by_count(subst,subst_count,subst_words);
printf("\n\nSUBSTITUTED WORDS (%d):\n\n", subst_words);
for (i=0; i<subst_words; i++)
	printf("  %4d    %-*s\n", subst_count[i], WC, subst[i]);
if (subst_words > 0)
	printf("------\n  %4d\n",total_sub);
}

/*****************************************************************/
/*  Prints occurences of:
	1) Splits (split candidates found in table)
	2) Split candidates
	3) Merges (merge candidates found in table)
	4) Merge candidates
*/

print_split_and_merge_words()
{
int i, j;

printf("\n\nOCCURENCES OF SPLITS (%d):\n\n", total_splits);
for (i=0; i<split_merge_TL; i++) {
	if (split_occs[i] > 0) {
		printf("  %4d ",split_occs[i]);
		printf("%s ==> ",split_merge_word[i]);
		for (j=0; j<SMSL; j++)
			printf("%s ",split_merge_sequence[i][j]);
		printf("\n");
	}
}
if (split_merge_TL > 0)
	printf("------\n  %4d\n",total_splits);

printf("\n\nOCCURENCES OF SPLIT CANDIDATES (%d):\n\n", total_split_candidates);
for (i=0; i<SMC_TL; i++) {
	if (SC_occs[i] > 0) {
		printf("  %4d ",SC_occs[i]);
		printf("%s ==> ",SMC_word[i]);
		for (j=0; j<SMSL; j++)
			printf("%s ",SMC_sequence[i][j]);
		printf("\n");
	}
}
if (SMC_TL > 0)
	printf("------\n  %4d\n",total_split_candidates);

printf("\n\nOCCURENCES OF MERGES (%d):\n\n", total_merges);
for (i=0; i<split_merge_TL; i++) {
	if (merge_occs[i] > 0) {
		printf("  %4d ",merge_occs[i]);
		for (j=0; j<SMSL; j++)
			printf("%s ",split_merge_sequence[i][j]);
		printf("==> %s",split_merge_word[i]);
		printf("\n");
	}
}
if (split_merge_TL > 0)
	printf("------\n  %4d\n",total_merges);

printf("\n\nOCCURENCES OF MERGE CANDIDATES (%d):\n\n", total_merge_candidates);
for (i=0; i<SMC_TL; i++) {
	if (MC_occs[i] > 0) {
		printf("  %4d ",MC_occs[i]);
		for (j=0; j<SMSL; j++)
			printf("%s ",SMC_sequence[i][j]);
		printf("==> %s",SMC_word[i]);
		printf("\n");
	}
}
if (SMC_TL > 0)
	printf("------\n  %4d\n",total_merge_candidates);
}

/*****************************************************************/
/* Sorts first size elements of input table of words and counts
   into descending order by counts */

sort_table_by_count(char wd[TL][WC], int *ct, int size)
{
int i, exchanges;

do {
	exchanges = FALSE;
	for (i=0; i<((size-1)-1); i++)
		if (ct[i] < ct[i+1]) {
			swap_strings(wd[i],wd[i+1]);
			swap_ints(ct[i],ct[i+1]);
			exchanges = TRUE;
		}
} while (exchanges);
}

/******************************************************************/
/* Sorts confusion pairs into descending order by occurence count */

sort_conf_by_count(wd1,wd2,ct,size,in_a_split,in_splits,in_a_merge,in_merges)
char wd1[TL][WC], wd2[TL][WC];
int ct[TL],in_a_split[TL],in_splits[TL],in_a_merge[TL],in_merges[TL],size;
{
int i, exchanges;

do {
	exchanges = FALSE;
	for (i=0; i<((size-1)-1); i++)
		if (ct[i] < ct[i+1]) {
			swap_strings(wd1[i],wd1[i+1]);
			swap_strings(wd2[i],wd2[i+1]);
			swap_ints(ct[i],ct[i+1]);
			swap_ints(in_a_split[i],in_a_split[i+1]);
			swap_ints(in_splits[i],in_splits[i+1]);
			swap_ints(in_a_merge[i],in_a_merge[i+1]);
			swap_ints(in_merges[i],in_merges[i+1]);
			exchanges = TRUE;
		}
} while (exchanges);
}

/*********************************************************************/
/* Checks that all words in input vector are in the lexicon list. */

check_lexicon(int count,char **wds, int *unknown)
{
int wd, top, bottom, middle, found, diff;
float float_middle;

*unknown = 0;
if (lexicon_size!=0)
	for (wd=0; wd<count; wd++) {
	    top = 0;
	    bottom = lexicon_size - 1;
	    if (dbug_lexicon) printf("top = %d and bottom = %d\n",top,bottom);
	    found = FALSE;
	    while ((!found) && (top<=bottom)) {
		    float_middle = (((float)(top+bottom))/2.0);
		    middle = sround(float_middle);
		    if (dbug_lexicon) printf("middle = %d\n", middle);
		    diff = strcmp(word[middle],wds[wd]);
		    if (dbug_lexicon) printf("Comparing \"%s\" and \"%s\" => %d\n", word[middle], wds[wd], diff);
		    if (diff == 0)
			    found = TRUE;
		    else if (diff > 0)
			    bottom = middle - 1;
		    else
			    top = middle + 1;
	    }
	    if (! found) {
		    *unknown += 1;
		    fflush(stderr);
		    printf("Warning: \"%s\" is not in the lexicon.\n", wds[wd]);
		    fflush(stdout);
		    fprintf(stderr,"Warning: \"%s\" is not in the lexicon.\n", wds[wd]);
	    }
	}
}

/******************************************************************/
/* Parses a sentence string in SNOR format (unaligned) and stores
   the individual words in the vector wd. The number of words found
   is placed in the address n. The optional sentence code number,
   if found, is placed in the address sent_code. */

parse(char *unaligned, int *n, char **wd, char *sent_code_string)
{
int b, x, y;
char *z, *a[SL];

strcpy(sent_code_string,"");
/*
 * delete new line, backward compat. (FAA)
 */
z = rindex (unaligned, '\n');
if (z != 0)
	*z = '\0';
/*
 * Find the *last* '(', bug fix (FAA)
 */
z = rindex (unaligned, '(');
if (z != 0) {
	if (sscanf(z,"%s",sent_code_string) != 1) strcpy(sent_code_string,"");
	if (dbug) printf("sent_code_string = \"%s\"\n",sent_code_string);
	*(--z) = '\0';
}
b = 0;
z = unaligned;
while (*z != '\0') {
	if (*z == ' ') {
		*z = '\0';
		++z;
		if (is_SNOR(*z)) a[b++] = z;
	}
	else if ((z == unaligned) && (is_SNOR(*z)))
		a[b++] = z++;
	else
		++z;
}

for (x=0, y=0; x<b; x++)
	if (strcmp(a[x],"SILENCE") != 0)
		wd[y++] = a[x];
*n = y;
}

/*****************************************************************/
/* Parses a line following a sentence (reference or hypothesis)
   to retrieve the beginning and end times for the words in the
   sentence. */

parse_times(line,begin,end,n)
char *line;
int begin[SL], end[SL], n;
{
int a, pairs = 0;
char *p = line;

for (a=0; ((*p) && (a<n)); a++) {
	if (sscanf(p,"%d%d",&begin[a],&end[a]) != 2) break;
	++pairs;
	while ((*p) && ! (isdigit(*p))) ++p;
	while ((*p) &&   (isdigit(*p))) ++p;
	while ((*p) && ! (isdigit(*p))) ++p;
	while ((*p) &&   (isdigit(*p))) ++p;
}
if (dbug) {
	printf("Parsed out %d pairs of begin/end times:\n",pairs);
	for (a=0; a<pairs; a++)
		printf("\t(%d,%d)\n",begin[a],end[a]);
}
if (pairs != n) print_err(E_time_pairs,n,line,"");
}

/*****************************************************************/
/* Aligns two string vectors and places the resulting aligned string
   created from the alignment at addresses ref_aligned and hyp_aligned.
   The types of errors (OK,SUB,INS,DEL) are returned in the vector
   s5 for each position in the aligned strings. */

int align(arg1,n1,arg2,n2,ref_aligned,hyp_aligned,s1,s2,s5,
		part_of_split,part_of_merge)

char *arg1[], *arg2[];			/* word vectors               */
int n1, n2;				/* sizes of the word vectors  */
char *ref_aligned, *hyp_aligned;	/* aligned sentence strings   */
char *s1[2*SL], *s2[2*SL];		/* aligned word vectors	      */
int s5[2*SL];				/* aligned error type vector  */
int part_of_split[2*SL];		/* count of times a confusion */
int part_of_merge[2*SL];		/* pair was part of a split   */
					/* or merge                   */
{
    int k, m;
    char *s3[2*SL], *s4[2*SL];
    int backtr[2*SL+1];
    /*
     * c and bp aren't on the stack because the compiler can deal with
     * large stack allocations (faa@cs.cmu.edu)
     */
    static int c[SL][SL], bp[SL][SL];
    int width[2*SL];
    char *stars, buffer[WC+1], field[WC+1+ONE];
    int split_count = 0, merge_count = 0, sub = 0, ins = 0, del = 0, err = 0, split_cands = 0, merge_cands = 0;
    int product, diff, pos;

    if (dbug2) printf("\nWord vector sizes are %d and %d\n", n1, n2);

    if ((n1>=SL) && (n2>=SL)) print_err(E_both_sents_long,max(n1,n2)-SL+1,"","");
    else if (n1>=SL)	  print_err(E_ref_sent_long,n1-SL+1,"","");
    else if (n2>=SL)	  print_err(E_hyp_sent_long,n2-SL+1,"","");

    create_penalty_table(c,bp,n1,n2,arg1,arg2);
    strings_to_lower_case(n1,arg1);
    strings_to_lower_case(n2,arg2);
    k = backtrace(n1,n2,bp,backtr,&err,&sub,&ins,&del);
    align_words(arg1,arg2,s1,s2,s3,s4,s5,part_of_split,part_of_merge,k,backtr);
    switches = ((attempt_realignment) ? realigner(s1,s2,s5,k) : 0);
    fill_string(stars=buffer,WC,"*");
    for (m=0; m<k; m++)
	width[m] = max(strlen(s1[m]),strlen(s2[m]));
    fill_gaps(k,s1,s2,s5,width,stars);
    /* print aligned strings */
    if (dbug) printf("\n\nREF:\t");
    for (m=0; m<k; m++) {
	sprintf(field, "%-*s ", width[m], s1[m]);
	if (dbug) printf("%s", field);
	strcat(ref_aligned, field);
    }

    if (dbug) printf("\nHYP:\t");
    for (m=0; m<k; m++) {
	sprintf(field, "%-*s ", width[m], s2[m]);
	if (dbug) printf("%s",field);
	strcat(hyp_aligned, field);
    }

    if (dbug2 || (dbug && err)) { 		/* showcase error columns */
	printf("\n    \t");
	for (m=0; m<k; m++) printf("%-*s ", width[m], s4[m]);
	printf("\n    \t");
	for (m=0; m<k; m++) printf("%-*s ", width[m], s3[m]);
	printf("\n    \t");
	for (m=0; m<k; m++) printf("%-*d ", width[m], s5[m]);
    }
    if (dbug) printf("\n");

    for (m=0; m < (k-1); m++) {
	product = s5[m] * s5[m+1];
	diff    = s5[m] - s5[m+1];
	if (product == (SUB * INS)) { /* adjacent sub. and ins. */
	    store_split_candidate(s1,s2,m);
	    ++split_cands;
	    pos = store_sent(SPL,s1,s2,m,diff);
	    if (check_for_split(m,s1,s2,part_of_split)) {
		++split_count;
		found_in_table(pos);
	    }
	}
	else if (product == (SUB * DEL)) { /* adjacent sub. and del. */
	    store_merge_candidate(s1,s2,m);
	    ++merge_cands;
	    pos = store_sent(MRG,s1,s2,m,diff);
	    if (check_for_merge(m,s1,s2,part_of_merge)) {
		++merge_count;
		found_in_table(pos);
	    }
	}
    }

    /* return important counts */
    sent_ref_wds = n1;			/* for display later */
    sent_hyp_wds = n2;
    sent_sub = sub;
    sent_ins = ins;
    sent_del = del;
    sent_err = err;
    sent_aligned_wds = k;
    sent_corr = k - err;
    splits = split_count;
    split_candidates = split_cands;
    merges = merge_count;
    merge_candidates = merge_cands;

    return(err);
}

/************************************************************************/
/* Converts a vector of strings to a vector of strings of all lower case
   charcters. */

strings_to_lower_case(n,arg)
int n;
char *arg[];
{
int m;

for (m=0; m<n; m++)
	string_to_lower_case(arg[m]);
}

/***************************************************************************/
/* Creates table for backtracing later */

create_penalty_table(c,bp,n1,n2,arg1,arg2)
int c[SL][SL], bp[SL][SL], n1, n2;
char *arg1[], *arg2[];
{
int i, j, ct, cx;

c[0][0] = 0, bp[0][0] = OK;
for (i=1; i <= n1; i++) c[i][0] = DEL_penalty * i, bp[i][0] = DEL;
for (j=1; j <= n2; j++) c[0][j] = INS_penalty * j, bp[0][j] = INS;
for (i=1; i <= n1; i++) {
	for (j=1; j <= n2; j++) {
		if (dbug2) print_penalty_table(c,bp,n1,n2);
		cx = MAX_penalty;
		ct = c[i-1][j] + DEL_penalty;
		if (ct<cx) cx=ct, c[i][j]=ct, bp[i][j]=DEL;
		if (strcmp(arg1[i-1],arg2[j-1])==0) {
			ct = c[i-1][j-1];
			if (ct<cx) cx=ct, c[i][j]=ct, bp[i][j]=OK;
		}
		else if ((homophones(arg1[i-1],arg2[j-1])) && HC) {
			ct = c[i-1][j-1];
			if (ct<cx) cx=ct, c[i][j]=ct, bp[i][j]=OK;
		}
		else {
			ct= c[i-1][j-1] + SUB_penalty;
			if (ct<cx) cx=ct, c[i][j]=ct, bp[i][j]=SUB;
		}
		ct = c[i][j-1] + DEL_penalty;
		if (ct<cx) cx=ct, c[i][j]=ct, bp[i][j]=INS;
	}
}
if (dbug2) print_penalty_table(c,bp,n1,n2); 
}

/***************************************************************************/
/* Prints penalty table -- only called when debugging */

print_penalty_table(c,bp,n1,n2)
int c[SL][SL], bp[SL][SL], n1, n2;
{
int a,b;
printf("\n");
for (a=0; a<=n1; a++) {
	for (b=0; b<=n2; b++)
		printf("%7d %1d", c[a][b], bp[a][b]);
	printf("\n");
	}
}

/***************************************************************************/
/* Backtraces through penalty table */

int backtrace(n1,n2,bp,backtr,err,sub,ins,del)
int n1,n2,bp[SL][SL],backtr[2*SL+1],*err,*sub,*ins,*del;
{
int i, j, k;

if (dbug2) printf("\nStarting backtrace.\n");
for (k=0, i=n1, j=n2; i>=0 && j>=0; k++) {
	backtr[k] = bp[i][j];
	switch (bp[i][j]) {
		case OK  : i--; j--;        	          break;
		case SUB : i--; j--; (*err)++; (*sub)++;  break;
		case INS :      j--; (*err)++; (*ins)++;  break;
		case DEL : i--;      (*err)++; (*del)++;  break;
	}
}
if (dbug2) for (i=0; i<= k; i++)
	printf("backtr[%2d] = %3d\n", i, backtr[i]);

return(--k);	/* return size after dumping faked entry */
}

/***************************************************************************/
/* Based on the backtrace information, words are aligned as appropriate
   with insertions and deletions causing asterisks to be placed in the
   word vectors */

align_words(arg1,arg2,s1,s2,s3,s4,s5,part_of_split,part_of_merge,k,backtr)
char *arg1[], *arg2[], *s1[], *s2[], *s3[], *s4[];
int s5[], part_of_split[], part_of_merge[], k, backtr[];
{
int i,j,l,m;

for (l = 0; l < (2*SL); l++) {
	part_of_split[l] = part_of_merge[l] = 0;
	s1[l] = s2[l] = s3[l] = s4[l] = "";
	s5[l] = OK;
}
for (m=k-1, i=j=l=0; (m >= 0); m--, l++) {
	if (backtr[m] != INS) s1[l] = arg1[i++]; else s1[l] = gap;
	if (backtr[m] != DEL) s2[l] = arg2[j++]; else s2[l] = gap;
	if (backtr[m] !=  OK) { s4[l] = "^"; s5[l] = backtr[m]; }
	switch(backtr[m]) {
	 	case OK:	break;
		case SUB:	s3[l] = "S"; 
				string_to_upper_case(s1[l]);
				string_to_upper_case(s2[l]);
				break;
		case INS:	s3[l] = "I";
				string_to_upper_case(s2[l]);
				break;
		case DEL:	s3[l] = "D";
				string_to_upper_case(s1[l]);
				break;
	}
}
}

/***************************************************************************/
/* Returns TRUE if the input strings are homophones, FALSE otherwise */

int homophones(s1,s2)
char *s1, *s2;
{
int result = FALSE;
int loop;

for (loop=0; ((loop < homophone_TL) && (! result)); loop++)
	if (((strcmp(s1,hom1[loop])==0) && (strcmp(s2,hom2[loop])==0)) ||
	    ((strcmp(s2,hom1[loop])==0) && (strcmp(s1,hom2[loop])==0)))
		result = TRUE;
if (dbug) if (result) printf("%s and %s are homophones\n",s1,s2);
return(result);
}

/***************************************************************************/
/* If the appropriate constant is TRUE, this procedure is called. It scans
   the aligned reference and hypothesis word vectors end-to-beginning looking
   for an insertion adjacent to a substitution (or a deletion adjacent to a
   substitution). When found, the misrecognized (or substituted) word will
   be realigned with the inserted (deleted) word if that pair is more similar
   than the pair previously called a substitution. "Switches" can propagate
   down the string. Passes continue until no switches are possible. */

int realigner(s1,s2,s5,k)
char *s1[2*SL], *s2[2*SL];
int s5[2*SL], k;
{
int switches = 0;
int changes_made = TRUE;
int diff, l, m;

while (changes_made) {
	changes_made = FALSE;
	for (m=(k-1); m>=0; m--) {
		diff = s5[m] - s5[m+1];
		l = s5[m] * s5[m+1];
		if ((l==2)||(l==3)) /* SUB (1) next to INS (2) or DEL (3) */
			if (try_to_realign(diff,&s1[m],&s1[m+1],&s2[m],&s2[m+1])) {
				swap_ints(s5[m],s5[m+1]);
				++switches;
				changes_made = TRUE;
				if (dbug) printf("Switched at %d\n", m);
			}
			else
				if (dbug) printf("Didn\'t switch at %d\n", m);
	}
	if (dbug) if (changes_made) printf("Another pass to follow\n");
}
return(switches);
}

/***************************************************************************/
/* See documentation for procedure "realigner" */

int try_to_realign(code,r1,r2,t1,t2)
int code;
char **r1, **r2, **t1, **t2;
{
int made_switch = FALSE;
float similarity();

switch (code) {
	case -2:	/* Sub + Del */
			if (similarity(*r2,*t1)>=similarity(*r1,*t1)) {
				swap_ptrs(t1,t2);
				made_switch = TRUE;
			}
			break;
	case -1:	/* Sub + Ins */
			if (similarity(*r1,*t2)>similarity(*r1,*t1)) {
				swap_ptrs(r1,r2);
				made_switch = TRUE;
			}
			break;
	case 1:		/* Ins + Sub */
			if (similarity(*r2,*t1)>=similarity(*r2,*t2)) {
				swap_ptrs(r1,r2);
				made_switch = TRUE;
			}
			break;
	case 2:		/* Del + Sub */
			if (similarity(*r1,*t2)>similarity(*r2,*t2)) {
				swap_ptrs(t1,t2);
				made_switch = TRUE;
			}
			break;
}
return(made_switch);
}

/***************************************************************************/

fill_gaps(k,s1,s2,s5,width,stars)
int k, s5[2*SL], width[2*SL];
char *s1[2*SL], *s2[2*SL], *stars;
{
int m;

for (m=0; m<k; m++) {				/* represent gaps as	*/
	if (s5[m] == INS) {			/* sequences of "*"s	*/
		s1[m] = stars;
		while (strlen(s1[m]) > width[m])
			++s1[m];
	}
	else if (s5[m] == DEL) {
		s2[m] = stars;
		while (strlen(s2[m]) > width[m])
			++s2[m];
	}
}
}

/***************************************************************************/
/* Stores the splits, split candidates, merges, and merge candidates of
   the current sentence for display later */

int store_sent(type_code,s1,s2,m,location_code)
int type_code, m, location_code;
char *s1[2*SL], *s2[2*SL];
{
int a;

switch (type_code) {
      case SPL: strcpy(sent_SMC_word[sent_SMC_TL],s1[(location_code>=0) ? m+1 : m]);
		for (a=0; a<SMSL; a++)
			strcpy(sent_SMC_sequence[sent_SMC_TL][a],s2[m+a]);
		sent_SMC_type[sent_SMC_TL]  = SPL;
		sent_SMC_found[sent_SMC_TL] = FALSE;
		break;
      case MRG: strcpy(sent_SMC_word[sent_SMC_TL],s2[(location_code>=0) ? m+1 : m]);
		for (a=0; a<SMSL; a++)
			strcpy(sent_SMC_sequence[sent_SMC_TL][a],s1[m+a]);
		sent_SMC_type[sent_SMC_TL]  = MRG;
		sent_SMC_found[sent_SMC_TL] = FALSE;
		break;
}
return(sent_SMC_TL++);
}

/***************************************************************************/
/* Turns on the flag that signifies that the current sentence's given
   split candidate or merge candidate at position "pos" in the table
   is a split or merge (found in the predeclared list). */

found_in_table(pos)
int pos;
{
sent_SMC_found[pos] = TRUE;
}

/***************************************************************************/
/* Initializes various tables and variables */

init_tables()
{
int i;

for (i=0; i<TL; i++) {
	insertion_count[i]	= 1;
	deletion_count[i]	= 1;
	misrec_count[i]		= 1;
	subst_count[i]		= 1;
	confusion_count[i]	= 1;
	in_a_split[i]		= 0;
	in_splits[i]		= 0;
	in_a_merge[i]		= 0;
	in_merges[i]		= 0;
}
for (i=0; i<STL; i++) {
	split_occs[i]	= 0;
	merge_occs[i]	= 0;
	SC_occs[i]	= 0;
	MC_occs[i]	= 0;
}
SMC_TL		= 0;
if (lexicon_present && show_lexicon) print_lexicon();
}

/***************************************************************************/
/* Calls appropriate procedures to record confusions, insertions, deletions,
   misrecognitions, and substitutions of particular words */

store_word_confusions(ref,hyp,comparison,part_of_split,part_of_merge)
char *ref[], *hyp[];
int comparison[],part_of_split[],part_of_merge[];
{
int i;

for (i=0; i<sent_aligned_wds; i++)
	switch( comparison[i] ) {
		case OK:	break;
		case SUB: 	confusion_of(ref[i],hyp[i],
					part_of_split[i],part_of_merge[i]);
				misrec_of(ref[i]);
				subst_of(hyp[i]);
				break;
		case INS: 	insertion_of(hyp[i]);
				break;
		case DEL: 	deletion_of(ref[i]);
				break;
	}
}

/***************************************************************************/
/* Stores confusion pair along with information signifying if it is part
   a split or merge and how many splits or merges it is part of */

confusion_of(ref,hyp,splits,merges)
char *ref, *hyp;
int splits, merges;
{
int i;
for (i=0; i<confused_pairs; i++)
	if ((strcmp(ref,confusion_ref[i])==0) && (strcmp(hyp,confusion_hyp[i])==0)) {
		++confusion_count[i];
		if (splits) {
			++in_a_split[i];
			in_splits[i] += splits;
		}
		if (merges) {
			++in_a_merge[i];
			in_merges[i] += merges;
		}
		return;
	}
strcpy(confusion_ref[confused_pairs],ref);
strcpy(confusion_hyp[confused_pairs],hyp);
if (splits) {
	++in_a_split[confused_pairs];
	in_splits[confused_pairs] += splits;
}
if (merges) {
	++in_a_merge[confused_pairs];
	in_merges[confused_pairs] += merges;
}
++confused_pairs;
}

/***************************************************************************/
/* Records the insertion of a word */

insertion_of(wd)
char *wd;
{
int i;
for (i=0; i<inserted_words; i++)
	if (strcmp(wd,insertion[i])==0) {
		++insertion_count[i];
		return;
	}
strcpy(insertion[inserted_words++],wd);
}

/***************************************************************************/
/* Records the deletion of a word */

deletion_of(wd)
char *wd;
{
int i;
for (i=0; i<deleted_words; i++)
	if (strcmp(wd,deletion[i])==0) {
		++deletion_count[i];
		return;
	}
strcpy(deletion[deleted_words++],wd);
}

/***************************************************************************/
/* Records the misrecognition of a word */

misrec_of(wd)
char *wd;
{
int i;
for (i=0; i<misrec_words; i++)
	if (strcmp(wd,misrec[i])==0) {
		++misrec_count[i];
		return;
	}
strcpy(misrec[misrec_words++],wd);
}

/***************************************************************************/
/* Records the substitution of a word */

subst_of(wd)
char *wd;
{
int i;
for (i=0; i<subst_words; i++)
	if (strcmp(wd,subst[i])==0) {
		++subst_count[i];
		return;
	}
strcpy(subst[subst_words++],wd);
}

/***************************************************************************/
/* Determines if a split candidate is a split in the predeclared list */

int check_for_split(m,s1,s2,part_of_split)
char *s1[2*SL], *s2[2*SL];
int m, part_of_split[2*SL];
{
int found_split = FALSE;
int occurs, problems, mismatches;
int a,b,loc;

if (dbug) printf("Split candidate at position %d\n", m);
for (a=0; (a<split_merge_TL) && (! found_split); a++) {
	occurs = problems = mismatches = 0;
	if (dbug2) printf("split_merge_word[%d] = \"%s\"\n", a, split_merge_word[a]);
	for (b=0; b<SMSL; b++) {
		if (strcmp(split_merge_word[a],s1[m+b])==0) {
			loc = m + b;
			occurs++;
		}
		else if (!(indx(s1[m+b],"*")>=0)) {
			problems++;
			break;
		}
		if (strcmp(split_merge_sequence[a][b],s2[m+b]) != 0) {
			mismatches++;
			if (dbug2) printf("Split Mismatch: \"%s\" <> \"%s\"\n",split_merge_sequence[a][b],s2[m+b]);
			break;
		}
	}
	if ((occurs==1) && (problems==0) && (mismatches==0)) {
		found_split = TRUE;
		++split_occs[a];
		++part_of_split[loc];
	}
}

if (dbug) printf("%s\n", (found_split ? "SPLIT" : "NO SPLIT"));
return(found_split);
}

/***************************************************************************/
/* Determines if a merge candidate is a merge in the predeclared list */
int check_for_merge(m,s1,s2,part_of_merge)
char *s1[2*SL], *s2[2*SL];
int m, part_of_merge[2*SL];
{
int found_merge = FALSE;
int occurs, problems, mismatches;
int a,b,loc;

if (dbug) printf("Merge candidate at position %d\n", m);
for (a=0; (a<split_merge_TL) && (! found_merge); a++) {
	occurs = problems = mismatches = 0;
	if (dbug2) printf("split_merge_word[%d] = \"%s\"\n", a, split_merge_word[a]);
	for (b=0; b<SMSL; b++) {
		if (strcmp(split_merge_word[a],s2[m+b])==0) {
			loc = m + b;
			occurs++;
		}
		else if (!(indx(s2[m+b],"*")>=0)) {
			problems++;
			break;
		}
		if (strcmp(split_merge_sequence[a][b],s1[m+b]) != 0) {
			mismatches++;
			if (dbug2) printf("Merge Mismatch: \"%s\" <> \"%s\"\n",split_merge_sequence[a][b],s1[m+b]);
			break;
		}
	}
	if ((occurs==1) && (problems==0) && (mismatches==0)) {
		found_merge = TRUE;
		++merge_occs[a];
		++part_of_merge[loc];
	}
}

if (dbug) printf("%s\n", (found_merge ? "MERGE" : "NO MERGE"));
return(found_merge);
}

/***************************************************************************/
/* Records a split candidate */
store_split_candidate(s1,s2,m)
char *s1[2*SL], *s2[2*SL];
int m;
{
int found = FALSE;
int a,b,offset;

if (dbug) printf("Storing split candidate at position %d\n", m);
offset = (*s1[m]=='*') ? 1 : 0;
for (a=0; (a<SMC_TL) && (! found); a++) {
	if (strcmp(SMC_word[a],s1[m+offset]) != 0) break;
	for (b=0; b<SMSL; b++)
		if (strcmp(SMC_sequence[a][b],s2[m+b]) != 0) break;
	found = TRUE;
	++SC_occs[a];
}
if (! found) {
	strcpy(SMC_word[SMC_TL],s1[m+offset]);
	for (b=0; b<SMSL; b++)
		strcpy(SMC_sequence[SMC_TL][b],s2[m+b]);
	++SC_occs[SMC_TL];
	if (++SMC_TL > STL) {	/* exceeded table size */
	    fprintf(stderr, "align: STL = %d is too small\n", STL);
	    exit(-1);
	}

}
if (dbug) printf("%s\n", (found ? "Old entry" : "New entry"));
}

/***************************************************************************/
/* Records a merge candidate */

store_merge_candidate(s1,s2,m)
char *s1[2*SL], *s2[2*SL];
int m;
{
int found = FALSE;
int a,b,offset;

if (dbug) printf("Storing merge candidate at position %d\n", m);
offset = (*s2[m] == '*') ? 1 : 0;
for (a=0; (a<SMC_TL) && (! found); a++) {
	if (strcmp(SMC_word[a],s2[m+offset]) != 0) break;
	for (b=0; b<SMSL; b++)
		if (strcmp(SMC_sequence[a][b],s1[m+b]) != 0) break;
	found = TRUE;
	++MC_occs[a];
}
if (! found) {
	strcpy(SMC_word[SMC_TL],s2[m+offset]);
	for (b=0; b<SMSL; b++)
		strcpy(SMC_sequence[SMC_TL][b],s1[m+b]);
	++MC_occs[SMC_TL];
	if (++SMC_TL > STL) {
	    /* exceeded table size */
	    fprintf(stderr, "align: STL = %d is too small\n", STL);
	    exit(-1);
	}
}
if (dbug) printf("%s\n", (found ? "Old entry" : "New entry"));
}

/***************************************************************************/
/* Determines if string at t is a substring of the string at s */

int indx(s,t)
char *s, *t;
{
int i, j, k;

for (i=0; s[i] != '\0'; i++) {
	for (j=i, k=0; t[k] != '\0' && s[j] == t[k]; j++, k++)
		;
	if (t[k] == '\0') return(i);
}
return(-1);
}

/**********************************************************************/
/* Reads in the lexicon words from an external file */

int read_lexicon(lex_file)
    char *lex_file;
{
    FILE *lexicon_fp, *fopen();
    int wd = - 1, inword = FALSE, loop;
    char c, *ch = lexicon_buffer;
    
    if ((lexicon_fp = fopen(lex_file,"r")) == NULL) {
	print_err(E_bad_lexicon_file,0,lex_file,"");
	lexicon_present = FALSE;
	return(0);
    }
    else {
	while ((c = getc(lexicon_fp)) != EOF) {
	    if (is_SNOR(c) && (c != '\n')) {
		if (! inword) {
		    inword = TRUE;
		    word[++wd] = ch;
		}
		*(ch++) = c;
	    }
	    else if (c == '\n') {
		inword = FALSE;
		*(ch++) = '\0';
	    }
	    else if ((c == ' ') || (c == '\t')) {
		inword = FALSE;
	    }
	    else
		print_err(E_illegal_char,c,LEXICON_FILE,"");
	}
	lexicon_present = TRUE;
	fclose(lexicon_fp);
	lexicon_sorted = TRUE;
	for (loop=0; ((loop<(wd-1)) && (lexicon_sorted)); loop++)
	    if (strcmp(word[loop],word[loop+1])>0) {
		lexicon_sorted = FALSE;
		print_err(E_unsorted_lexicon,0,word[loop],word[loop+1]);
	    }
	return(wd + 1);
    }
}

/**********************************************************************/
/* Reads in the homphone pairs from the external file */

int read_homophones(homo_file)
char *homo_file;
{
FILE *homophone_fp, *fopen();
int loop = 0;

if ((homophone_fp = fopen(homo_file,"r")) == NULL) {
	print_err(E_bad_hom_file,0,"HOMOPHONES","");
	return(0);
}

while (fscanf(homophone_fp,"%s%s",hom1[loop],hom2[loop])==2)
	++loop;

fclose(homophone_fp);
return(loop);
}

/***************************************************************************/

#include "similarity.c"


