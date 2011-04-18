#include <stdio.h>
#include <stdbool.h>
#include <pgm.h>

#include <sys/mman.h>
#include <unistd.h>
#include "pgmutils.h"

#ifndef MAP_ANONYMOUS
#ifdef MAP_ANON
#define MAP_ANONYMOUS MAP_ANON
#endif
#endif

#define MSIZE 7
#define HALFM ((MSIZE / 2) + 1)
gray ** gray_1D_to_2D(gray * pix1D, int width, int height);
bool valid_pixel(int width, int height, int y, int x);
void blur_pixel(gray * pix1D, int width, int height, gray maxval, int y, int x, double matrix[MSIZE][MSIZE]);

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
  PGMfile out = in;

  gray *shared_img;
  shared_img = mmap(NULL, sizeof(gray) * in.height * in.width, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);

  if (fork()) {
    wait(0);
    for (int i = in.height / 2; i < in.height; ++i) {
      for (int j = 0; j < in.width; ++j) {
        blur_pixel(shared_img, in.width, in.height, in.maxval, i, j, convolutionM);
      }
    }
    out.pix = gray_1D_to_2D(shared_img, in.width, in.height);
    write_outfile(argv[2], out);

    free_pgm(in);
    munmap(shared_img, sizeof(gray) * in.height * in.width);
    free_pgm(out);
    return 0;
  }
  else {
    // Convert the pixel 2D array into a 1D array
    for (int i = 0; i < in.height; ++i) {
      for (int j = 0; j < in.width; ++j) {
        shared_img[i*in.width + j] = in.pix[i][j];
      }
    }

    for (int i = 0; i < in.height / 2; ++i) {
      for (int j = 0; j < in.width; ++j) {
        blur_pixel(shared_img, in.width, in.height, in.maxval, i, j, convolutionM);
      }
    }
    msync(shared_img, sizeof(gray) * in.height * in.width, MS_SYNC);
    munmap(shared_img, sizeof(gray) * in.height * in.width);
    free_pgm(in);
    return 0;
  }
}

// ALLOCATES and converts the 1D array into a 2D array as originally
gray ** gray_1D_to_2D(gray * pix1D, int width, int height) {
  gray ** pix2D = pgm_allocarray(width, height);
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      pix2D[i][j] = pix1D[i*width + j];
    }
  }
  
  return pix2D;
}

bool valid_pixel(int width, int height, int y, int x) {
  if (y < height && y >= 0 && x < width && x >= 0)
    return true;
  return false;
}

void blur_pixel(gray * pix1D, int width, int height, gray maxval, int y, int x, double matrix[MSIZE][MSIZE]) {
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
      if (valid_pixel(width, height, i, j)) {
        weighted_sum += pix1D[i*width + j] * matrix[mi][mj];
        sum_of_weights += matrix[mi][mj];
      }
    }
  }

  // Now we have all the needed values, apply the blur to the pixel
  double blurred_pix_val = (weighted_sum / sum_of_weights);

  // Buuuuuuuuuut, bound the brightness to maxval
  if (blurred_pix_val > maxval) {
    blurred_pix_val = maxval;
  }

  pix1D[y*width + x] = blurred_pix_val;
}
