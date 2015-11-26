#include "md3viewer.h"

#ifdef DMALLOC
#include "debugmalloc.h"
#endif

bool MD3Viewer_Init(Config *config) {
	if( !config ) {
		MD3Viewer_Error("MD3Viewer_Init: Invalid config!\n");
		return false;
	}
	
	mConfig = config;
	
	if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) ) {
		MD3Viewer_Error("Failed to initialize the SDL Library! Error: %s\n", SDL_GetError()); return false;
	}
	
	mWindow = SDL_CreateWindow(
		DEF_TITLE,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		mConfig->screen_width, mConfig->screen_height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | (mConfig->fullscreen ? SDL_WINDOW_FULLSCREEN : 0)
		);
		
	if( !mWindow ) {
		MD3Viewer_Error("Failed to create SDL Window! Error: %s\n", SDL_GetError()); 
		return false;
	}
	
	mGLContext = SDL_GL_CreateContext(mWindow);
	
	if( !mGLContext ) {
		MD3Viewer_Error("Failed to create GL Context! Error: %s\n", SDL_GetError()); 
		return false;
	}
	
	if( !MD3Viewer_InitOpenGL() ) {
		MD3Viewer_Error("Failed to initialize OpenGL! Error code: 0x%X\n", glGetError()); 
		return false;
	}
	
	mScreenRatio 	= (float)mConfig->screen_width / mConfig->screen_width;
	
	mRunning 		= false;
	mPosition 		= (vector){0, 0, 0};
	mRotate 		= (vector){-90, 0, 0};
	mTexture		= true;
	mTriangles 		= true;
	
	mFrame 			= 0;
	mAnim 			= 0;
	mFPS			= 0;
	mFPSFrames		= 0;
	
	mAutoRotateX	= false;
	mAutoRotateY	= false;
	mAutoRotateZ	= false;
	
	return true;
}

bool MD3Viewer_InitOpenGL() {
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f);
	
	MD3Viewer_Switchto3D();
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	
	return !glGetError();
}

void MD3Viewer_Switchto3D() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(45, mScreenRatio, 0.1, 1000.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void MD3Viewer_Switchto2D() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glOrtho(0, mConfig->screen_width, 0, mConfig->screen_height, -1, 1);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void MD3Viewer_SetModel(MD3Model *model) {
	mModel = model;
}

void MD3Viewer_SetAnims(MD3Anims *anims) {
	mAnims = anims;
}

void MD3Viewer_Start() {
	Uint32 time, now;
	
	mRunning = true;
	
	while( mRunning ) {
		time = SDL_GetTicks();
		
		MD3Viewer_Events();
		MD3Viewer_Update();
		MD3Viewer_Draw();
		
		SDL_GL_SwapWindow(mWindow);
		
		now = SDL_GetTicks();
		if( (1000 / mModel->fps) > (now - time) )
			SDL_Delay((1000 / mModel->fps) - (now - time));
		
		if( now - mLastTime > 1000 ) {
			mFPS 		= mFPSFrames;
			mFPSFrames 	= 0;
			mLastTime 	= now;
		}
		
		mFPSFrames++;
	}
}

void MD3Viewer_Update() {
	if( mAutoRotateX )
		mRotate.x = fmodf(++mRotate.x, 360);

	if( mAutoRotateY )
		mRotate.y = fmodf(++mRotate.y, 360);
	
	if( mAutoRotateZ )
		mRotate.z = fmodf(++mRotate.z, 360);
}

void MD3Viewer_Draw() {
	MD3Viewer_Switchto3D();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	glTranslatef(mPosition.x, mPosition.y, mPosition.z);
	glScalef(mModel->scale, mModel->scale, mModel->scale);
	
	glRotatef(mRotate.x, 1.0f, 0.0f, 0.0f);
	glRotatef(mRotate.y, 0.0f, 1.0f, 0.0f);
	glRotatef(mRotate.z, 0.0f, 0.0f, 1.0f);
	
	MD3Viewer_DrawModel();
	
	#if 1
	MD3Viewer_Switchto2D();
	MD3Viewer_DrawButtons();
		#if 0
		MD3Viewer_DrawFPS();
		//	MD3Viewer_DrawInfo();
		#endif
	#endif
}

void MD3Viewer_Events() {
	SDL_Event event;
	
    while( SDL_PollEvent(&event) ) {
		
		switch( event.type ) {
			
			case SDL_QUIT: 				mRunning = false; break;
			case SDL_KEYDOWN:			MD3Viewer_OnKeyDown(event.key.keysym.sym); break;
			case SDL_MOUSEBUTTONDOWN:	MD3Viewer_OnMouseButtonDown(event.button); break;
			case SDL_MOUSEWHEEL: 		MD3Viewer_OnMouseWheel(event.wheel.y > 0 ? MOUSE_WHEEL_UP : MOUSE_WHEEL_DOWN); break;
			
		}
    }
}

void MD3Viewer_DrawModel() {
	uint i, j, k;
	
	glColor3f(1.0f, 1.0f, 1.0f);
	for(i = 0; i < mModel->header.num_surfaces; i++) {
		MD3Surface *surface = &mModel->surfaces[i];
		
		if( mTexture ) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, surface->textures[0]);
		} else 
			glDisable(GL_TEXTURE_2D);
		
		for(j = 0; j < surface->num_triangles; j++) {
			MD3Triangle *triangle = &surface->triangles[j];
			
			glBegin(mTriangles ? GL_TRIANGLES : GL_LINE_STRIP);
			
			for(k = 0; k < 3; k++) {
				glTexCoord2f(
					surface->textCoords[triangle->indexes[k]].s, 
					surface->textCoords[triangle->indexes[k]].t);
					
				glVertex3f(
					surface->vertices[triangle->indexes[k] + (MD3Viewer_GetValidFrame() * surface->num_vertices)].position.x, 
					surface->vertices[triangle->indexes[k] + (MD3Viewer_GetValidFrame() * surface->num_vertices)].position.y, 
					surface->vertices[triangle->indexes[k] + (MD3Viewer_GetValidFrame() * surface->num_vertices)].position.z);
			}
			
			glEnd();
		}
	}
	
	MD3Viewer_NextFrame();
}

void MD3Viewer_DrawText(const uchar str[], int x, int y) {
	glRasterPos2f(x, y);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, str);
}

void MD3Viewer_DrawFPS() {
	char buffer[8];
	sprintf(buffer, "FPS: %u", mFPS);
	MD3Viewer_DrawText((uchar*)buffer, 20, mConfig->screen_height - 40);
}

void MD3Viewer_DrawInfo() {
	if( mAnims != NULL ) {
		char animText[strlen(mAnims->anims[mAnim].name) + 11];
		sprintf(animText, "Animation: %s", mAnims->anims[mAnim].name);
		MD3Viewer_DrawText((uchar*)animText, 20, mConfig->screen_height - 60);
	}
}

void MD3Viewer_DrawButtons() {
	glColor3f(0.0f, 0.0f, 0.0f);
	
	MD3Viewer_DrawText((uchar *)"1: Enable / Disable texture", 20, mConfig->screen_height - 60);
	MD3Viewer_DrawText((uchar *)"2: Change vertex's drawing mode", 20, mConfig->screen_height - 80);
	MD3Viewer_DrawText((uchar *)"3: Switch to next animation", 20, mConfig->screen_height - 100);
	MD3Viewer_DrawText((uchar *)"9: Take a screenshoot", 20, mConfig->screen_height - 120);
}

void MD3Viewer_OnKeyDown(SDL_Keycode keyCode) {
	switch( keyCode ) {
		case SDLK_F1: 		mRunning = false; break;
		
		case SDLK_w: 		mPosition.z += 10; break;
		case SDLK_s: 		mPosition.z -= 10; break;
		case SDLK_a: 		mPosition.x += 10; break;
		case SDLK_d: 		mPosition.x -= 10; break;
		case SDLK_n: 		mPosition.y += 10; break;
		case SDLK_m: 		mPosition.y -= 10; break;
		
		case SDLK_KP_7: 	mRotate.x += 10; break;
		case SDLK_KP_9: 	mRotate.x -= 10; break;
		case SDLK_KP_4: 	mRotate.y += 10; break;
		case SDLK_KP_6: 	mRotate.y -= 10; break;
		case SDLK_KP_1: 	mRotate.z += 10; break;
		case SDLK_KP_3: 	mRotate.z -= 10; break;
		
		case SDLK_KP_8:		mAutoRotateX = !mAutoRotateX; break;
		case SDLK_KP_5:		mAutoRotateY = !mAutoRotateY; break;
		case SDLK_KP_2:		mAutoRotateZ = !mAutoRotateZ; break;
		
		
		case SDLK_1: mTexture 	= !mTexture; break;
		case SDLK_2: mTriangles = !mTriangles; break;
		case SDLK_3: MD3Viewer_NextAnim(); break;
		case SDLK_9: MD3Viewer_Screenshot(Utils_GetTimedFilename("sc", "bmp")); break;
	}
}

void MD3Viewer_OnMouseButtonDown(SDL_MouseButtonEvent event) {
	if( event.type != SDL_MOUSEBUTTONDOWN )
		return;
	
	if( event.button == SDL_BUTTON_LEFT )
		MD3Viewer_NextAnim();
	else if( event.button == SDL_BUTTON_RIGHT )
		MD3Viewer_PreviousAnim();
}

void MD3Viewer_OnMouseWheel(MouseWheel direction) {
	if( direction == MOUSE_WHEEL_UP )
		mPosition.z += 10;
	else
		mPosition.z -= 10;
}

void MD3Viewer_Quit() {
	SDL_GL_DeleteContext(mGLContext);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

// TODO: use???
void MD3Viewer_Printf(const char *fmt, ...) {
	char buffer[DEF_OUTPUT_BUFFER_SIZE];
	strcpy(buffer, ">> ");
	
	va_list args;
	va_start (args, fmt);
	vsprintf (buffer, fmt, args);
	va_end (args);
}

void MD3Viewer_Error(const char *fmt, ...) {
	char buffer[DEF_OUTPUT_BUFFER_SIZE];
	strcpy(buffer, ">> Error: ");
	
	va_list args;
	va_start (args, fmt);
	vsprintf (buffer, fmt, args);
	va_end (args);
}

bool MD3Viewer_Screenshot(const char *filename) {
	if( !filename ) {
		MD3Viewer_Error("MD3Viewer_Screenshot: Wrong or empty filename!\n");
		return false;
	}
	
	int w, h;
	SDL_GetWindowSize(mWindow, &w, &h);
	
	uchar *pixels = (uchar *) malloc(w * h * 4);
	
	if( !pixels ) {
		MD3Viewer_Error("MD3Viewer_Screenshot: Failed to allocate memory for pixels!\n");
		return false;
	}
	
	uchar *rPixels = (uchar *) malloc(w * h * 4);
	
	if( !rPixels ) {
		free(pixels);
		MD3Viewer_Error("MD3Viewer_Screenshot: Failed to allocate memory for rotated pixels!\n");
		
		return false;
	}
	
    glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	int y;
	for(y = 0; y < h; y++) {
		memcpy((uchar*)rPixels + y * w * 4, (uchar*)pixels + (h - y - 1) * w * 4, w * 4);
	}
	
	free(pixels);
	
    SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(
		rPixels, w, h, 
		8 * 4, w * 4, 
		0x0000ff, 0x00ff00, 0xff0000, 0);
		
	if( surface ) {
		SDL_SaveBMP(surface, filename);
		SDL_FreeSurface(surface);
		
		free(rPixels);
		
		return true;
	} else {
		free(rPixels);
		MD3Viewer_Error("Failed to create screenshot surface!\n");
		
		return false;
	}
}
// TODO: dont play anim if no enoght frame.... 
void MD3Viewer_NextAnim() {
	if( mAnims != NULL ) {
		mAnim++;
		mAnim = mAnim % mAnims->n_anims;
		
		if( mAnims->anims[mAnim].firstFrame + mAnims->anims[mAnim].numFrames <= mModel->header.num_frames ) {
			mFrame = 0;
		
			mModel->fps = mAnims->anims[mAnim].fps;
		
			printf("Current animation(%u): %s\n", mAnim, mAnims->anims[mAnim].name);
		} else
			MD3Viewer_NextAnim();	
	}
}

void MD3Viewer_PreviousAnim() {
	if( mAnims != NULL ) {
		mAnim--;
		mAnim = mAnim % mAnims->n_anims;
		
		if( mAnims->anims[mAnim].firstFrame + mAnims->anims[mAnim].numFrames <= mModel->header.num_frames ) {
			mFrame = 0;
		
			mModel->fps = mAnims->anims[mAnim].fps;
		
			printf("Current animation(%u): %s\n", mAnim, mAnims->anims[mAnim].name);
		} else
			MD3Viewer_PreviousAnim();	
	}
}

void MD3Viewer_NextFrame() {
	if( mAnims != NULL ) {
		mFrame++;
		mFrame = (mFrame % mAnims->anims[mAnim].numFrames);
	}
}

uint MD3Viewer_GetValidFrame() {
	return mAnims == NULL ? 0 : mAnims->anims[mAnim].firstFrame + mFrame;
}