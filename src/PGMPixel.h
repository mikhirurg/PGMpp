//
// Created by @mikhirurg on 06.06.2020.
//

#ifndef LAB4__CPIXEL_H_
#define LAB4__CPIXEL_H_
typedef unsigned char uchar;

struct PGMMonoPixel {
  uchar val;
};
struct PGMColorPixelDbl;
struct PGMColorPixel {
  union {
    uchar r;
    uchar h;
    uchar c;
    uchar Y;
  };
  union {
    uchar g;
    uchar s;
    uchar m;
    uchar Co;
    uchar Cb;
  };
  union {
    uchar b;
    uchar v;
    uchar l;
    uchar y;
    uchar Cg;
    uchar Cr;
  };

  operator PGMColorPixelDbl();
};

struct PGMColorPixelDbl {
  union {
    double r;
    double h;
    double c;
    double Y;
  };
  union {
    double g;
    double s;
    double m;
    double Co;
    double Cb;
  };
  union {
    double b;
    double v;
    double l;
    double y;
    double Cg;
    double Cr;
  };

  operator PGMColorPixel() {
    return {(uchar) (r * 255.0), (uchar) (g * 255.0), (uchar) (b * 255.0)};
  }
};

PGMColorPixel::operator PGMColorPixelDbl() {
  return {r/255.0, g/255.0, b/255.0};
}

#endif //LAB4__CPIXEL_H_
