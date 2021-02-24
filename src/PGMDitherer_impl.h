//
// Created by @mikhirurg on 09.08.2020.
//

#ifndef PGMPP_SRC_PGMDITHERER_IMPL_H_
#define PGMPP_SRC_PGMDITHERER_IMPL_H_

#include "PGMDitherer.h"

template<>
PGMMonoPixel PGMDitherer<PGMMonoPixel>::ModifyPixelByMap(PGMMonoPixel pixel, int x, int y, const SampleBayer &bayer, int n) {
  double resizer = double(img_.GetMaxVal()) / n;
  return img_.Clamp(FindNearestPaletteColor(
      double(pixel.val) + int(resizer * bayer.data_[(y % bayer.n_) * bayer.n_ + x % bayer.n_]),
      n));
}

template<>
PGMColorPixel PGMDitherer<PGMColorPixel>::ModifyPixelByMap(PGMColorPixel pixel, int x, int y, const SampleBayer &bayer, int n) {
  double resizer = double(img_.GetMaxVal()) / n;
  return {
      (uchar) FindNearestPaletteColor(pixel.r + int(resizer * bayer.data_[(y % bayer.n_) * bayer.n_ + x % bayer.n_]),
                                      n),
      (uchar) FindNearestPaletteColor(pixel.g + int(resizer * bayer.data_[(y % bayer.n_) * bayer.n_ + x % bayer.n_]),
                                      n),
      (uchar) FindNearestPaletteColor(pixel.b + int(resizer * bayer.data_[(y % bayer.n_) * bayer.n_ + x % bayer.n_]),
                                      n)};
}

template<>
PGMColorPixel PGMDitherer<PGMColorPixel>::ModifyPixelByRandom(PGMColorPixel pixel, int n, int seed) {
  double resizer = double(img_.GetMaxVal()) / n;
  std::uniform_real_distribution<> urd(0 + DBL_EPSILON, 1 + DBL_EPSILON);
  return {(uchar) FindNearestPaletteColor(pixel.r + int(resizer * urd(rand)), n),
          (uchar) FindNearestPaletteColor(pixel.g + int(resizer * urd(rand)), n),
          (uchar) FindNearestPaletteColor(pixel.b + int(resizer * urd(rand)), n)};
}

template<>
PGMMonoPixel PGMDitherer<PGMMonoPixel>::ModifyPixelByRandom(PGMMonoPixel color_val, int n, int seed) {
  double resizer = double(img_.GetMaxVal()) / n;
  std::uniform_real_distribution<> urd(-0.5 + DBL_EPSILON, 0.5 + DBL_EPSILON);
  return {(uchar) FindNearestPaletteColor(color_val.val + int(resizer * urd(rand)), n)};
}

template<class T>
PGMDitherer<T>::PGMDitherer(PGMImage<T> &img)
    : img_(img),

      SAMPLE_BAYER2{
          new double[4]{
              0.0, 2.0,
              3.0, 1.0}, (int) 2
      },

      SAMPLE_BAYER4{
          new double[16]{0.0, 8.0, 2.0, 10.0,
                         12.0, 4.0, 14.0, 6.0,
                         3.0, 11.0, 1.0, 9.0,
                         15.0, 7.0, 13.0, 5.0}, (int) 4
      },

      SAMPLE_BAYER8{
          new double[64]{0.0, 48.0, 12.0, 60.0, 3.0, 51.0, 15.0, 63.0,
                         32.0, 16.0, 44.0, 28.0, 35.0, 19.0, 47.0, 31.0,
                         8.0, 56.0, 4.0, 52.0, 11.0, 59.0, 7.0, 55.0,
                         40.0, 24.0, 36.0, 20.0, 43.0, 27.0, 39.0, 23.0,
                         2.0, 50.0, 14.0, 62.0, 1.0, 49.0, 13.0, 61.0,
                         34.0, 18.0, 46.0, 30.0, 33.0, 17.0, 45.0, 29.0,
                         10.0, 58.0, 6.0, 54.0, 9.0, 57.0, 5.0, 53.0,
                         42.0, 26.0, 38.0, 22.0, 41.0, 25.0, 37.0, 21.0}, (int) 8
      },

      HALFTONE_ORTHOGONAL{
          new double[16]{7.0, 13.0, 11.0, 4.0,
                         12.0, 16.0, 14.0, 8.0,
                         10.0, 15.0, 6.0, 2.0,
                         5.0, 9.0, 3.0, 1.0}, (int) 4
      },

      FLOYD_STEINBERG{
          new int[6]{-1, 0, 7,
                     3, 5, 1},
          (int) 3, (int) 2, (int) 1, (int) 0, (int) 16
      },

      JJN{
          new int[15]{-1, -1, 0, 7, 5,
                      3, 4, 7, 5, 3,
                      1, 3, 5, 3, 1},
          (int) 5, (int) 3, (int) 2, (int) 0, (int) 48
      },

      ATKINSON{
          new int[12]{-1, 0, 1, 1,
                      1, 1, 1, -1,
                      -1, 1, -1, -1},
          (int) 4, (int) 3, (int) 1, (int) 0, (int) 8
      },
      SIERRA{
          new int[15]{-1, -1, 0, 5, 3,
                      2, 4, 5, 4, 2,
                      -1, 2, 3, 2, -1},
          (int) 5, (int) 3, (int) 2, (int) 0, (int) 32
      },
      gray_buffer(img.GetWidth(), img.GetHeight()),
      color_buffer(img.GetWidth(), img.GetHeight()) {
  SAMPLE_BAYER2 /= (SAMPLE_BAYER2.n_ * SAMPLE_BAYER2.n_);
  SAMPLE_BAYER4 /= (SAMPLE_BAYER4.n_ * SAMPLE_BAYER4.n_);
  SAMPLE_BAYER8 /= (SAMPLE_BAYER8.n_ * SAMPLE_BAYER8.n_);
  HALFTONE_ORTHOGONAL /= (HALFTONE_ORTHOGONAL.n_ * HALFTONE_ORTHOGONAL.n_);

  SAMPLE_BAYER2 += -0.5;
  SAMPLE_BAYER4 += -0.5;
  SAMPLE_BAYER8 += -0.5;

  HALFTONE_ORTHOGONAL += -0.5;

}
template<class T>
PGMDitherer<T>::~PGMDitherer() {

}

template<class T>
void PGMDitherer<T>::ApplyErrorDiffMatrix(const ErrorDiffMatrix &matrix, int x, int y, double err) {
  for (int i = 0; i < matrix.h_; i++) {
    for (int j = 0; j < matrix.w_; j++) {
      int x1 = x + j - matrix.x0_;
      int y1 = y + i - matrix.y0_;
      if (matrix.Get(j, i) > 0 && x1 >= 0 && x1 < img_.GetWidth() && y1 >= 0 && y1 < img_.GetHeight()) {
        gray_buffer.Add(x1, y1, float(err * matrix.Get(j, i) / double(matrix.del_)));
      }
    }
  }
}

template<>
void PGMDitherer<PGMColorPixel>::ApplyErrorDiffMatrix(const ErrorDiffMatrix &matrix,
                                                      int x,
                                                      int y,
                                                      double err_r,
                                                      double err_g,
                                                      double err_b) {

  for (int i = 0; i < matrix.h_; i++) {
    for (int j = 0; j < matrix.w_; j++) {
      int x1 = x + j - matrix.x0_;
      int y1 = y + i - matrix.y0_;
      if (matrix.Get(j, i) > 0 && x1 >= 0 && x1 < img_.GetWidth() && y1 >= 0 && y1 < img_.GetHeight()) {
        color_buffer.Add(x1,
                         y1,
                         double(err_r * matrix.Get(j, i) / double(matrix.del_)),
                         double(err_g * matrix.Get(j, i) / double(matrix.del_)),
                         double(err_b * matrix.Get(j, i) / double(matrix.del_)));
      }
    }
  }
}

template<>
void PGMDitherer<PGMMonoPixel>::DoColorBitCorrection(int n) {
  for (int y = 0; y < img_.GetHeight(); y++) {
    for (int x = 0; x < img_.GetWidth(); x++) {
      img_.PutPixelWithGamma(x, y, FindNearestPaletteColor(img_.GetLinearVal(x, y), n));
    }
  }
}

template<>
void PGMDitherer<PGMColorPixel>::DoColorBitCorrection(int n) {
  for (int y = 0; y < img_.GetHeight(); y++) {
    for (int x = 0; x < img_.GetWidth(); x++) {
      double pixel_r = img_.GetLinearRVal(x, y);
      double pixel_g = img_.GetLinearGVal(x, y);
      double pixel_b = img_.GetLinearBVal(x, y);
      img_.PutPixelWithGamma(x, y, FindNearestPaletteColor(pixel_r, n),
                             FindNearestPaletteColor(pixel_g, n),
                             FindNearestPaletteColor(pixel_b, n));
    }
  }
}

template<>
void PGMDitherer<PGMMonoPixel>::DoOrderedDithering(const SampleBayer &bayer, int n) {
  for (int y = 0; y < img_.GetHeight(); y++) {
    for (int x = 0; x < img_.GetWidth(); x++) {
      PGMMonoPixel p = PGMMonoPixel{uchar(round(img_.GetLinearVal(x, y)))};
      PGMMonoPixel pixel = ModifyPixelByMap(p, x, y, bayer, n);
      img_.PutPixelWithGamma(x, y, pixel.val);
    }
  }
}

template<>
void PGMDitherer<PGMColorPixel>::DoOrderedDithering(const SampleBayer &bayer, int n) {
  for (int y = 0; y < img_.GetHeight(); y++) {
    for (int x = 0; x < img_.GetWidth(); x++) {
      PGMColorPixel p = PGMColorPixel{uchar(round(img_.GetLinearRVal(x, y))),
                                      uchar(round(img_.GetLinearGVal(x, y))),
                                      uchar(round(img_.GetLinearBVal(x, y)))};
      PGMColorPixel pixel = ModifyPixelByMap(p, x, y, bayer, n);
      img_.PutPixelWithGamma(x, y, pixel.r, pixel.g, pixel.b);
    }
  }
}

template<>
void PGMDitherer<PGMMonoPixel>::DoRandomDithering(int n, int seed) {
  rand.seed(seed);
  for (int y = 0; y < img_.GetHeight(); y++) {
    for (int x = 0; x < img_.GetWidth(); x++) {
      PGMMonoPixel p = PGMMonoPixel{uchar(round(img_.GetLinearVal(x, y)))};
      img_.PutPixelWithGamma(x, y, ModifyPixelByRandom(p, n, seed).val);
    }
  }
}

template<>
void PGMDitherer<PGMColorPixel>::DoRandomDithering(int n, int seed) {
  rand.seed(seed);
  for (int y = 0; y < img_.GetHeight(); y++) {
    for (int x = 0; x < img_.GetWidth(); x++) {
      PGMColorPixel p = PGMColorPixel{uchar(round(img_.GetLinearRVal(x, y))),
                                      uchar(round(img_.GetLinearGVal(x, y))),
                                      uchar(round(img_.GetLinearBVal(x, y)))};
      PGMColorPixel pixel = ModifyPixelByRandom(p, n, seed);
      img_.PutPixelWithGamma(x, y, pixel.r, pixel.g, pixel.b);
    }
  }
}

template<>
void PGMDitherer<PGMMonoPixel>::DoFloydSteinbergDithering(int n) {
  for (int y = 0; y < img_.GetHeight(); y++) {
    for (int x = 0; x < img_.GetWidth(); x++) {
      double old_pixel = img_.GetLinearVal(x, y);
      double new_pixel = FindNearestPaletteColor((old_pixel + gray_buffer.Get(x, y)), n);
      double quant_err = old_pixel + gray_buffer.Get(x, y) - new_pixel;

      img_.PutPixelWithGamma(x, y, new_pixel);

      if (x + 1 < img_.GetWidth()) {
        gray_buffer.Add(x + 1, y, quant_err * (7.0 / 16.0));
      }
      if (x - 1 >= 0 && y + 1 < img_.GetHeight()) {
        gray_buffer.Add(x - 1, y + 1, quant_err * (3.0 / 16.0));
      }
      if (y + 1 < img_.GetHeight()) {
        gray_buffer.Add(x, y + 1, quant_err * (5.0 / 16.0));
      }
      if (y + 1 < img_.GetHeight() && x + 1 < img_.GetWidth()) {
        gray_buffer.Add(x + 1, y + 1, quant_err * (1.0 / 16.0));
      }
    }
  }
}

template<>
void PGMDitherer<PGMColorPixel>::DoFloydSteinbergDithering(int n) {
  for (int y = 0; y < img_.GetHeight(); y++) {
    for (int x = 0; x < img_.GetWidth(); x++) {
      double old_pixel_r = img_.GetLinearRVal(x, y);
      double old_pixel_g = img_.GetLinearGVal(x, y);
      double old_pixel_b = img_.GetLinearBVal(x, y);
      double new_pixel_r = FindNearestPaletteColor(round(double(old_pixel_r) + color_buffer.GetR(x, y)), n);
      double new_pixel_g = FindNearestPaletteColor(round(double(old_pixel_g) + color_buffer.GetG(x, y)), n);
      double new_pixel_b = FindNearestPaletteColor(round(double(old_pixel_b) + color_buffer.GetB(x, y)), n);

      img_.PutPixelWithGamma(x, y, new_pixel_r, new_pixel_g, new_pixel_b);
      double quant_err_r = old_pixel_r + color_buffer.GetR(x, y) - new_pixel_r;
      double quant_err_g = old_pixel_g + color_buffer.GetG(x, y) - new_pixel_g;
      double quant_err_b = old_pixel_b + color_buffer.GetB(x, y) - new_pixel_b;
      if (x + 1 < img_.GetWidth()) {
        color_buffer.Add(x + 1,
                         y,
                         double(quant_err_r * 7.0 / 16.0),
                         double(quant_err_g * 7.0 / 16.0),
                         double(quant_err_b * 7.0 / 16.0));
      }
      if (x - 1 >= 0 && y + 1 < img_.GetHeight()) {
        color_buffer.Add(x - 1,
                         y + 1,
                         double(quant_err_r * 3.0 / 16.0),
                         double(quant_err_r * 3.0 / 16.0),
                         double(quant_err_r * 3.0 / 16.0));
      }
      if (y + 1 < img_.GetHeight()) {
        color_buffer.Add(x,
                         y + 1,
                         double(quant_err_r * 5.0 / 16.0),
                         double(quant_err_r * 5.0 / 16.0),
                         double(quant_err_r * 5.0 / 16.0));
      }
      if (y + 1 < img_.GetHeight() && x + 1 < img_.GetWidth()) {
        color_buffer.Add(x + 1,
                         y + 1,
                         double((quant_err_r * 1.0) / 16),
                         double((quant_err_r * 1.0) / 16),
                         double((quant_err_r * 1.0) / 16));
      }
    }
  }
}

template<>
void PGMDitherer<PGMColorPixel>::DoJJNDithering(int n) {
  for (int y = 0; y < img_.GetHeight(); y++) {
    for (int x = 0; x < img_.GetWidth(); x++) {
      double old_pixel_r = img_.GetLinearRVal(x, y);
      double old_pixel_g = img_.GetLinearGVal(x, y);
      double old_pixel_b = img_.GetLinearBVal(x, y);
      double new_pixel_r = FindNearestPaletteColor(old_pixel_r + color_buffer.GetR(x, y), n);
      double new_pixel_g = FindNearestPaletteColor(old_pixel_g + color_buffer.GetG(x, y), n);
      double new_pixel_b = FindNearestPaletteColor(old_pixel_b + color_buffer.GetB(x, y), n);
      img_.PutPixelWithGamma(x, y, new_pixel_r, new_pixel_g, new_pixel_b);
      double quant_err_r = old_pixel_r + color_buffer.GetR(x, y) - new_pixel_r;
      double quant_err_g = old_pixel_g + color_buffer.GetG(x, y) - new_pixel_g;
      double quant_err_b = old_pixel_b + color_buffer.GetB(x, y) - new_pixel_b;

      double del = 48.0;

      if (x + 1 < img_.GetWidth()) {
        color_buffer.Add(x + 1,
                         y,
                         double(quant_err_r * 7.0 / del),
                         double(quant_err_g * 7.0 / del),
                         double(quant_err_b * 7.0 / del));
      }
      if (x + 2 < img_.GetWidth()) {
        color_buffer.Add(x + 2,
                         y,
                         double(quant_err_r * 5.0 / del),
                         double(quant_err_g * 5.0 / del),
                         double(quant_err_b * 5.0 / del));
      }
      if ((x - 2 >= 0) && (y + 1 < img_.GetHeight())) {
        color_buffer.Add(x - 2,
                         y + 1,
                         double(quant_err_r * 3.0 / del),
                         double(quant_err_g * 3.0 / del),
                         double(quant_err_b * 3.0 / del));
      }
      if ((x - 1 >= 0) && (y + 1 < img_.GetHeight())) {
        color_buffer.Add(x - 1,
                         y + 1,
                         double(quant_err_r * 5.0 / del),
                         double(quant_err_g * 5.0 / del),
                         double(quant_err_b * 5.0 / del));
      }
      if (y + 1 < img_.GetHeight()) {
        color_buffer.Add(x,
                         y + 1,
                         double(quant_err_r * 7.0 / del),
                         double(quant_err_g * 7.0 / del),
                         double(quant_err_b * 7.0 / del));
      }

      if ((x + 1 < img_.GetWidth()) && (y + 1 < img_.GetHeight())) {
        color_buffer.Add(x + 1,
                         y + 1,
                         double(quant_err_r * 5.0 / del),
                         double(quant_err_g * 5.0 / del),
                         double(quant_err_b * 5.0 / del));
      }

      if ((x + 2 < img_.GetWidth()) && (y + 1 < img_.GetHeight())) {
        color_buffer.Add(x + 2,
                         y + 1,
                         double(quant_err_r * 3.0 / del),
                         double(quant_err_g * 3.0 / del),
                         double(quant_err_b * 3.0 / del));
      }
      if ((x - 2 >= 0) && (y + 2 < img_.GetHeight())) {
        color_buffer.Add(x - 2,
                         y + 2,
                         double((quant_err_r) / del),
                         double((quant_err_g) / del),
                         double((quant_err_b) / del));
      }
      if ((x - 1 >= 0) && (y + 2 < img_.GetHeight())) {
        color_buffer.Add(x - 1,
                         y + 2,
                         double(quant_err_r * 3.0 / del),
                         double(quant_err_g * 3.0 / del),
                         double(quant_err_b * 3.0 / del));
      }
      if (y + 2 < img_.GetHeight()) {
        color_buffer.Add(x,
                         y + 2,
                         double(quant_err_r * 5.0 / del),
                         double(quant_err_g * 5.0 / del),
                         double(quant_err_b * 5.0 / del));
      }
      if ((x + 1 < img_.GetWidth()) && (y + 2 < img_.GetHeight())) {
        color_buffer.Add(x + 1,
                         y + 2,
                         double(quant_err_r * 3.0 / del),
                         double(quant_err_g * 3.0 / del),
                         double(quant_err_b * 3.0 / del));
      }
      if ((x + 2 < img_.GetWidth()) && (y + 2 < img_.GetHeight())) {
        color_buffer.Add(x + 2, y + 2, double(quant_err_r / del), double(quant_err_g / del), double(quant_err_b / del));
      }
    }
  }
}

template<>
void PGMDitherer<PGMMonoPixel>::DoJJNDithering(int n) {
  for (int y = 0; y < img_.GetHeight(); y++) {
    for (int x = 0; x < img_.GetWidth(); x++) {
      double old_pixel = img_.GetLinearVal(x, y);
      double new_pixel = FindNearestPaletteColor(old_pixel + gray_buffer.Get(x, y), n);

      img_.PutPixelWithGamma(x, y, new_pixel);
      double quant_err = old_pixel + gray_buffer.Get(x, y) - new_pixel;

      if (x + 1 < img_.GetWidth()) {
        gray_buffer.Add(x + 1, y, (quant_err * 7.0) / 48);
      }

      if (x + 2 < img_.GetWidth()) {
        gray_buffer.Add(x + 2, y, (quant_err * 5.0) / 48);
      }

      if (x - 2 >= 0 && y + 1 < img_.GetHeight()) {
        gray_buffer.Add(x - 2, y + 1, (quant_err * 3.0) / 48);
      }

      if (x - 1 >= 0 && y + 1 < img_.GetHeight()) {
        gray_buffer.Add(x - 1, y + 1, (quant_err * 5.0) / 48);
      }

      if (y + 1 < img_.GetHeight()) {
        gray_buffer.Add(x, y + 1, (quant_err * 7.0) / 48);
      }

      if (x + 1 < img_.GetWidth() && y + 1 < img_.GetHeight()) {
        gray_buffer.Add(x + 1, y + 1, (quant_err * 5.0) / 48);
      }

      if (x + 2 < img_.GetWidth() && y + 1 < img_.GetHeight()) {
        gray_buffer.Add(x + 2, y + 1, (quant_err * 3.0) / 48);
      }

      if (x - 2 >= 0 && y + 2 < img_.GetHeight()) {
        gray_buffer.Add(x - 2, y + 2, (quant_err) / 48.0);
      }

      if (x - 1 >= 0 && y + 2 < img_.GetHeight()) {
        gray_buffer.Add(x - 1, y + 2, (quant_err * 3.0) / 48);
      }

      if (y + 2 < img_.GetHeight()) {
        gray_buffer.Add(x, y + 2, (quant_err * 5.0) / 48);
      }

      if (x + 1 < img_.GetWidth() && y + 2 < img_.GetHeight()) {
        gray_buffer.Add(x + 1, y + 2, (quant_err * 3.0) / 48);
      }

      if (x + 2 < img_.GetWidth() && y + 2 < img_.GetHeight()) {
        gray_buffer.Add(x + 2, y + 2, (quant_err) / 48.0);
      }
    }
  }
}

template<>
void PGMDitherer<PGMMonoPixel>::DoAtkinsonDithering(int n) {
  for (int y = 0; y < img_.GetHeight(); y++) {
    for (int x = 0; x < img_.GetWidth(); x++) {
      double old_pixel = img_.GetLinearVal(x, y);
      double new_pixel = FindNearestPaletteColor(old_pixel + gray_buffer.Get(x, y), n);
      double quant_err = old_pixel + gray_buffer.Get(x, y) - new_pixel;
      img_.PutPixelWithGamma(x, y, new_pixel);

      if (x + 1 < img_.GetWidth()) {
        gray_buffer.Add(x + 1, y, (quant_err) / 8.0);
      }

      if (x + 2 < img_.GetWidth()) {
        gray_buffer.Add(x + 2, y, (quant_err) / 8.0);
      }

      if (x - 1 >= 0 && y + 1 < img_.GetHeight()) {
        gray_buffer.Add(x - 1, y + 1, (quant_err) / 8.0);
      }

      if (y + 1 < img_.GetHeight()) {
        gray_buffer.Add(x, y + 1, (quant_err) / 8.0);
      }

      if (x + 1 < img_.GetWidth() && y + 1 < img_.GetHeight()) {
        gray_buffer.Add(x + 1, y + 1, (quant_err) / 8.0);
      }

      if (y + 2 < img_.GetHeight()) {
        gray_buffer.Add(x, y + 2, (quant_err) / 8.0);
      }
    }
  }
}

template<>
void PGMDitherer<PGMColorPixel>::DoAtkinsonDithering(int n) {
  for (int y = 0; y < img_.GetHeight(); y++) {
    for (int x = 0; x < img_.GetWidth(); x++) {
      double old_pixel_r = img_.GetLinearRVal(x, y);
      double old_pixel_g = img_.GetLinearGVal(x, y);
      double old_pixel_b = img_.GetLinearBVal(x, y);

      double new_pixel_r = FindNearestPaletteColor(old_pixel_r + color_buffer.GetR(x, y), n);
      double new_pixel_g = FindNearestPaletteColor(old_pixel_g + color_buffer.GetG(x, y), n);
      double new_pixel_b = FindNearestPaletteColor(old_pixel_b + color_buffer.GetB(x, y), n);

      double quant_err_r = old_pixel_r + color_buffer.GetR(x, y) - new_pixel_r;
      double quant_err_g = old_pixel_g + color_buffer.GetG(x, y) - new_pixel_g;
      double quant_err_b = old_pixel_b + color_buffer.GetB(x, y) - new_pixel_b;

      img_.PutPixelWithGamma(x, y, new_pixel_r, new_pixel_g, new_pixel_b);
      if (x + 1 < img_.GetWidth()) {
        color_buffer.Add(x + 1,
                         y,
                         (quant_err_r) / 8.0,
                         (quant_err_g) / 8.0,
                         (quant_err_b) / 8.0);
      }

      if (x + 2 < img_.GetWidth()) {
        color_buffer.Add(x + 2,
                         y,
                         (quant_err_r) / 8.0,
                         (quant_err_g) / 8.0,
                         (quant_err_b) / 8.0);
      }

      if (x - 1 >= 0 && y + 1 < img_.GetHeight()) {
        color_buffer.Add(x - 1,
                         y + 1,
                         (quant_err_r) / 8.0,
                         (quant_err_g) / 8.0,
                         (quant_err_r) / 8.0);
      }

      if (y + 1 < img_.GetHeight()) {
        color_buffer.Add(x,
                         y + 1,
                         (quant_err_r) / 8.0,
                         (quant_err_g) / 8.0,
                         (quant_err_b) / 8.0);
      }

      if (x + 1 < img_.GetWidth() && y + 1 < img_.GetHeight()) {
        color_buffer.Add(x + 1,
                         y + 1,
                         (quant_err_r) / 8.0,
                         (quant_err_g) / 8.0,
                         (quant_err_b) / 8.0);
      }

      if (y + 2 < img_.GetHeight()) {
        color_buffer.Add(x, y + 2, (quant_err_r) / 8.0, (quant_err_g) / 8.0, (quant_err_b) / 8.0);
      }
    }
  }
}

template<>
void PGMDitherer<PGMMonoPixel>::DoSierraDithering(int n) {
  for (int y = 0; y < img_.GetHeight(); y++) {
    for (int x = 0; x < img_.GetWidth(); x++) {
      double old_pixel = img_.GetLinearVal(x, y);
      double new_pixel = FindNearestPaletteColor(old_pixel + gray_buffer.Get(x, y), n);
      img_.PutPixelWithGamma(x, y, new_pixel);
      double quant_err = old_pixel + gray_buffer.Get(x, y) - new_pixel;
      if (x + 1 < img_.GetWidth()) {
        gray_buffer.Add(x + 1, y, (quant_err * 5.0) / 32);
      }

      if (x + 2 < img_.GetWidth()) {
        gray_buffer.Add(x + 2, y, (quant_err * 3.0) / 32);
      }

      if (x - 2 >= 0 && y + 1 < img_.GetHeight()) {
        gray_buffer.Add(x - 2, y + 1, (quant_err * 2.0) / 32);
      }

      if (x - 1 >= 0 && y + 1 < img_.GetHeight()) {
        gray_buffer.Add(x - 1, y + 1, (quant_err * 4.0) / 32);
      }

      if (y + 1 < img_.GetHeight()) {
        gray_buffer.Add(x, y + 1, (quant_err * 5.0) / 32);
      }

      if (x + 1 < img_.GetWidth() && y + 1 < img_.GetHeight()) {
        gray_buffer.Add(x + 1, y + 1, (quant_err * 4.0) / 32);
      }

      if (x + 2 < img_.GetWidth() && y + 1 < img_.GetHeight()) {
        gray_buffer.Add(x + 2, y + 1, (quant_err * 2.0) / 32);
      }

      if (x - 1 >= 0 && y + 2 < img_.GetHeight()) {
        gray_buffer.Add(x - 1, y + 2, (quant_err * 2.0) / 32);
      }

      if (y + 2 < img_.GetHeight()) {
        gray_buffer.Add(x, y + 2, (quant_err * 3.0) / 32);
      }

      if (x + 1 < img_.GetWidth() && y + 2 < img_.GetHeight()) {
        gray_buffer.Add(x + 1, y + 2, (quant_err * 2.0) / 32);
      }
    }
  }
}

template<>
void PGMDitherer<PGMColorPixel>::DoSierraDithering(int n) {
  for (int y = 0; y < img_.GetHeight(); y++) {
    for (int x = 0; x < img_.GetWidth(); x++) {
      double old_pixel_r = img_.GetLinearRVal(x, y);
      double old_pixel_g = img_.GetLinearGVal(x, y);
      double old_pixel_b = img_.GetLinearBVal(x, y);

      double new_pixel_r = FindNearestPaletteColor(old_pixel_r + color_buffer.GetR(x, y), n);
      double new_pixel_g = FindNearestPaletteColor(old_pixel_g + color_buffer.GetG(x, y), n);
      double new_pixel_b = FindNearestPaletteColor(old_pixel_b + color_buffer.GetB(x, y), n);

      img_.PutPixelWithGamma(x, y, new_pixel_r, new_pixel_g, new_pixel_b);
      double quant_err_r = old_pixel_r + color_buffer.GetR(x, y) - new_pixel_r;
      double quant_err_g = old_pixel_g + color_buffer.GetG(x, y) - new_pixel_g;
      double quant_err_b = old_pixel_b + color_buffer.GetB(x, y) - new_pixel_b;

      if (x + 1 < img_.GetWidth()) {
        color_buffer.Add(x + 1,
                         y,
                         (quant_err_r * 5.0) / 32,
                         (quant_err_g * 5.0) / 32,
                         (quant_err_b * 5.0) / 32);
      }

      if (x + 2 < img_.GetWidth()) {
        color_buffer.Add(x + 2,
                         y,
                         (quant_err_r * 3.0) / 32,
                         (quant_err_g * 3.0) / 32,
                         (quant_err_b * 3.0) / 32);
      }

      if (x - 2 >= 0 && y + 1 < img_.GetHeight()) {
        color_buffer.Add(x - 2,
                         y + 1,
                         (quant_err_r * 2.0) / 32,
                         (quant_err_g * 2.0) / 32,
                         (quant_err_b * 2.0) / 32);
      }

      if (x - 1 >= 0 && y + 1 < img_.GetHeight()) {
        color_buffer.Add(x - 1,
                         y + 1,
                         (quant_err_r * 4.0) / 32,
                         (quant_err_g * 4.0) / 32,
                         (quant_err_b * 4.0) / 32);
      }

      if (y + 1 < img_.GetHeight()) {
        color_buffer.Add(x,
                         y + 1,
                         (quant_err_r * 5.0) / 32,
                         (quant_err_g * 5.0) / 32,
                         (quant_err_b * 5.0) / 32);
      }

      if (x + 1 < img_.GetWidth() && y + 1 < img_.GetHeight()) {
        color_buffer.Add(x + 1,
                         y + 1,
                         (quant_err_r * 4.0) / 32,
                         (quant_err_g * 4.0) / 32,
                         (quant_err_b * 4.0) / 32);
      }

      if (x + 2 < img_.GetWidth() && y + 1 < img_.GetHeight()) {
        color_buffer.Add(x + 2,
                         y + 1,
                         (quant_err_r * 2.0) / 32,
                         (quant_err_g * 2.0) / 32,
                         (quant_err_b * 2.0) / 32);
      }

      if (x - 1 >= 0 && y + 2 < img_.GetHeight()) {
        color_buffer.Add(x - 1,
                         y + 2,
                         (quant_err_r * 2.0) / 32,
                         (quant_err_g * 2.0) / 32,
                         (quant_err_b * 2.0) / 32);
      }

      if (y + 2 < img_.GetHeight()) {
        color_buffer.Add(x,
                         y + 2,
                         (quant_err_r * 3.0) / 32,
                         (quant_err_g * 3.0) / 32,
                         (quant_err_b * 3.0) / 32);
      }

      if (x + 1 < img_.GetWidth() && y + 2 < img_.GetHeight()) {
        color_buffer.Add(x + 1,
                         y + 2,
                         (quant_err_r * 2.0) / 32,
                         (quant_err_g * 2.0) / 32,
                         (quant_err_b * 2.0) / 32);
      }
    }
  }
}

template<>
void PGMDitherer<PGMMonoPixel>::DoHalftoneDithering(int n) {
  for (int y = 0; y < img_.GetHeight(); y++) {
    for (int x = 0; x < img_.GetWidth(); x++) {
      PGMMonoPixel p = img_.Clamp(img_.GetLinearVal(x, y));
      PGMMonoPixel pixel = ModifyPixelByMap(p, x, y, HALFTONE_ORTHOGONAL, n);
      img_.PutPixelWithGamma(x, y, pixel.val);
    }
  }
}

template<>
void PGMDitherer<PGMColorPixel>::DoHalftoneDithering(int n) {
  for (int y = 0; y < img_.GetHeight(); y++) {
    for (int x = 0; x < img_.GetWidth(); x++) {
      PGMColorPixel p = PGMColorPixel{uchar(round(img_.GetLinearRVal(x, y))),
                                      uchar(round(img_.GetLinearGVal(x, y))),
                                      uchar(round(img_.GetLinearBVal(x, y)))};
      PGMColorPixel pixel = ModifyPixelByMap(p, x, y, HALFTONE_ORTHOGONAL, n);
      img_.PutPixelWithGamma(x, y, pixel.r, pixel.g, pixel.b);
    }
  }
}

template<>
void PGMDitherer<PGMMonoPixel>::DoErrorDiffDithering(const ErrorDiffMatrix &matrix, int n) {
  for (int y = 0; y < img_.GetHeight(); y++) {
    for (int x = 0; x < img_.GetWidth(); x++) {
      double old_pixel = img_.GetLinearVal(x, y);
      double new_pixel = FindNearestPaletteColor(double(old_pixel) + gray_buffer.Get(x, y), n);
      img_.PutPixelWithGamma(x, y, new_pixel);
      double quant_err = old_pixel - new_pixel;
      ApplyErrorDiffMatrix(matrix, x, y, quant_err);
    }
  }
}

template<>
void PGMDitherer<PGMColorPixel>::DoErrorDiffDithering(const ErrorDiffMatrix &matrix, int n) {
  for (int y = 0; y < img_.GetHeight(); y++) {
    for (int x = 0; x < img_.GetWidth(); x++) {
      double old_pixel_r = img_.GetLinearRVal(x, y);
      double old_pixel_g = img_.GetLinearGVal(x, y);
      double old_pixel_b = img_.GetLinearBVal(x, y);

      double new_pixel_r = FindNearestPaletteColor(double(old_pixel_r) + color_buffer.GetR(x, y), n);
      double new_pixel_g = FindNearestPaletteColor(double(old_pixel_g) + color_buffer.GetG(x, y), n);
      double new_pixel_b = FindNearestPaletteColor(double(old_pixel_b) + color_buffer.GetB(x, y), n);

      img_.PutPixelWithGamma(x, y, new_pixel_r, new_pixel_g, new_pixel_b);
      double quant_err_r = old_pixel_r - new_pixel_r;
      double quant_err_g = old_pixel_g - new_pixel_g;
      double quant_err_b = old_pixel_b - new_pixel_b;

      ApplyErrorDiffMatrix(matrix, x, y, quant_err_r, quant_err_g, quant_err_b);
    }
  }
}

template<class T>
double PGMDitherer<T>::FindNearestPaletteColor(double color_val, int n) {
  color_val = std::max(std::min(255.0, color_val), 0.0);
  int levels = pow(2, n);
  double interval_len = double(img_.GetMaxVal()) / (levels - 1);
  double thresh_ind = round((color_val) / interval_len);
  double out = thresh_ind * interval_len;
  return out;
}

template<class T>
void PGMDitherer<T>::GenBayerMatrix(int *matrix, int log, int x, int y, int n, int mult, int add) {
  int w = (int) pow(2, log) / 2;
  if (log == 1) {
    matrix[y * n + x] = add;
    matrix[y * n + x + 1] = 2 * mult + add;
    matrix[(y + 1) * n + x] = 3 * mult + add;
    matrix[(y + 1) * n + x + 1] = mult + add;
  } else {
    GenBayerMatrix(matrix, log - 1, x, y, n, mult * 4, add * mult);
    GenBayerMatrix(matrix, log - 1, x + w, y, n, mult * 4, add * mult + 2);
    GenBayerMatrix(matrix, log - 1, x, y + w, n, mult * 4, add * mult + 3);
    GenBayerMatrix(matrix, log - 1, x + w, y + w, n, mult * 4, add * mult + 1);
  }
}

template<class T>
SampleBayer PGMDitherer<T>::GetBayerMatrix(int log) {
  SampleBayer bayer{
      new double[(int) pow(2, log) * (int) pow(2, log)],
      (int) pow(2, log)
  };
  GenBayerMatrix(bayer.data_, log, 0, 0, (int) pow(2, log), 1, 0);
  return bayer;
}

template<class T>
void PGMDitherer<T>::FlipMatrixByHorizontal(int *matrix, int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n / 2; j++) {
      std::swap(matrix[i * n + j], matrix[i * n + (n - j - 1)]);
    }
  }
}

template<class T>
void PGMDitherer<T>::FlipMatrixByVertical(int *matrix, int n) {
  for (int i = 0; i < n / 2; i++) {
    for (int j = 0; j < n; j++) {
      std::swap(matrix[i * n + j], matrix[(n - i - 1) * n + j]);
    }
  }
}

template<class T>
void PGMDitherer<T>::RotateMatrixLeft(int *matrix, int n) {
  auto *tmp = new int[n * n];
  int k = 0;
  for (int j = 0; j < n; j++) {
    for (int i = n - 1; i >= 0; i--) {
      tmp[k] = matrix[i * n + j];
      k++;
    }
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      matrix[i * n + j] = tmp[i * n + j];
    }
  }
  delete[] (tmp);
}

template<class T>
void PGMDitherer<T>::RotateMatrixRight(int *matrix, int n) {
  auto *tmp = new int[n * n];
  int k = 0;
  for (int j = n - 1; j >= 0; j--) {
    for (int i = 0; i < n; i++) {
      tmp[k] = matrix[i * n + j];
      k++;
    }
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      matrix[i * n + j] = tmp[i * n + j];
    }
  }
  delete[] (tmp);
}

template<class T>
void PGMDitherer<T>::TransponeMatrixByMain(int *matrix, int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < i; j++) {
      std::swap(matrix[i * n + j], matrix[j * n + i]);
    }
  }
}

template<class T>
void PGMDitherer<T>::TransponeMatrixBySide(int *matrix, int n) {
  for (int i = 0; i <= n / 2; i++) {
    for (int j = n / 2; j >= i; j--) {
      std::swap(matrix[i * n + j], matrix[j * n + i]);
    }
  }
}

#endif //PGMPP_SRC_PGMDITHERER_IMPL_H_
