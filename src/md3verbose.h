#ifndef MD3Verbose__H
#define MD3Verbose__H

#include <stdio.h>

#include "types.h"
#include "utils.h"

#include "md3structs.h"

void MD3Verbose_PrintModelInfo(const MD3Model *model);
void MD3Verbose_PrintSurfaceInfo(const MD3Surface *surface);

void MD3Verbose_Start();
void MD3Verbose_End();

void MD3Verbose_Header(const MD3Header *header);

void MD3Verbose_Frame(MD3Frame frame);
void MD3Verbose_Tag(MD3Tag tag);
void MD3Verbose_Surface(const MD3Surface *surface);

void MD3Verbose_Shader(MD3Shader shader);
void MD3Verbose_Triangle(MD3Triangle triangle);
void MD3Verbose_TextCoord(MD3TextCoord textCoord);
void MD3Verbose_Vertex(MD3Vertex vertex);

#endif