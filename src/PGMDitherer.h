//
// Created by @mikhirurg on 25.04.2020.
//

#ifndef COMPUTERGEOMETRY_GRAPHICS_LAB3_CDITHERER_H_
#define COMPUTERGEOMETRY_GRAPHICS_LAB3_CDITHERER_H_

#include <random>
#include "PGMImage.h"

struct SampleBayer {
  double *data_;
  int n_;

  SampleBayer(double *data, int n)
      : data_(data), n_(n) {

  }

  ~SampleBayer() {
    delete[] data_;
  }

  void operator+=(double a) {
    for (int i = 0; i < n_; i++) {
      for (int j = 0; j < n_; j++) {
        data_[i * n_ + j] += a;
      }
    }
  }

  void operator/=(double a) {
    for (int i = 0; i < n_; i++) {
      for (int j = 0; j < n_; j++) {
        data_[i * n_ + j] /= a;
      }
    }
  }
};

struct ErrorDiffMatrix {
  int *data_;
  int w_, h_;
  int x0_, y0_;
  int del_;

  ErrorDiffMatrix(int *data, int w, int h, int x0, int y0, int del)
      : data_(data), w_(w), h_(h), x0_(x0), y0_(y0), del_(del) {

  }

  ~ErrorDiffMatrix() {
    delete[] data_;
  }

  int Get(int x, int y) const {
    return data_[y * w_ + x];
  }
};

struct GrayCompensationBuffer {
  double *buffer_;
  int w_, h_;

  GrayCompensationBuffer(int w, int h)
      : w_(w), h_(h) {
    buffer_ = new double[w_ * h_];
    for (int i = 0; i < w_ * h_; i++) {
      buffer_[i] = 0.0f;
    }
  }

  ~GrayCompensationBuffer() {
    delete[] buffer_;
  }
  double Get(int x, int y) const {
    return buffer_[y * w_ + x];
  }
  void Add(int x, int y, double val) {
    buffer_[y * w_ + x] += val;
  }
  void Set(int x, int y, double val) {
    buffer_[y * w_ + x] = val;
  }
};

struct ColorCompensationBuffer {
  double *buffer_r_;
  double *buffer_g_;
  double *buffer_b_;
  int w_, h_;

  ColorCompensationBuffer(int w, int h)
      : w_(w), h_(h) {
    buffer_r_ = new double[w_ * h_];
    buffer_g_ = new double[w_ * h_];
    buffer_b_ = new double[w_ * h_];
    for (int i = 0; i < w_ * h_; i++) {
      buffer_r_[i] = 0.0f;
      buffer_g_[i] = 0.0f;
      buffer_b_[i] = 0.0f;
    }
  }

  ~ColorCompensationBuffer() {
    delete[] buffer_r_;
    delete[] buffer_g_;
    delete[] buffer_b_;
  }
  double GetR(int x, int y) const {
    return buffer_r_[y * w_ + x];
  }
  double GetG(int x, int y) const {
    return buffer_g_[y * w_ + x];
  }
  double GetB(int x, int y) const {
    return buffer_b_[y * w_ + x];
  }

  void Add(int x, int y, double val_r, double val_g, double val_b) {
    buffer_r_[y * w_ + x] += val_r;
    buffer_g_[y * w_ + x] += val_g;
    buffer_b_[y * w_ + x] += val_b;
  }

  void Set(int x, int y, double val_r, double val_g, double val_b) {
    buffer_r_[y * w_ + x] = val_r;
    buffer_g_[y * w_ + x] = val_g;
    buffer_b_[y * w_ + x] = val_b;
  }
};

template<class T>
class PGMDitherer {
 public:

  PGMDitherer(PGMImage<T> &img);
  ~PGMDitherer();
  void DoColorBitCorrection(int n);
  void DoOrderedDithering(const SampleBayer &bayer, int n);
  void DoRandomDithering(int n, int seed);
  void DoFloydSteinbergDithering(int n);
  void DoJJNDithering(int n);
  void DoSierraDithering(int n);
  void DoAtkinsonDithering(int n);
  void DoHalftoneDithering(int n);
  void DoErrorDiffDithering(const ErrorDiffMatrix &matrix, int n);

  SampleBayer SAMPLE_BAYER2;
  SampleBayer SAMPLE_BAYER4;
  SampleBayer SAMPLE_BAYER8;
  SampleBayer HALFTONE_ORTHOGONAL;

  ErrorDiffMatrix FLOYD_STEINBERG;
  ErrorDiffMatrix JJN;
  ErrorDiffMatrix ATKINSON;
  ErrorDiffMatrix SIERRA;

  GrayCompensationBuffer gray_buffer;

  ColorCompensationBuffer color_buffer;

 private:
  PGMImage<T> &img_;
  SampleBayer GetBayerMatrix(int log);
  void GenBayerMatrix(int *matrix, int log, int x, int y, int n, int mult, int add);
  void RotateMatrixLeft(int *matrix, int n);
  void RotateMatrixRight(int *matrix, int n);
  void FlipMatrixByHorizontal(int *matrix, int n);
  void FlipMatrixByVertical(int *matrix, int n);
  void TransponeMatrixByMain(int *matrix, int n);
  void TransponeMatrixBySide(int *matrix, int n);
  void ApplyErrorDiffMatrix(const ErrorDiffMatrix &matrix, int x, int y, double err);
  void ApplyErrorDiffMatrix(const ErrorDiffMatrix &matrix, int x, int y, double err_r, double err_g, double err_b);
  double FindNearestPaletteColor(double color_val, int n);
  T ModifyPixelByMap(T pixel, int x, int y, const SampleBayer &bayer, int n);
  T ModifyPixelByRandom(T pixel, int n, int seed);
  std::mt19937 rand;
};

#include "PGMDitherer_impl.h"
#endif //COMPUTERGEOMETRY_GRAPHICS_LAB3_CDITHERER_H_
