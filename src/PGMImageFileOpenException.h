//
// Created by @mikhirurg on 11.04.2020.
//

#ifndef COMPUTERGEOMETRY_GRAPHICS_CIMAGEFILEOPENEXCEPTION_H
#define COMPUTERGEOMETRY_GRAPHICS_CIMAGEFILEOPENEXCEPTION_H

#include "PGMImageException.h"

class PGMImageFileOpenException : public PGMImageException {
 public:
  PGMImageFileOpenException();
};

PGMImageFileOpenException::PGMImageFileOpenException()
    : PGMImageException("EXCEPTION: Cannot open file!") {

}

#endif //COMPUTERGEOMETRY_GRAPHICS_CIMAGEFILEOPENEXCEPTION_H
