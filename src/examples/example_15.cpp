//
// Created by @mikhirurug on 19.03.2023.
//
#include "../PGMPixel.h"
#include "../PGMImage_impl.h"
#include "../PGMSpace_impl.h"

int main(int argc, char *argv[]) {

  // Open PGM image "lena.ppm"
  PGMImage<PGMColorPixel> img = PGMImage<PGMColorPixel>("pgm_img/lena.ppm", 2.2);

  // Create the tmp PGM image
  PGMImage<PGMColorPixel> tmp = PGMImage<PGMColorPixel>(img.GetWidth(),
                                                        img.GetHeight(),
                                                        img.GetMaxVal(),
                                                        img.GetFileType(),
                                                        img.GetGamma());

  // Create the HSL color space object
  HSL color_space = HSL();

  // Apply color transformation to each pixel of the input image and write them to the output image
  for (int y = 0; y < img.GetHeight(); y++) {
    for (int x = 0; x < img.GetWidth(); x++) {
      tmp.PutPixel(x, y, color_space.FromRGB(img.GetPixel(x, y)));
    }
  }

  // Save image to output file
  tmp.WriteImg("output_img/output_15.ppm");
}