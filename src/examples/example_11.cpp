//
// Created by @mikhirurug on 19.03.2023.
//
#include "../PGMPixel.h"
#include "../PGMImage_impl.h"
#include "../PGMGraphics_impl.h"

int main(int argc, char *argv[]) {

  // Open PGM image "coast.ppm"
  PGMImage<PGMColorPixel> img = PGMImage<PGMColorPixel>("pgm_img/coast.ppm", 2.2);

  // Create PGMDitherer object
  PGMDitherer<PGMColorPixel> ditherer = PGMDitherer<PGMColorPixel>(img);

  // Perform the error diffusion dithering
  ditherer.DoErrorDiffDithering(ditherer.FLOYD_STEINBERG, 2);

  // Save image to output file
  img.WriteImg("output_img/output_11.ppm");
}