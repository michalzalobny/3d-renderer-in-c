#include "matrix.h"

mat4_t mat4_identity(void){
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

mat4_t mat4_scale(float sx, float sy, float sz){
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

vec4_t mat4_t_mul_vec4(mat4_t m, vec4_t v){
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
