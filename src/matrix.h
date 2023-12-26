#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"

typedef struct {
  float m[4][4]; // 4x4 matrix -> 4 rows and 4 columns
} mat4_t;

mat4_t mat4_identity(void);
mat4_t mat4_scale(float sx, float sy, float sz);
mat4_t mat4_translation(float tx, float ty, float tz);
vec4_t mat4_t_mul_vec4(mat4_t m, vec4_t v);

#endif