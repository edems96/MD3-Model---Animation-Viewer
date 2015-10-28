#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>

#include "types.h"
#include "utils.h"
#include "md3structs.h"
#include "md3loader.h"
#include "md3viewer.h"

#include "md3anim.h" // delete my later!

#define DEF_SCREEN_WIDTH 800
#define DEF_SCREEN_HEIGHT 600
#define DEF_FULLSCREEN false
#define DEF_REDIRECT2FILE false
#define DEF_DEBUG false
#define DEF_INFO false
#define DEF_MODEL_SCLAE 1.0f / 64.0f
#define DEF_FPS 20

int main(int argc, char *args[]);
void printUsage(char *program);

#endif