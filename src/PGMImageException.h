//
// Created by @mikhirurg on 11.04.2020.
//

#ifndef COMPUTERGEOMETRY_GRAPHICS_CIMAGEEXCEPTION_H
#define COMPUTERGEOMETRY_GRAPHICS_CIMAGEEXCEPTION_H

#include <string>

class PGMImageException {
 public:
  PGMImageException(const std::string &err_m);

  const char *getErr();

 private:
  std::string err_m_;
};

PGMImageException::PGMImageException(const std::string &err_m)
    : err_m_(err_m) {

}

const char *PGMImageException::getErr() {
  return err_m_.c_str();
}

#endif //COMPUTERGEOMETRY_GRAPHICS_CIMAGEEXCEPTION_H
