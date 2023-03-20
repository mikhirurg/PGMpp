//
// Created by @mikhirurug on 19.03.2023.
//
#include "../PGMPixel.h"
#include "../PGMImage_impl.h"
#include "../PGMGraphics_impl.h"

int main(int argc, char *argv[]) {

  // Open PGM image "bird.pgm"
  PGMImage<PGMMonoPixel> img = PGMImage<PGMMonoPixel>("pgm_img/bird.pgm", 2.2);

  // Create PGMGraphics object
  PGMGraphics<PGMMonoPixel> graphics = PGMGraphics<PGMMonoPixel>(img);

  // Flip image vertically
  graphics.verticalFlip();

  // Save image to output file
  img.WriteImg("output_img/output_3.pgm");
}