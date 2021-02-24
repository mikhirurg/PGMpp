//
// Created by @mikhirurg on 11.04.2020.
//

#ifndef COMPUTERGEOMETRY_GRAPHICS_CIMAGEFILEDELETEEXCEPTION_H
#define COMPUTERGEOMETRY_GRAPHICS_CIMAGEFILEDELETEEXCEPTION_H

#include "PGMImageException.h"

class PGMImageFileDeleteException : public PGMImageException {
 public:
  PGMImageFileDeleteException();
};

PGMImageFileDeleteException::PGMImageFileDeleteException()
    : PGMImageException("EXCEPTION: Cannot delete file!") {

}

#endif //COMPUTERGEOMETRY_GRAPHICS_CIMAGEFILEDELETEEXCEPTION_H
