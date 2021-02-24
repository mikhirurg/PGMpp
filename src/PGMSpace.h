//
// Created by @mikhirurg on 06.06.2020.
//

#ifndef LAB4__CSPACE_H_
#define LAB4__CSPACE_H_
#include <string>
#include <map>
#include <cmath>
#include "PGMPixel.h"

class CSpaceException {
  std::string err_m_;
 public:
  CSpaceException(const std::string &str) : err_m_(str) {
  }

  operator std::string() {
    return err_m_;
  }
};

class PGMSpace {
  std::string name;
  static std::map<const std::string, PGMSpace *> cs_map;
 public:
  PGMSpace(std::string nm) : name(nm) {}
  virtual PGMColorPixel FromRGB(PGMColorPixel c) = 0;
  virtual PGMColorPixel ToRGB(PGMColorPixel c) = 0;
  virtual PGMColorPixelDbl FromRGB(PGMColorPixelDbl c) = 0;
  virtual PGMColorPixelDbl ToRGB(PGMColorPixelDbl c) = 0;

  static PGMSpace &CSpaceByName(const std::string &name);
};

class RGB : public PGMSpace {
 public:
  RGB() : PGMSpace("RGB") {}
  PGMColorPixel FromRGB(PGMColorPixel c) override {
    return c;
  }
  PGMColorPixel ToRGB(PGMColorPixel c) override {
    return c;
  }
  PGMColorPixelDbl FromRGB(PGMColorPixelDbl c) override {
    return c;
  }
  PGMColorPixelDbl ToRGB(PGMColorPixelDbl c) override {
    return c;
  }

};

class HSV : public PGMSpace {
 public:
  HSV() : PGMSpace("HSV") {}
  PGMColorPixel FromRGB(PGMColorPixel c) override;
  PGMColorPixel ToRGB(PGMColorPixel c) override;
  PGMColorPixelDbl FromRGB(PGMColorPixelDbl c) override;
  PGMColorPixelDbl ToRGB(PGMColorPixelDbl c) override;
};

class HSL : public PGMSpace {
 public:
  HSL() : PGMSpace("HSL") {}
  PGMColorPixel FromRGB(PGMColorPixel c) override;
  PGMColorPixel ToRGB(PGMColorPixel c) override;
  PGMColorPixelDbl FromRGB(PGMColorPixelDbl c) override;
  PGMColorPixelDbl ToRGB(PGMColorPixelDbl c) override;
};

class CMY : public PGMSpace {
 public:
  CMY() : PGMSpace("CMY") {}
  PGMColorPixel FromRGB(PGMColorPixel c) override;
  PGMColorPixel ToRGB(PGMColorPixel c) override;
  PGMColorPixelDbl FromRGB(PGMColorPixelDbl c) override;
  PGMColorPixelDbl ToRGB(PGMColorPixelDbl c) override;
};

class YCoCg : public PGMSpace {
 public:
  YCoCg() : PGMSpace("YCoCg") {}
  PGMColorPixel FromRGB(PGMColorPixel c) override;
  PGMColorPixel ToRGB(PGMColorPixel c) override;
  PGMColorPixelDbl FromRGB(PGMColorPixelDbl c) override;
  PGMColorPixelDbl ToRGB(PGMColorPixelDbl c) override;
};

class YCbCr601 : public PGMSpace {
 public:
  YCbCr601() : PGMSpace("YCbCr.601") {}
  PGMColorPixel FromRGB(PGMColorPixel c) override;
  PGMColorPixel ToRGB(PGMColorPixel c) override;
  PGMColorPixelDbl FromRGB(PGMColorPixelDbl c) override;
  PGMColorPixelDbl ToRGB(PGMColorPixelDbl c) override;
};

class YCbCr709 : public PGMSpace {
 public:
  YCbCr709() : PGMSpace("YCbCr.709") {}
  PGMColorPixel FromRGB(PGMColorPixel c) override;
  PGMColorPixel ToRGB(PGMColorPixel c) override;
  PGMColorPixelDbl FromRGB(PGMColorPixelDbl c) override;
  PGMColorPixelDbl ToRGB(PGMColorPixelDbl c) override;
};

#include "PGMDitherer_impl.h"

#endif //LAB4__CSPACE_H_
