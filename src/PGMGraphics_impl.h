//
// Created by @mikhirurg on 09.08.2020.
//

#ifndef PGMPP_SRC_PGMGRAPHICS_IMPL_H_
#define PGMPP_SRC_PGMGRAPHICS_IMPL_H_

#include "PGMGraphics.h"

template<class T>
void PGMGraphics<T>::drawLine(uchar bright, double thickness, double x1, double y1,
                              double x2, double y2, double gamma) {
  if (thickness > 1) {
    int scale_x = 4;
    int scale_y = 4;

    std::vector<std::pair<double, double>>
        points = CalculateLineBorderPoints(
        thickness, x1, y1, x2, y2);
    std::pair<double, double> upper_corner = GetUpperCorner(points);

    ScaleBorderPoints(points, scale_x, scale_y);

    Polygon polygon;
    polygon.GroupAdd(points);
    polygon.close();

    std::pair<double, double>
        bounds = GetScaledBounds(points, scale_x, scale_y);

    PGMImage<PGMMonoPixel> tmp = PGMImage<PGMMonoPixel>(bounds.first, bounds.second, img_.GetMaxVal(), P5, img_.GetGamma());

    FillPolygon(polygon, tmp, {255});

#ifdef DUMP_TMP
    tmp.writeImg("tmp.pgm");
#endif
    DrawDownscaled(tmp, scale_x, scale_y, upper_corner, {bright}, gamma);
  } else {
    DrawWuLine({bright}, thickness, x1, y1, x2, y2, gamma);
  }
}

template<class T>
std::vector<std::pair<double, double>>
PGMGraphics<T>::CalculateLineBorderPoints(double thickness, double x1, double y1,
                                          double x2,
                                          double y2) {
  std::vector<std::pair<double, double>> border_points;
  x1 += 0.5;
  y1 += 0.5;
  x2 += 0.5;
  y2 += 0.5;

  double l = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
  double th2 = thickness / 2;
  double ex = (x2 - x1) / l;
  double ey = (y2 - y1) / l;
  x1 -= ex * th2;
  y1 -= ey * th2;

  x2 += ex * th2;
  y2 += ey * th2;

  if (std::abs(y2 - y1) > eps) {
    double d = (x2 - x1) / (y2 - y1);
    double a = 1 + d * d;
    double b = -(2 * d * d * x1 + 2 * x1);
    double c = x1 * x1 + d * d * x1 * x1 - thickness * thickness / 4;

    double xa_1 = (-b + sqrt(b * b - 4 * a * c)) / (2 * a);
    double xa_2 = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);

    double ya_1 = -d * xa_1 + d * x1 + y1;
    double ya_2 = -d * xa_2 + d * x1 + y1;

    b = -(2 * d * d * x2 + 2 * x2);
    c = x2 * x2 + d * d * x2 * x2 - thickness * thickness / 4;

    double xb_1 = (-b + sqrt(b * b - 4 * a * c)) / (2 * a);
    double xb_2 = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);

    double yb_1 = -d * xb_1 + d * x2 + y2;
    double yb_2 = -d * xb_2 + d * x2 + y2;

    border_points.emplace_back(xa_1, ya_1);
    border_points.emplace_back(xa_2, ya_2);
    border_points.emplace_back(xb_2, yb_2);
    border_points.emplace_back(xb_1, yb_1);
    border_points.emplace_back(xa_1, ya_1);
  } else {
    border_points.emplace_back(x1, y1 - thickness / 2);
    border_points.emplace_back(x2, y2 - thickness / 2);
    border_points.emplace_back(x2, y2 + thickness / 2);
    border_points.emplace_back(x1, y1 + thickness / 2);
    border_points.emplace_back(x1, y1 - thickness / 2);
  }
  return border_points;
}

template<class T>
void
PGMGraphics<T>::ScaleBorderPoints(std::vector<std::pair<double, double>> &points,
                                  int scale_x, int scale_y) {
  double x_min = INT32_MAX;
  double y_min = INT32_MAX;
  double x_max = INT32_MIN;
  double y_max = INT32_MIN;
  for (std::pair<double, double> p: points) {
    x_min = std::min(x_min, p.first);
    x_max = std::max(x_max, p.first);
    y_min = std::min(y_min, p.second);
    y_max = std::max(y_max, p.second);
  }

  for (std::pair<double, double> &p: points) {
    p.first -= floor(x_min);
    p.second -= floor(y_min);
    p.first = p.first * scale_x;
    p.second = (p.second * scale_y);
  }
}

template<class T>
std::pair<double, double> PGMGraphics<T>::GetScaledBounds(
    const std::vector<std::pair<double, double>> &points,
    int scale_x,
    int scale_y) {
  double x_min = INT32_MAX;
  double y_min = INT32_MAX;
  double x_max = INT32_MIN;
  double y_max = INT32_MIN;
  for (std::pair<double, double> p: points) {
    x_min = floor(std::min(x_min, p.first));
    x_max = ceil(std::max(x_max, p.first));
    y_min = floor(std::min(y_min, p.second));
    y_max = ceil(std::max(y_max, p.second));
  }
  return std::make_pair(x_max - x_min, y_max - y_min);
}

template<class T>
std::pair<double, double> PGMGraphics<T>::GetUpperCorner(
    const std::vector<std::pair<double, double>> &points) {
  double x_min = INT32_MAX;
  double y_min = INT32_MAX;
  for (std::pair<double, double> p: points) {
    x_min = std::min(x_min, p.first);
    y_min = std::min(y_min, p.second);
  }
  return std::make_pair(floor(x_min), floor(y_min));
}

template<class T>
void
PGMGraphics<T>::DrawDownscaled(const PGMImage<PGMMonoPixel> &img, int scale_x,
                               int scale_y,
                               const std::pair<double, double> &start_coord,
                               T bright,
                               double gamma) {
  int alpha_val = 0;
  int y;
  int x;
  for (y = 0; y < img.GetHeight(); y += scale_y) {
    for (x = 0; x < img.GetWidth(); x += scale_x) {
      for (int j = 0; j < scale_y; j++) {
        for (int i = 0; i < scale_x; i++) {
          alpha_val += img.GetPixel(x + i, y + j).val;
        }
      }
      alpha_val /= (scale_x * scale_y);
      T pix = img_.GetPixel(x / scale_x + (int) start_coord.first,
                             y / scale_y + (int) start_coord.second);
      img_.PutPixel(x / scale_x + (int) start_coord.first,
                     y / scale_y + (int) start_coord.second,
                     apply_alpha(bright, pix, alpha_val, gamma));
      alpha_val = 0;
    }
  }
}

template<class T>
void
PGMGraphics<T>::plot(PGMImage<T> &img, double x, double y, double alpha, PGMMonoPixel bright,
                     double gamma) {
  if (x >= 0 && y >= 0 && x < img.GetWidth() && y < img.GetHeight()) {
    img.PutPixel(x, y, apply_alpha(bright, img.GetPixel(x, y), alpha, gamma));
  }
}

template<class T>
double PGMGraphics<T>::intPart(double x) {
  return floor(x);
}

template<class T>
double PGMGraphics<T>::FloatPart(double x) {
  return x - floor(x);
}

template<class T>
void PGMGraphics<T>::DrawWuLine(PGMMonoPixel brightness, double thickness, double x1,
                                double y1,
                                double x2, double y2, double gamma) {
  bool check = abs(y2 - y1) > abs(x2 - x1);
  brightness.val *= thickness;
  if (check) {
    std::swap(x1, y1);
    std::swap(x2, y2);
  }
  if (x1 > x2) {
    std::swap(x1, x2);
    std::swap(y1, y2);
  }
  double dx = x2 - x1;
  double dy = y2 - y1;
  double delta = dy / dx;

  if (dx == 0.0) {
    delta = 1.0;
  }

  if (check) {
    plot(img_, y1, x1, img_.GetMaxVal(), brightness, gamma);
    plot(img_, y2, x2, img_.GetMaxVal(), brightness, gamma);
    double y = y1 + delta;
    for (double x = x1 + 1.0; x < x2; x++) {
      plot(img_, intPart(y), x,
           (double) img_.GetMaxVal() * (1.0 - FloatPart(y)), brightness,
           gamma);
      plot(img_, intPart(y) + 1.0, x,
           (double) img_.GetMaxVal() * FloatPart(y), brightness, gamma);
      y += delta;
    }
  } else {
    plot(img_, x1, y1, img_.GetMaxVal(), brightness, gamma);
    plot(img_, x2, y2, img_.GetMaxVal(), brightness, gamma);
    double y = y1 + delta;
    for (double x = x1 + 1.0; x < x2; x++) {
      plot(img_, x, intPart(y),
           (double) img_.GetMaxVal() * (1.0 - FloatPart(y)), brightness,
           gamma);
      plot(img_, x, intPart(y) + 1.0,
           (double) img_.GetMaxVal() * FloatPart(y), brightness, gamma);
      y += delta;
    }
  }
}

template<class T>
void PGMGraphics<T>::ShiftPoints(std::vector<std::pair<double, double>> &points,
                                 double shift_x, double shift_y) {
  for (std::pair<double, double> &p: points) {
    p.first += shift_x;
    p.second += shift_y;
  }
}

template<class T>
void PGMGraphics<T>::Polygon::addActive(int p, double cy) {
  Point &np = (*this)[p + 1];
  Point &pnt = (*this)[p];
  Edge &ne = active[nact];

  if (pnt.y == np.y) {
    return;
  } else {
    double dX = np.x - pnt.x;
    double dY = np.y - pnt.y;
    double dy;
    if (pnt.y < np.y) {
      ne.dir = -1;
      ne.x = pnt.x;
      dy = cy - pnt.y;
    } else {
      ne.dir = 1;
      ne.x = np.x;
      dy = cy - np.y;
    }
    ne.dx = dX / dY;
    ne.x += (dX * dy) / dY;
  }
  pnt.edge = &ne;
  active_list.push_back(&ne);
  nact++;
}

template<class T>
void PGMGraphics<T>::Polygon::sortActive() {
  struct {
    bool operator()(Edge *a, Edge *b) const {
      return a->x <= b->x;
    }
  } le;
  std::sort(active_list.begin(), active_list.end(), le);
}

template<class T>
void PGMGraphics<T>::Polygon::delActive(Edge *e) {
  active_list.erase(std::remove(active_list.begin(), active_list.end(), e),
                    active_list.end());
}

template<class T>
void PGMGraphics<T>::FillPolygon(Polygon &polygon, PGMImage<T> &img, T color) {
  double k, y, xl, xr;
  int drawing;
  int right_bound = img.GetWidth() - 1;
  double y_min = polygon.getYMin();
  double y_max = polygon.getYMax();
  int hash_size = (int(y_max - y_min)) + 4;

  int hash_offset = (int) ceil(y_min - 0.5 - eps);

  int counter;

  int counter_mask = -1;

  if (polygon.size() <= 1) return;

  std::map<int, std::set<int>> y_hash;

  for (int i = 0; i < polygon.size(); i++) {
    Point &next_point = polygon[i + 1];
    int key = (int) ceil(polygon[i].y - hash_offset - 0.5);
    y_hash[key].insert(i);
    polygon[i].edge = 0;
  }

  for (y = hash_offset + 0.5, k = 0;
       y <= y_max && k <= hash_size; y += 1.0, k++) {
    for (auto it = y_hash[k].begin(); it != y_hash[k].end(); ++it) {
      Point &prev = polygon[*it - 1];
      Point &next = polygon[*it + 1];
      Point &pt = polygon[*it];
      if (!prev.last_point) {
        if (prev.edge && prev.y <= y) {
          polygon.delActive(prev.edge);
          prev.edge = 0;
        } else if (prev.y > y) {
          polygon.addActive(*it - 1, y);
        }
      }

      if (!pt.last_point) {
        if (pt.edge && next.y <= y) {
          polygon.delActive(pt.edge);
          pt.edge = 0;
        } else if (next.y > y) {
          polygon.addActive(*it, y);
        }
      }
    }

    if (polygon.active_list.empty()) continue;

    polygon.sortActive();

    xl = xr = 0;
    counter = 0;
    drawing = 0;
    for (auto curEdge: polygon.active_list) {
      counter += curEdge->dir;
      if ((counter & counter_mask) && !drawing) {
        xl = floor(curEdge->x);
        drawing = 1;
      }

      if (!(counter & counter_mask) && drawing) {
        xr = floor(curEdge->x);

        if (xl <= xr) {
          for (int i = xl; i <= xr; i++) {
            img.PutPixel(i, y, color);
          }
        }
        drawing = 0;
      }

      curEdge->x += curEdge->dx;
    }

    if (drawing && xl <= right_bound) {
      for (int i = xl; i <= right_bound; i++) {
        img.PutPixel(i, y, color);
      }
    }
  }
}

template<class T>
void PGMGraphics<T>::swap_pixels(PGMImage<T> &img, int x1, int y1, int x2, int y2) {
  if (x1 < img.GetWidth() && x1 >= 0 && y1 < img.GetHeight() && y1 >= 0 && x2 < img.GetWidth() &&
      x2 >= 0 &&
      y2 < img.GetHeight() && y2 >= 0) {
    T pixel = img.GetPixel(x1, y1);
    img.PutPixel(x1, y1, img.GetPixel(x2, y2));
    img.PutPixel(x2, y2, pixel);
  }

}

template<class T>
void PGMGraphics<T>::invert() {
  for (int y = 0; y < img_.GetHeight(); y++) {
    for (int x = 0; x < img_.GetWidth(); ++x) {
      img_.PutPixel(x, y, {static_cast<uchar>(255 - img_.GetPixel(x, y).val)});
    }
  }
}

template<class T>
void PGMGraphics<T>::horizontalFlip() {
  for (int y = 0; y < img_.GetHeight(); y++) {
    for (int x = 0; x < img_.GetWidth() / 2; x++) {
      swap_pixels(img_, x, y, img_.GetWidth() - x - 1, y);
    }
  }
}

template<class T>
void PGMGraphics<T>::verticalFlip() {
  for (int y = 0; y < img_.GetHeight() / 2; y++) {
    for (int x = 0; x < img_.GetWidth(); x++) {
      swap_pixels(img_, x, y, x, img_.GetHeight() - y - 1);
    }
  }
}

#endif //PGMPP_SRC_PGMGRAPHICS_IMPL_H_
