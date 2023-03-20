//
// Created by @mikhirurg on 09.08.2020.
//

#ifndef PGMPP_SRC_PGMIMAGE_IMPL_H_
#define PGMPP_SRC_PGMIMAGE_IMPL_H_

#include "PGMImage.h"

template<typename T>
PGMImage<T>::PGMImage(const std::string &fname, double gamma)
    : fname_(fname), gamma_(gamma) {
  FILE *f = fopen(fname.c_str(), "rb");
  if (!f) {
    throw PGMImageFileOpenException();
  }
  int i = fscanf(f, "P%i%i%i%i\n", &type_, &w_, &h_, &max_val_);
  if (i != 4 || w_ <= 0 || h_ <= 0 || max_val_ <= 0) {
    fclose(f);
    throw PGMImageParamsException();
  }
  if (type_ != P5 && type_ != P6) {
    fclose(f);
    throw PGMImageFileFormatException();
  }
  try {
    data_ = new T[w_ * h_];
    long long check = fread(data_, sizeof(T), w_ * h_, f);
    if (check != w_ * h_) {
      fclose(f);
      throw PGMImageFileReadException();
    }
  } catch (std::bad_alloc &) {
    throw PGMImageMemAllocException();
  }
  fclose(f);
}

template<typename T>
PGMImage<T>::~PGMImage() {
  delete[](data_);
}

template<typename T>
PGMImage<T>::PGMImage(const std::string &fname, FileType type, int w, int h,
                      int max_val, double gamma)
    : fname_(fname), type_(type), w_(w), h_(h), max_val_(max_val), gamma_(gamma) {
  try {
    data_ = new T[w * h];
  } catch (std::bad_alloc &e) {
    throw PGMImageMemAllocException();
  }
}

template<typename T>
PGMImage<T>::PGMImage(const std::string &fname, FileType type, int w, int h,
                      int max_val, const T *&data, double gamma)
    : fname_(fname), type_(type), w_(w), h_(h), max_val_(max_val), gamma_(gamma) {
  try {
    data_ = new T[w_ * h_];
  } catch (std::bad_alloc &e) {
    throw PGMImageMemAllocException();
  }
  for (int i = 0; i < h_; i++) {
    for (int j = 0; j < w_; j++) {
      this[i][j] = data[i * w_ + j];
    }
  }
}

template<class T>
PGMImage<T>::PGMImage(int w, int h, int max_val, FileType type, double gamma)
    : w_(w), h_(h), max_val_(max_val), type_(type), gamma_(gamma) {
  try {
    data_ = new T[w * h];
    for (int i = 0; i < w * h; i++) {
      data_[i] = {0};
    }
  } catch (std::bad_alloc &e) {
    throw PGMImageMemAllocException();
  }
}

template<>
PGMImage<PGMColorPixel>::PGMImage(const PGMImage<PGMMonoPixel>& img1, const PGMImage<PGMMonoPixel>& img2, const PGMImage<PGMMonoPixel>& img3)
    : w_(img1.GetWidth()), h_(img2.GetHeight()), max_val_(img1.GetMaxVal()), type_(P6), gamma_(img1.GetGamma()) {
  try {
    data_ = new PGMColorPixel[w_ * h_];
    for (int y = 0; y < h_; y++) {
      for (int x = 0; x < w_; x++) {
        data_[y * w_ + x] = PGMColorPixel{img1.GetPixel(x, y).val, img2.GetPixel(x, y).val, img3.GetPixel(x, y).val};
      }
    }
  } catch (std::bad_alloc &e) {
    throw PGMImageMemAllocException();
  }
}

template<typename T>
T PGMImage<T>::GetPixel(int x, int y) const {
  if (x >= 0 && y >= 0 && x < w_ && y < h_) {
    return data_[y * w_ + x];
  }
  return {0};
}

template<>
double PGMImage<PGMMonoPixel>::GetLinearVal(int x, int y) const {
  if (gamma_ == 0) {
    double c = GetPixel(x, y).val / double(max_val_);
    if (c <= 0.04045) {
      return c * max_val_ / 12.92;
    } else {
      return pow((c + 0.055) / 1.055, 2.4) * double(max_val_);
    }
  }
  return pow(double(GetPixel(x, y).val) / double(max_val_), gamma_) * double(max_val_);
}

template<>
double PGMImage<PGMColorPixel>::GetLinearRVal(int x, int y) const {
  if (gamma_ == 0) {
    double c = GetPixel(x, y).r / double(max_val_);
    if (c <= 0.04045) {
      return c * max_val_ / 12.92;
    } else {
      return pow((c + 0.055) / 1.055, 2.4) * max_val_;
    }
  }
  return pow(double(GetPixel(x, y).r) / double(max_val_), gamma_) * max_val_;
}

template<>
double PGMImage<PGMColorPixel>::GetLinearGVal(int x, int y) const {
  if (gamma_ == 0) {
    double c = GetPixel(x, y).g / double(max_val_);
    if (c <= 0.04045) {
      return c * max_val_ / 12.92;
    } else {
      return pow((c + 0.055) / 1.055, 2.4) * max_val_;
    }
  }
  return pow(double(GetPixel(x, y).g) / double(max_val_), gamma_) * max_val_;
}

template<>
double PGMImage<PGMColorPixel>::GetLinearBVal(int x, int y) const {
  if (gamma_ == 0) {
    double c = GetPixel(x, y).b / double(max_val_);
    if (c <= 0.04045) {
      return c * max_val_ / 12.92;
    } else {
      return pow((c + 0.055) / 1.055, 2.4) * max_val_;
    }
  }
  return pow(double(GetPixel(x, y).b) / double(max_val_), gamma_) * max_val_;
}

template<class T>
void PGMImage<T>::PutPixel(int x, int y, T pixel) {
  if (x >= 0 && y >= 0 && x < w_ && y < h_) {
    data_[y * w_ + x] = pixel;
  }
}

template<>
void PGMImage<PGMMonoPixel>::PutPixelWithGamma(int x, int y, double val) {
  if (x >= 0 && y >= 0 && x < w_ && y < h_) {
    if (gamma_ == 0) {
      double c = val / double(max_val_);
      if (c <= 0.0031308) {
        data_[y * w_ + x] = Clamp(12.92 * c * max_val_);
      } else {
        data_[y * w_ + x] = Clamp((1.055 * pow(c, 1.0 / 2.4) - 0.055) * max_val_);
      }
    } else {
      data_[y * w_ + x] = Clamp(round(pow(val / max_val_, 1.0 / gamma_) * double(max_val_)));
    }
  }
}

template<>
void PGMImage<PGMColorPixel>::PutPixelWithGamma(int x, int y, double val_r, double val_g, double val_b) {
  if (x >= 0 && y >= 0 && x < w_ && y < h_) {
    if (gamma_ == 0) {
      double c_r = val_r / double(max_val_);
      double c_g = val_g / double(max_val_);
      double c_b = val_b / double(max_val_);
      uchar r = 0;
      uchar g = 0;
      uchar b = 0;
      if (c_r <= 0.0031308) {
        r = uchar(12.92 * c_r * max_val_);
      } else {
        r = uchar((1.055 * pow(c_r, 1.0 / 2.4) - 0.055) * max_val_);
      }
      if (c_g <= 0.0031308) {
        g = uchar(12.92 * c_g * max_val_);
      } else {
        g = uchar((1.055 * pow(c_g, 1.0 / 2.4) - 0.055) * max_val_);
      }
      if (c_b <= 0.0031308) {
        b = uchar(12.92 * c_b * max_val_);
      } else {
        b = uchar((1.055 * pow(c_b, 1.0 / 2.4) - 0.055) * max_val_);
      }
      data_[y * w_ + x] = {r, g, b};
    } else {
      data_[y * w_ + x] = {uchar(round(pow(val_r / max_val_, 1.0 / gamma_) * double(max_val_))),
                           uchar(round(pow(val_g / max_val_, 1.0 / gamma_) * double(max_val_))),
                           uchar(round(pow(val_b / max_val_, 1.0 / gamma_) * double(max_val_)))};
    }
  }
}

template<>
void PGMImage<PGMMonoPixel>::CorrectImageWithGamma() {
  for (int y = 0; y < GetHeight(); y++) {
    for (int x = 0; x < GetWidth(); x++) {
      PutPixel(x, y, {uchar(std::pow((double) GetPixel(x, y).val / (double) max_val_, 1.0 / gamma_) * max_val_)});
    }
  }
}

template<>
void PGMImage<PGMColorPixel>::CorrectImageWithGamma() {
  for (int y = 0; y < GetHeight(); y++) {
    for (int x = 0; x < GetWidth(); x++) {
      PutPixel(x, y, {uchar(std::pow((double) GetPixel(x, y).r / (double) max_val_, 1.0 / gamma_) * max_val_),
                      uchar(std::pow((double) GetPixel(x, y).g / (double) max_val_, 1.0 / gamma_) * max_val_),
                      uchar(std::pow((double) GetPixel(x, y).b / (double) max_val_, 1.0 / gamma_) * max_val_)});
    }
  }
}

template<typename T>
T *PGMImage<T>::operator[](int i) {
  return data_ + i * w_;
}

template<typename T>
int PGMImage<T>::GetWidth() const {
  return w_;
}

template<typename T>
int PGMImage<T>::GetHeight() const {
  return h_;
}

template<typename T>
int PGMImage<T>::GetMaxVal() const {
  return max_val_;
}

template<class T>
FileType PGMImage<T>::GetFileType() const {
  return type_;
}

template<typename T>
void PGMImage<T>::WriteImg(const std::string &fname) {
  FILE *f = fopen(fname.c_str(), "wb");
  if (!f) {
    f = fopen(fname.c_str(), "ab+");
    if (!f) {
      throw PGMImageFileDeleteException();
    }
    throw PGMImageFileOpenException();
  }
  char *head = new char[MAX_HEADER_SIZE];
  int len = snprintf(head, MAX_HEADER_SIZE, "P%i\n%i %i\n%invert\n", type_, w_, h_,
                     max_val_);
  fwrite(head, 1, len, f);
  auto *buf = (uchar *) data_;
  fwrite(buf, sizeof(T), w_ * h_, f);
  delete[](head);
  fclose(f);
}

template<typename T>
bool PGMImage<T>::FileExists(const char *s) {
  FILE *file;
  file = fopen((const char *) s, "r");
  if (file) {
    fclose(file);
    return true;
  }
  return false;
}

template<class T>
void PGMImage<T>::WriteImg() {
  FILE *f = fopen(fname_.c_str(), "wb");
  char head[MAX_HEADER_SIZE];
  int len = snprintf(head, MAX_HEADER_SIZE, "P%i\n%i %invert\n%i\n", type_, w_, h_,
                     max_val_);
  fwrite(head, 1, len, f);
  auto *buf = (uchar *) data_;
  fwrite(buf, sizeof(T), w_ * h_, f);
}

template<class T>
T apply_alpha(T bright, T under_color, int a, double gamma) {
  double b = under_color.val / 255.0;
  double f = bright.val / 255.0;
  double alpha = (double) a / 255.0;
  double out = pow(pow(f, gamma) * alpha + pow(b, gamma) * (1.0 - alpha), 1.0 / gamma);
  return {(uchar) (255.0 * out)};
}

template<class T>
PGMImage<T>::PGMImage(const PGMImage<T> &img)
    : w_(img.GetWidth()), h_(img.GetHeight()), max_val_(img.GetMaxVal()), type_(img.GetFileType()) {
  data_ = new T[w_ * h_];
  for (int i = 0; i < w_ * h_; i++) {
    data_[i] = img.data_[i];
  }
}

template<class T>
double PGMImage<T>::IntPart(double x) {
  return floor(x);
}

template<class T>
double PGMImage<T>::FloatPart(double x) {
  return x - floor(x);
}

template<>
void PGMImage<PGMMonoPixel>::FillWithGradient() {
  for (int y = 0; y < h_; y++) {
    for (int x = 0; x < w_; x++) {
      PutPixelWithGamma(x, y, double(x) * max_val_ / w_);
    }
  }
}
template<class T>
PGMMonoPixel PGMImage<T>::Clamp(double val) {
  return {uchar(std::min(std::max(val, 0.0), double(max_val_)))};
}
template<class T>
PGMColorPixel PGMImage<T>::Clamp(double val_r, double val_g, double val_b) {
  return {uchar(std::min(std::max(val_r, 0.0), double(max_val_))),
          uchar(std::min(std::max(val_g, 0.0), double(max_val_))),
          uchar(std::min(std::max(val_b, 0.0), double(max_val_)))};
}

template<>
void PGMImage<PGMColorPixel>::FillWithGradient() {
  for (int y = 0; y < h_; y++) {
    for (int x = 0; x < w_; x++) {
      PutPixelWithGamma(x, y, (double) x * max_val_ / w_, (double) x * max_val_ / w_, (double) x * max_val_ / w_);
    }
  }
}

template<class T>
double PGMImage<T>::GetGamma() const {
  return gamma_;
}

template<>
PGMImage<PGMMonoPixel>* PGMImage<PGMColorPixel>::GetImageByChannel(char channel_name) {
  PGMImage<PGMMonoPixel>* img = new PGMImage<PGMMonoPixel>(w_, h_, max_val_, P5, gamma_);
  switch (channel_name) {
    case 'R': {
      for (int y = 0; y < h_; y++) {
        for (int x = 0; x < w_; x++) {
          img->PutPixel(x, y, {GetPixel(x, y).r});
        }
      }
    }
      break;
    case 'G': {
      for (int y = 0; y < h_; y++) {
        for (int x = 0; x < w_; x++) {
          img->PutPixel(x, y, {GetPixel(x, y).g});
        }
      }
    }
      break;
    case 'B': {
      for (int y = 0; y < h_; y++) {
        for (int x = 0; x < w_; x++) {
          img->PutPixel(x, y, {GetPixel(x, y).b});
        }
      }
    }
      break;
    default: {
      //Never happen
    }
  }
  return img;
}

#endif //PGMPP_SRC_PGMIMAGE_IMPL_H_
