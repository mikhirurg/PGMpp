//
// Created by @mikhirurg on 11.04.2020.
//

#ifndef COMPUTERGEOMETRY_GRAPHICS_CIMAGE_H
#define COMPUTERGEOMETRY_GRAPHICS_CIMAGE_H
typedef unsigned char uchar;
#include <string>
#include <vector>
#include <cfloat>
#include <algorithm>
#include <cmath>
#include <map>
#include <set>

#include "PGMImage.h"
#include "PGMImageFileOpenException.h"
#include "PGMImageFileDeleteException.h"
#include "PGMImageParamsException.h"
#include "PGMImageMemAllocException.h"
#include "PGMImageFileFormatException.h"
#include "PGMImageFileReadException.h"
#include "PGMPixel.h"

enum FileType {
  P5 = 5,
  P6
};

template<class T>
class PGMImage {
 public:
  explicit PGMImage(const std::string &fname, double gamma);

  PGMImage(const std::string &fname, FileType type, int w, int h, int max_val, double gamma);

  PGMImage(const std::string &fname, FileType type, int w, int h, int max_val, const T *&data, double gamma);

  PGMImage(int w, int h, int max_val, FileType type, double gamma);

  PGMImage(const PGMImage &img);

  PGMImage(const PGMImage<PGMMonoPixel>& img1, const PGMImage<PGMMonoPixel>& img2, const PGMImage<PGMMonoPixel>& img3);

  ~PGMImage();

  void WriteImg(const std::string &fname);

  void WriteImg();

  T GetPixel(int x, int y) const;

  double GetLinearVal(int x, int y) const;

  double GetLinearRVal(int x, int y) const;

  double GetLinearGVal(int x, int y) const;

  double GetLinearBVal(int x, int y) const;

  void PutPixel(int x, int y, T pixel);

  void PutPixelWithGamma(int x, int y, double val);

  void PutPixelWithGamma(int x, int y, double val_r, double val_g, double val_b);

  void CorrectImageWithGamma();

  T *operator[](int i);

  int GetWidth() const;

  int GetHeight() const;

  int GetMaxVal() const;

  FileType GetFileType() const;

  double GetGamma() const;

  void FillWithGradient();

  PGMMonoPixel Clamp(double val);

  PGMColorPixel Clamp(double val_r, double val_g, double val_b);

  PGMImage<PGMMonoPixel> *GetImageByChannel(char channel_name);

 private:
  const double eps = 1e-10;
  const int MAX_HEADER_SIZE = 50;
  std::string fname_;
  FileType type_;
  int w_, h_;
  int max_val_;
  T *data_;
  double gamma_;

  bool FileExists(const char *s);

  double IntPart(double x);

  double FloatPart(double x);

};

#include "PGMImage_impl.h"
#endif //COMPUTERGEOMETRY_GRAPHICS_CIMAGE_H