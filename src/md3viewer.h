#ifndef MD3VIEWER_H
#define MD3VIEWER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>

#include <GL/GL.h>
#include <GL/Glu.h>

#include "openglut/openglut.h"

#include "types.h"

#include "md3structs.h"
#include "md3anim.h"

#define DEF_TITLE "MD3 Model Viewer 1.0"
#define DEF_OUTPUT_BUFFER_SIZE 512

SDL_Window *mWindow;
SDL_GLContext mGLContext;

MD3Model *mModel;
MD3Anims *mAnims;

vector mPosition, mRotate;

uint mFrame, mAnim;

bool mRunning, 
	 mTexture,
	 mTriangles;

bool MD3Viewer_Init(ushort width, ushort height, bool fullscreen);
bool MD3Viewer_InitOpenGL(ushort width, ushort height);

void MD3Viewer_SetModel(MD3Model *model);
void MD3Viewer_SetAnims(MD3Anims *anims);

void MD3Viewer_Start();

void MD3Viewer_Draw();
void MD3Viewer_Events();

void MD3Viewer_DrawModel();

void MD3Viewer_OnKeyDown(SDL_Keycode keyCode);

void MD3Viewer_Quit();

void MD3Viewer_Printf(const char *fmt, ...);
void MD3Viewer_Error(const char *fmt, ...);

bool MD3Viewer_Screenshot(const char *filename);

void MD3Viewer_NextAnim();
void MD3Viewer_NextFrame();
uint MD3Viewer_GetValidFrame();

#endif