//
// Created by @mikhirurug on 19.03.2023.
//
#include "../PGMPixel.h"
#include "../PGMImage_impl.h"
#include "../PGMGraphics_impl.h"

int main(int argc, char *argv[]) {

  // Open PGM image "black_square.pgm"
  PGMImage<PGMColorPixel> img = PGMImage<PGMColorPixel>("pgm_img/coast.ppm", 2.2);

  // Create PGMDitherer object
  PGMDitherer<PGMColorPixel> ditherer = PGMDitherer<PGMColorPixel>(img);

  // Perform the Atkinson dithering (2 bits)
  ditherer.DoAtkinsonDithering(2);

  // Save image to output file
  img.WriteImg("output_img/output_5.ppm");
}