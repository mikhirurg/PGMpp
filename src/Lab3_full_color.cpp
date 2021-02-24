//
// Created by @mikhirurg on 28.04.2020.
//
#include <ctime>
#include <iostream>
#include "PGMImage.h"
#include "PGMDitherer.h"

int main(int argc, char *argv[]) {
  try {
    if (argc == 7) {
      std::string fin = argv[1];
      std::string fout = argv[2];
      int grad, d, n_bits;
      double gamma;
      try {
        grad = std::stoi(argv[3]);
        d = std::stoi(argv[4]);
        n_bits = std::stoi(argv[5]);
        gamma = std::stod(argv[6]);
      } catch (std::bad_cast &e_cast) {
        throw PGMImageParamsException();
      }
      if (gamma == 0) {
        gamma = 2.2;
      }
      PGMImage<PGMColorPixel> img = PGMImage<PGMColorPixel>(fin, gamma);
      if (img.GetFileType() != P6) {
        throw PGMImageFileFormatException();
      }
      if (grad == 1) {
        img.FillWithGradient();
      }
      if (d < 0 || d > 7) {
        throw PGMImageFileFormatException();
      }
      if (n_bits < 1 || n_bits > 8) {
        throw PGMImageFileFormatException();
      }
      PGMDitherer<PGMColorPixel> ditherer = PGMDitherer<PGMColorPixel>(img);
      switch (d) {
        case 0: {
          ditherer.DoColorBitCorrection(n_bits);
          break;
        }
        case 1: {
          ditherer.DoOrderedDithering(ditherer.SAMPLE_BAYER8, n_bits);
          break;
        }
        case 2: {
          ditherer.DoRandomDithering(n_bits, time(0));
          break;
        }
        case 3: {
          ditherer.DoFloydSteinbergDithering(n_bits);
          break;
        }
        case 4: {
          ditherer.DoJJNDithering(n_bits);
          break;
        }
        case 5: {
          ditherer.DoSierraDithering(n_bits);
          break;
        }
        case 6: {
          ditherer.DoAtkinsonDithering(n_bits);
          break;
        }
        case 7: {
          ditherer.DoHalftoneDithering(n_bits);
          break;
        }
        default: {
          // Never happen
        }
      }
      img.WriteImg(fout);
    } else {
      throw PGMImageParamsException();
    }
  } catch (PGMImageException e) {
    std::cerr << e.getErr() << std::endl;
    return 1;
  }
  return 0;
}
