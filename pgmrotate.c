#include <stdio.h>
#include <assert.h>
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
    
  return 0;
}
