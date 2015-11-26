#ifndef MD3LOADER_H
#define MD3LOADER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "types.h"
#include "utils.h"
#include "texture.h"
#include "md3structs.h"

#define MD3_VERSION 15
#define MD3_IDENT 0x33504449
#define MD3_SURFACE_IDENT 0x33504449

bool MD3Loader_Load(MD3Model *model, const char *file);
bool MD3Loader_ReadHeader(FILE *f, MD3Header *header);

bool MD3Loader_ReadFrames(FILE *f, MD3Model *model);
bool MD3Loader_ReadTags(FILE *f, MD3Model *model);
bool MD3Loader_ReadSurfaces(FILE *f, MD3Model *model);

bool MD3Loader_ReadShaders(FILE *f, MD3Surface *surface);
bool MD3Loader_ReadTriangles(FILE *f, MD3Surface *surface);
bool MD3Loader_ReadTextCoords(FILE *f, MD3Surface *surface);
bool MD3Loader_ReadVertices(FILE *f, MD3Surface *surface);

bool MD3Loader_LoadTextures(FILE *f, MD3Model *model, MD3Surface *surface);

void MD3Loader_FreeSurface(MD3Surface *surface);
void MD3Loader_FreeModel(MD3Model *model);

#endif