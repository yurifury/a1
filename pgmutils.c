#include "pgmutils.h"

void print_usage(void) {
  pm_usage("infile outfile");
}

PGMfile read_infile(char * file_name) {
  PGMfile in;

  in.file = pm_openr(file_name);
  in.pix = pgm_readpgm(in.file, &in.width, &in.height, &in.maxval);

  return in;
}

void write_outfile(char * file_name, PGMfile out) {
  out.file = pm_openw(file_name);
  pgm_writepgm(out.file, out.pix, out.width, out.height, out.maxval, pm_plain_output);
}

