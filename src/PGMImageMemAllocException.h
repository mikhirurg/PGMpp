//
// Created by @mikhirurg on 11.04.2020.
//

#ifndef COMPUTERGEOMETRY_GRAPHICS_CIMAGEMEMALLOCEXCEPTION_H
#define COMPUTERGEOMETRY_GRAPHICS_CIMAGEMEMALLOCEXCEPTION_H

#include "PGMImageException.h"

class PGMImageMemAllocException : public PGMImageException {
 public:
  PGMImageMemAllocException();
};

PGMImageMemAllocException::PGMImageMemAllocException()
    : PGMImageException("EXCEPTION: Cannot allocate memory!") {

}

#endif //COMPUTERGEOMETRY_GRAPHICS_CIMAGEMEMALLOCEXCEPTION_H
