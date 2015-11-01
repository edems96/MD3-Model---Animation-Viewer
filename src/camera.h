#ifndef CAMERA_H
#define CAMERA_H

#include <stdio.h> // TODO: delete me
#include <math.h>

#include <SDL2/SDL.h>

#include <GL/GL.h>

#include "types.h"

#define CAMERA_SPEED 10
#define MOUSE_SPEED 2

typedef enum CameraDirection {DIR_FORWARD=0, DIR_BACKWARD=180, DIR_LEFT=90, DIR_RIGHT=270} CameraDirection;

vector mPosition;

float mPitch, mYaw;

void Camera_Rotate(SDL_Window *window);
void Camera_Move();
void Camera_MoveInDirection(CameraDirection direction);

#endif