#include "camera.h"

void Camera_Rotate(SDL_Window *window) {
	int w, h, x, y;
	
	SDL_GetWindowSize(window, &w, &h);
	SDL_GetMouseState(&x, &y);
	SDL_WarpMouseInWindow(window, w / 2 , h / 2);
	
	mPitch 	-= ((w / 2.0f) - x) * MOUSE_SPEED;
	mYaw 	-= ((h / 2.0f) - y) * MOUSE_SPEED; 
	
	glRotatef(mPitch, 	0.0f, 1.0f, 0.0f);
	glRotatef(mYaw,		1.0f, 0.0f, 0.0f);
}

void Camera_Move() {
	const Uint8 *keyState = SDL_GetKeyboardState(NULL);
	
	if( keyState[SDL_SCANCODE_W] )
		Camera_MoveInDirection(DIR_FORWARD);
	else if( keyState[SDL_SCANCODE_S] )
		Camera_MoveInDirection(DIR_BACKWARD);
		
	if( keyState[SDL_SCANCODE_A] )
		Camera_MoveInDirection(DIR_LEFT);
	else if( keyState[SDL_SCANCODE_D] )
		Camera_MoveInDirection(DIR_RIGHT);
	
	glTranslatef(mPosition.x, mPosition.y, mPosition.z);
}

void Camera_MoveInDirection(CameraDirection direction) {
	printf("move dir: %d\n", direction);
	mPosition.z += cos((mYaw + direction) * 180.0f / M_PI) * CAMERA_SPEED;
	mPosition.x += sin((mPitch + direction) * 180.0f / M_PI) * CAMERA_SPEED;
}