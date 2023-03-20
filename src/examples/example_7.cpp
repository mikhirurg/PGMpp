//
// Created by @mikhirurug on 19.03.2023.
//
#include "../PGMPixel.h"
#include "../PGMImage_impl.h"
#include "../PGMGraphics_impl.h"

int main(int argc, char *argv[]) {

  // Open PGM image "black_square.pgm"
  PGMImage<PGMColorPixel> img = PGMImage<PGMColorPixel>("pgm_img/coast.ppm", 2.2);

  // Create PGMGraphics object
  PGMDitherer<PGMColorPixel> ditherer = PGMDitherer<PGMColorPixel>(img);

  // Flip image vertically
  ditherer.DoFloydSteinbergDithering(2);

  // Save image to output file
  img.WriteImg("output_img/output_7.ppm");
}