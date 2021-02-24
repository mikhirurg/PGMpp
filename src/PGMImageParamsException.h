//
// Created by @mikhirurg on 11.04.2020.
//

#ifndef COMPUTERGEOMETRY_GRAPHICS_CIMAGEPARAMSEXCEPTION_H
#define COMPUTERGEOMETRY_GRAPHICS_CIMAGEPARAMSEXCEPTION_H

#include "PGMImageException.h"

class PGMImageParamsException : public PGMImageException {
 public:
  PGMImageParamsException();
};

PGMImageParamsException::PGMImageParamsException()
    : PGMImageException("EXCEPTION: Wrong params!") {

}

#endif //COMPUTERGEOMETRY_GRAPHICS_CIMAGEPARAMSEXCEPTION_H
