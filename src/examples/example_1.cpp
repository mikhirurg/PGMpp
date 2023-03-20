//
// Created by @mikhirurug on 19.03.2023.
//
#include "../PGMPixel.h"
#include "../PGMImage_impl.h"
#include "../PGMGraphics_impl.h"

int main(int argc, char *argv[]) {

  // Open PGM image "mona_lisa.pgm"
  PGMImage<PGMMonoPixel> img = PGMImage<PGMMonoPixel>("pgm_img/mona_lisa.pgm", 2.2);

  // Create PGMGraphics object
  PGMGraphics<PGMMonoPixel> graphics = PGMGraphics<PGMMonoPixel>(img);

  // Invert colors of the image
  graphics.invert();

  // Save image to output file
  img.WriteImg("output_img/output_1.pgm");
}