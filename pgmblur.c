#include <stdio.h>
#include <stdbool.h>
#include <pgm.h>
#include "pgmutils.h"

#define MSIZE 7
#define HALFM ((MSIZE / 2) + 1)
bool valid_pixel(PGMfile file, int y, int x);
void blur_pixel(PGMfile file, int y, int x, double matrix[MSIZE][MSIZE]);

static double convolutionM[MSIZE][MSIZE] = {
  {0.00000067,  0.00002292,  0.00019117,  0.00038771,  0.00019117,  0.00002292,  0.00000067},
  {0.00002292,  0.00078633,  0.00655965,  0.01330373,  0.00655965,  0.00078633,  0.00002292},
  {0.00019117,  0.00655965,  0.05472157,  0.11098164,  0.05472157,  0.00655965,  0.00019117},
  {0.00038771,  0.01330373,  0.11098164,  0.22508352,  0.11098164,  0.01330373,  0.00038771},
  {0.00019117,  0.00655965,  0.05472157,  0.11098164,  0.05472157,  0.00655965,  0.00019117},
  {0.00002292,  0.00078633,  0.00655965,  0.01330373,  0.00655965,  0.00078633,  0.00002292},
  {0.00000067,  0.00002292,  0.00019117,  0.00038771,  0.00019117,  0.00002292,  0.00000067}
};

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
  PGMfile out = in; // Don't allocate space for out, use in's existing image

  for (int i = 0; i < in.height; ++i) {
    for (int j = 0; j < in.width; ++j) {
      blur_pixel(out, i, j, convolutionM);
    }
  }

  write_outfile(argv[2], out);

  return 0;
}

bool valid_pixel(PGMfile file, int y, int x) {
  if (y <= file.height && y >= 0 && x <= file.width & x >= 0)
    return true;
  return false;
}

void blur_pixel(PGMfile file, int y, int x, double matrix[MSIZE][MSIZE]) {
  // Matrix transformation values
  double weighted_sum = 0.0;
  double sum_of_weights = 0.0;

  /* Iterate through the convolution matrix.
   * We look at the pixels offset from the center x y pixel
   * If it's a valid pixel, we calculate the weighted sum for that point.
   * We also put the matrix value into sum_of_weights to divide by later.
   * Note that we don't do anything with invalid pixels
   */
  for (int mi = 0, i = y - HALFM + 1; mi < MSIZE; ++mi, ++i) {
    for (int mj = 0, j = x - HALFM + 1; mj < MSIZE; ++mj, ++j) {
      if (valid_pixel(file, i, j)) {
        weighted_sum += file.pix[i][j] * matrix[mi][mj];
        sum_of_weights += matrix[mi][mj];
      }
    }
  }

  // Now we have all the needed values, apply the blur to the pixel
  double blurred_pix_val = (weighted_sum / sum_of_weights);

  // Buuuuuuuuuut, bound the brightness to maxval
  if (blurred_pix_val > file.maxval) {
    blurred_pix_val = file.maxval;
  }

  file.pix[y][x] = blurred_pix_val;
}
