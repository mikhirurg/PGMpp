//
// Created by @mikhirurg on 11.04.2020.
//

#ifndef COMPUTERGEOMETRY_GRAPHICS_CIMAGEFILEREADEXCEPTION_H
#define COMPUTERGEOMETRY_GRAPHICS_CIMAGEFILEREADEXCEPTION_H

#include "PGMImageException.h"

class PGMImageFileReadException : public PGMImageException {
 public:
  PGMImageFileReadException();
};

PGMImageFileReadException::PGMImageFileReadException()
    : PGMImageException("EXCEPTION: Error while reading data") {

}

#endif //COMPUTERGEOMETRY_GRAPHICS_CIMAGEFILEREADEXCEPTION_H
