#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdint.h>
#include "vector.h"

typedef struct {
  int a;
  int b;
  int c;
  uint32_t color;
} face_t;

typedef struct {
  vec2_t points[3];
  uint32_t color;
  float avg_depth;
} triangle_t;

void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

#endif
