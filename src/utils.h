#ifndef UTILS_H
#define UTILS_H

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"

#ifndef M_PI
#define M_PI 3.14159265359
#endif

vector Utils_DecodeVertexNormal(uchar normal[2]);

const char *Utils_Implode(const char *a, const char *b);
const char *Utils_FileInPath(const char *path, const char *filename);
const char *Utils_GetPath(const char *fullPath);
const char *Utils_GetFile(const char *fullPath);
const char *Utils_GetFileWithoutExt(const char *filename);
const char *Utils_GetTimedFilename(const char *prefix, const char *extension);

#endif