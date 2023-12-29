#include <math.h>

#include "matrix.h"
#include "stdio.h"

mat4_t mat4_identity(void) {
  // | 1 0 0 0 |
  // | 0 1 0 0 |
  // | 0 0 1 0 |
  // | 0 0 0 1 |

  mat4_t eye = { // eye for identity
    .m = { // can skip the .m = part - it's just for clarity
      {1, 0, 0, 0}, 
      {0, 1, 0, 0}, 
      {0, 0, 1, 0}, 
      {0, 0, 0, 1}
    }
  };
  return eye;
}

vec4_t mat4_mul_vec4(mat4_t m, vec4_t v){
  // Example of this multiplication (values can be all different):
  // | sx 0 0 0 |   | x |   | x'|
  // | 0 sy 0 0 | X | y | = | y'|
  // | 0 0 sz 0 |   | z |   | z'|
  // | 0 0 0  1 |   | 1 |   | 1 |

  vec4_t result;

  result.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w;
  result.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w;
  result.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w;
  result.w = m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w;

  return result;
}

mat4_t mat4_mul_mat4(mat4_t a, mat4_t b) {
    mat4_t m;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m.m[i][j] = a.m[i][0] * b.m[0][j] + a.m[i][1] * b.m[1][j] + a.m[i][2] * b.m[2][j] + a.m[i][3] * b.m[3][j];
        }
    }
    return m;
}

mat4_t mat4_make_scale(float sx, float sy, float sz){
  // | sx 0 0 0 |
  // | 0 sy 0 0 |
  // | 0 0 sz 0 |
  // | 0 0 0 1 |

  mat4_t m = mat4_identity();

  m.m[0][0] = sx;
  m.m[1][1] = sy;
  m.m[2][2] = sz;

  return m;
}

mat4_t mat4_make_translation(float tx, float ty, float tz){
  // | 1 0 0 tx |
  // | 0 1 0 ty |
  // | 0 0 1 tz | 
  // | 0 0 0  1 | 

  mat4_t m = mat4_identity();

  m.m[0][3] = tx;
  m.m[1][3] = ty;
  m.m[2][3] = tz;

  return m;
}

mat4_t mat4_make_rotation_x(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    // | 1  0  0  0 |
    // | 0  c -s  0 |
    // | 0  s  c  0 |
    // | 0  0  0  1 |
    mat4_t m = mat4_identity();
    m.m[1][1] = c;
    m.m[1][2] = -s;
    m.m[2][1] = s;
    m.m[2][2] = c;
    return m;
}

mat4_t mat4_make_rotation_y(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    // |  c  0  s  0 |
    // |  0  1  0  0 |
    // | -s  0  c  0 |
    // |  0  0  0  1 |
    mat4_t m = mat4_identity();
    m.m[0][0] = c;
    m.m[0][2] = s;
    m.m[2][0] = -s;
    m.m[2][2] = c;
    return m;
}

mat4_t mat4_make_rotation_z(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    // | c -s  0  0 |
    // | s  c  0  0 |
    // | 0  0  1  0 |
    // | 0  0  0  1 |
    mat4_t m = mat4_identity();
    m.m[0][0] = c;
    m.m[0][1] = -s;
    m.m[1][0] = s;
    m.m[1][1] = c;
    return m;
}

//https://www.youtube.com/watch?v=U0_ONQQ5ZNM - more explanation of this those matrices
//https://www.youtube.com/watch?v=vu1VNKHfzqQ here too - start with this one



vec4_t mat4_mul_vec4_project(mat4_t mat_proj, vec4_t v) {
    // multiply the projection matrix by our original vector
    vec4_t result = mat4_mul_vec4(mat_proj, v);

    // perform perspective divide with original z-value that is now stored in w
    if (result.w != 0.0) {
        result.x /= result.w;
        result.y /= result.w;
        result.z /= result.w;
    }
    return result;
}

// translate and scale projected vector to -1 to 1 range
mat4_t mat4_make_ortho(float l, float b, float n, float r, float t, float f) {
    // | 2/(r-l)        0              0           -(r+l)/(r-l) |
    // |       0  2/(t-b)              0           -(t+b)/(t-b) |
    // |       0        0        2/(f-n)           -(f+n)/(f-n) |
    // |       0        0              0                      1 |
    mat4_t m = {{{ 0 }}};
    m.m[0][0] = 2.0 / (r - l);
    m.m[0][3] = -(r + l) / (r - l);
    m.m[1][1] = 2.0 / (t - b);
    m.m[1][3] = -(t + b) / (t - b);
    m.m[2][2] = 2.0 / (f - n);
    m.m[2][3] = -(f + n) / (f - n);
    m.m[3][3] = 1.0;

    return m;
}

mat4_t mat4_make_perspective(float n, float f) {
    // | n  0       0     0 |
    // | 0  n       0     0 |
    // | 0  0   (f+n) (-fn) |
    // | 0  0       1     0 |
   
    mat4_t m = {{{ 0 }}};
    m.m[0][0] = n;
    m.m[1][1] = n;
    m.m[2][2] = f + n;
    m.m[2][3] = -f * n;
    m.m[3][2] = 1.0;

    return m;
} 

mat4_t mat4_make_projection(float fov, float aspect_ratio, float near, float far) {
    // Orthographic matrix X Perspective matrix

    // Usually r,t,f = 1.0 and l,b,n = -1.0, but here we adjust them to the aspect ratio and fov

    float r = near * tan(fov / 2.0) * aspect_ratio; // aspect_ratio = width / height
    float t = near * tan(fov / 2.0);
    float f = 1.0;
  
    float l = -r;
    float b = -t;
    float n = -f;

    mat4_t t_ortho = mat4_make_ortho(l, b, n, r, t, f);
    mat4_t t_perspective = mat4_make_perspective(near, far);
  
    mat4_t m = mat4_mul_mat4(t_ortho, t_perspective);
    return m;
}


mat4_t mat4_make_perspective_old(float fov, float aspect, float znear, float zfar){
  // | (h/w)*1/tan(fov/2)             0              0                 0 |
  // |                  0  1/tan(fov/2)              0                 0 |
  // |                  0             0     zf/(zf-zn)  (-zf*zn)/(zf-zn) |
  // |                  0             0              1                 0 |
  mat4_t m = {{{ 0 }}};
  m.m[0][0] = aspect * (1 / tan(fov / 2));
  m.m[1][1] = 1 / tan(fov / 2);
  m.m[2][2] = zfar / (zfar - znear);
  m.m[2][3] = (-zfar * znear) / (zfar - znear);
  m.m[3][2] = 1.0;
  return m;
}


mat4_t mat4_make_perspective_opengl(float fov, float aspect, float znear, float zfar){
  // |  n/r   0      0              0            |
  // |  0     n/t    0              0            |
  // |  0     0      -(f+n)/(f-n)   (-2fn)/(f-n) |
  // |  0     0      -1             0            |

  float n = znear;
  float f = zfar;
  float t = tan(fov / 2) * n;
  float r = t * aspect;

  mat4_t m = {{{ 0 }}};
  m.m[0][0] = n / r;
  m.m[1][1] = n / t;
  m.m[2][2] = -(f + n) / (f - n);
  m.m[2][3] = (-2 * f * n) / (f - n);
  m.m[3][2] = -1.0;

  return m;

}


