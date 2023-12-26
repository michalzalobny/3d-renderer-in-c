#ifndef MATRIX_H
#define MATRIX_H

typedef struct {
  float m[4][4]; // 4x4 matrix -> 4 rows and 4 columns
} mat4_t;

mat4_t mat4_identity(void);
mat4_t mat4_scale(float sx, float sy, float sz);

#endif