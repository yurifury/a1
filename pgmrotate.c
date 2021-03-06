#include <stdio.h>
#include <pgm.h>
#include "pgmutils.h"

int main (int argc, char *argv[]) {
  /* Make this program act like a Netpbm program
     enables flags like -quiet, -version, -plain
     This also calls pm_init
     */
  pm_proginit(&argc, argv);

  // Check for correct number of arguments
  if (argc != 3) {
    print_usage();
    return 1;
  }

  PGMfile in = read_infile(argv[1]);
  PGMfile out = in;

  // Allocate space for the output image
  out.pix = pgm_allocarray(out.width, out.height);

  for (int i = 1; i <= in.height; ++i) {
    for (int j = 1; j <= in.width; ++j) {
      // Rotate it
      out.pix[out.height - i][out.width - j] = in.pix[i-1][j-1];
    }
  }

  write_outfile(argv[2], out);

  free_pgm(in);
  free_pgm(out);

  return 0;
}
