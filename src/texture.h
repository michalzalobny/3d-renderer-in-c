#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdint.h>

typedef struct {
  float u;
  float v;
} tex2_t;

extern int texture_width;
extern int texture_height;

extern const uint8_t REDBRICK_TEXTURE[];

extern uint32_t* mesh_texture;

#endif