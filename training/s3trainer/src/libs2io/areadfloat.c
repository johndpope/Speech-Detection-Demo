/*
	30 May 1989 David R. Fulmer (drf) updated to do byte order
		conversions when necessary.
 */

#include <s2/byteorder.h>

#include <s3/s2io.h>

#include <sys_compat/file.h>
#include <stdio.h>
#include <assert.h>

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

/* Macro to byteswap an int variable.  x = ptr to variable */
#define MYSWAP_INT(x)   *(x) = ((0x000000ff & (*(x))>>24) | \
                                (0x0000ff00 & (*(x))>>8) | \
                                (0x00ff0000 & (*(x))<<8) | \
                                (0xff000000 & (*(x))<<24))
/* Macro to byteswap a float variable.  x = ptr to variable */
#define MYSWAP_FLOAT(x) MYSWAP_INT((int *) x)

int
areadfloat (char *file,
	    float **data_ref,
	    int *length_ref)
{
  FILE		  *fp;
  int             length;
  int             size, ret, byterev, n;
  int             offset;
  float           *data;
  struct stat statbuf;

  if (stat(file, &statbuf) < 0) {
      printf("stat_retry(%s) failed\n", file);
      return -1;
  }

  if ((fp = fopen(file, "rb")) == NULL) {
      printf("fopen(%s,rb) failed\n", file);
      return -1;
  }

  /* Read #floats in header */
  if (fread(&length, sizeof(int), 1, fp) != 1) {
      fclose (fp);
      return -1;
  }

  /* Check if length matches file size */
  byterev = 0;
  if ((length*sizeof(float) + 4) != statbuf.st_size) {
      n = length;
      MYSWAP_INT(&n);

      if ((n*sizeof(float) + 4) != statbuf.st_size) {
          printf("Header size field: %d(%08x); filesize: %d(%08x)\n",
                  length, length, statbuf.st_size, statbuf.st_size);
          fflush(stdout);
          fclose (fp);
          return -1;
      }

      length = n;
      byterev = 1;
  }
  if (length <= 0) {
      printf("Header size field: %d\n",  length); fflush(stdout);
      fclose (fp);
      return -1;
  }

  if ((data = (float *) calloc (length,sizeof(float))) == NULL)
  {
    fprintf (stderr, "areadfloat: %s: can't alloc data\n", file); 
    fflush(stdout);
    fclose (fp);
    return -1;
  }
  if ((ret = fread(data,sizeof(float),length,fp)) != length)
  {
    fprintf (stderr,"areadfloat: %s: expected %d, got %d\n",file,length,ret);
    fflush(stdout);
    fclose (fp);
    free (data);
    return -1;
  }
  fclose (fp);
  *data_ref = (float *) data;
  if (byterev==1) 
     for(offset = 0; offset < length; offset++) MYSWAP_FLOAT(*data_ref+offset);
  *length_ref = length;
  return length;
}

int
areadfloat_part (char *file,
		 int s_coeff,
		 int e_coeff,
		 float **data_ref,
		 int *length_ref)
{
    static char p_file[MAXPATHLEN] = "";
    static FILE *fp = NULL;
    static int len;
    int r_len;
    float *r_buf;
    int i, n;

    static int byterev;
    struct stat statbuf;

    if (strcmp(file, p_file) != 0) {
	if (fp) {
	    fclose(fp);
	}
        if (stat(file, &statbuf) < 0) {
	    fprintf(stderr,"areadfloat_part: unable to open %s for reading;",file);
	    perror("");
	    *data_ref = NULL;
	    *length_ref = 0;
            return -1;
        }

	fp = fopen(file, "rb");
	if (fp == NULL) {
	    fprintf(stderr, "areadfloat_part: unable to open %s for reading;", file);
	    perror("");
	    *data_ref = NULL;
	    *length_ref = 0;

	    return -1;
	}
	strcpy(p_file, file);

	if (fread(&len, sizeof(int), 1, fp) != 1) {
	    fprintf(stderr, "areadfloat_part: unable to read length from %s;", file);
	    perror("");

	    *data_ref = NULL;
	    *length_ref = 0;

	    return -1;
	}

        /* Check if length matches file size */
	byterev = 0;
        if ((len*sizeof(float) + 4) != statbuf.st_size) {
            n = len;
            MYSWAP_INT(&n);

            if ((n*sizeof(float) + 4) != statbuf.st_size) {
                fprintf(stderr,"Header size field: %d(%08x); filesize: %d(%08x)\n",
                    len, len, statbuf.st_size, statbuf.st_size);
	        perror("");
                fclose (fp);
	        *data_ref = NULL;
	        *length_ref = 0;
                return -1;
            }
            len = n;
            byterev = 1;
        }
    }

    if (s_coeff >= len) {
	fprintf(stderr, "areadfloat_part: start of data beyond end of file\n");
	*data_ref = NULL;
	*length_ref = 0;

	return 0;
    }

    if (e_coeff >= len) {
	fprintf(stderr, "areadfloat_part: end of data beyond end of file; resetting\n");

	e_coeff = len-1;
    }

    if (fseek(fp, s_coeff * sizeof(float) + sizeof(int), SEEK_SET) < 0) {
	fprintf(stderr, "areadfloat_part: seek fail;");
	perror("");
	fprintf(stderr, "offset == %u in %s\n",
		s_coeff * sizeof(float) + sizeof(int), file);

	*data_ref = NULL;
	*length_ref = 0;
	return -1;
    }

    r_len = e_coeff - s_coeff + 1;

    assert(s_coeff + r_len <= len);

    r_buf = calloc(r_len, sizeof(float));
    if (fread(r_buf, sizeof(float), r_len, fp) != r_len) {
	fprintf(stderr, "areadfloat_part: unable to read %d coeff @ %d from %s\n",
		r_len, s_coeff, file);
	free(r_buf);
	*data_ref = NULL;
	*length_ref = 0;
	return -1;
    }

    for (i = 0; i < r_len; i++) {
	if (byterev)
	    SWAPF(&r_buf[i]);
    }

    *data_ref = r_buf;
    *length_ref = r_len;

    return r_len;
}


