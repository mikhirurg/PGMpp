//
// Created by @mikhirurug on 19.03.2023.
//
#include "../PGMPixel.h"
#include "../PGMImage_impl.h"
#include "../PGMGraphics_impl.h"

int main(int argc, char *argv[]) {

  // Open PGM image "black_square.pgm"
  PGMImage<PGMMonoPixel> img = PGMImage<PGMMonoPixel>("pgm_img/black_square.pgm", 2.2);

  // Create PGMGraphics object
  PGMGraphics<PGMMonoPixel> graphics = PGMGraphics<PGMMonoPixel>(img);

  // Draw two lines
  graphics.drawLine(100, 10.5, 50, 350, 350, 50, 2.2);
  graphics.drawLine(100, 10.5, 50, 50, 350, 350, 2.2);

  // Save image to output file
  img.WriteImg("output_img/output_4.pgm");
}