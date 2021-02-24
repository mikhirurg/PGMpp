//
// Created by @mikhirurg on 11.04.2020.
//

#ifndef COMPUTERGEOMETRY_GRAPHICS_CIMAGEFILEFORMATEXCEPTION_H
#define COMPUTERGEOMETRY_GRAPHICS_CIMAGEFILEFORMATEXCEPTION_H

#include "PGMImageException.h"

class PGMImageFileFormatException : public PGMImageException {
 public:
  PGMImageFileFormatException();
};

PGMImageFileFormatException::PGMImageFileFormatException()
    : PGMImageException("EXCEPTION: Wrong file format!") {

}

#endif //COMPUTERGEOMETRY_GRAPHICS_CIMAGEFILEFORMATEXCEPTION_H
