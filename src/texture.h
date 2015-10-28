#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdio.h>

#include <SDL2/SDL_image.h>

#include <GL/GL.h>

#include "types.h"

#define GL_UNSIGNED_INT_8_8_8_8 0x8035

bool Texture_Load(uint *id, const char *file);

#endif