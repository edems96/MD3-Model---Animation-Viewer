#ifndef MD3VIEWER_H
#define MD3VIEWER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <SDL2/SDL.h>

#include <GL/GL.h>
#include <GL/Glu.h>

#include "openglut/openglut.h"

#include "types.h"
#include "config.h"
#include "utils.h"

#include "md3structs.h"
#include "md3anim.h"

#define DEF_TITLE "MD3 Model Viewer 1.0"
#define DEF_OUTPUT_BUFFER_SIZE 512

SDL_Window *mWindow;
SDL_GLContext mGLContext;

Config *mConfig;

MD3Model *mModel;
MD3Anims *mAnims;

vector mPosition, mRotate;

uint mFrame, mAnim, mFPS, mFPSFrames;

Uint32 mLastTime;

float mScreenRatio;

bool mRunning, 
	 mTexture,
	 mTriangles;
	 
bool mAutoRotateX, mAutoRotateY, mAutoRotateZ;

bool MD3Viewer_Init(Config *config);
bool MD3Viewer_InitOpenGL();

void MD3Viewer_SetModel(MD3Model *model);
void MD3Viewer_SetAnims(MD3Anims *anims);

void MD3Viewer_Switchto3D();
void MD3Viewer_Switchto2D();

void MD3Viewer_Start();

void MD3Viewer_Update();
void MD3Viewer_Draw();
void MD3Viewer_Events();

void MD3Viewer_DrawModel();
void MD3Viewer_DrawText(const uchar str[], int x, int y);
void MD3Viewer_DrawFPS();
void MD3Viewer_DrawInfo();
void MD3Viewer_DrawButtons();

void MD3Viewer_OnKeyDown(SDL_Keycode keyCode);
void MD3Viewer_OnMouseButtonDown(SDL_MouseButtonEvent event);
void MD3Viewer_OnMouseWheel(MouseWheel direction);

void MD3Viewer_Quit();

void MD3Viewer_Printf(const char *fmt, ...);
void MD3Viewer_Error(const char *fmt, ...);

bool MD3Viewer_Screenshot(const char *filename);

void MD3Viewer_NextAnim();
void MD3Viewer_PreviousAnim();
void MD3Viewer_NextFrame();
uint MD3Viewer_GetValidFrame();

#endif