#ifndef PGM_UTILS
#define PGM_UTILS

#include <stdio.h>
#include <pgm.h>

typedef struct pgm {
  /* Describes the structure of the pgm file.
   * Basically a lightweight version of the pam struct
   */

  FILE * file;
  int width;
  int height;
  gray maxval;

  /* Stores the actual image.
   * Called pix because we'll be accessing it as pgm.pix[x][y],
   * which reads well.
   */
  gray ** pix;
} PGMfile;

void print_usage(void);
PGMfile read_infile(char * file_name);
void write_outfile(char * file_name, PGMfile out);
void free_pgm(PGMfile pgm);

#endif
