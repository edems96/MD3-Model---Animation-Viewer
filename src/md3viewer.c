#include "md3viewer.h"

bool MD3Viewer_Init(ushort width, ushort height, bool fullscreen) {
	if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) ) {
		MD3Viewer_Error("Failed to initialize the SDL Library! Error: %s\n", SDL_GetError()); return false;
	}
	
	mWindow = SDL_CreateWindow(
		DEF_TITLE,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | (fullscreen ? SDL_WINDOW_FULLSCREEN : 0)
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
	
	if( !MD3Viewer_InitOpenGL(width, height) ) {
		MD3Viewer_Error("Failed to initialize OpenGL! Error code: 0x%X\n", glGetError()); 
		return false;
	}
	
	mRunning 	= false;
	mPosition 	= (vector){0, 0, 0};
	mRotate 	= (vector){-90, 0, 0};
	mTexture	= true;
	mTriangles 	= true;
	
	mFrame 		= 0;
	mAnim 		= 0;
	
	return true;
}

bool MD3Viewer_InitOpenGL(ushort width, ushort height) {
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f);
	
	glMatrixMode(GL_PROJECTION);
	
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height, 0.1, 1000.0);
	
	glMatrixMode(GL_MODELVIEW);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	
	return !glGetError();
}

void MD3Viewer_SetModel(MD3Model *model) {
	mModel = model;
}

void MD3Viewer_SetAnims(MD3Anims *anims) {
	mAnims = anims;
}

void MD3Viewer_Start() {
	Uint32 time;
	
	mRunning = true;
	
	while( mRunning ) {
		time = SDL_GetTicks();
		
		MD3Viewer_Events();
		MD3Viewer_Draw();
		
		SDL_GL_SwapWindow(mWindow);
		
		if( (1000 / mModel->fps) > (SDL_GetTicks() - time) )
			SDL_Delay((1000 / mModel->fps) - (SDL_GetTicks() - time));
	}
}

void MD3Viewer_Draw() {
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	glTranslatef(mPosition.x, mPosition.y, mPosition.z);
	glRotatef(mRotate.x, 1.0f, 0.0f, 0.0f);
	glRotatef(mRotate.y, 0.0f, 1.0f, 0.0f);
	
	glScalef(mModel->scale, mModel->scale, mModel->scale);

	MD3Viewer_DrawModel();
}

void MD3Viewer_Events() {
	SDL_Event event;
	
    while( SDL_PollEvent(&event) ) {
		
		switch( event.type ) {
			
			case SDL_QUIT: 		mRunning = false; break;
			case SDL_KEYDOWN:	MD3Viewer_OnKeyDown(event.key.keysym.sym); break; 
			/*case SDL_MOUSEBUTTONDOWN: 	OnMouseButtonDown(event); break; */
			
		}
    }
}

void MD3Viewer_DrawModel() {
	uint i, j, k;
	
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
	
	//printf("frame: %u\n", mFrame);
	
	MD3Viewer_NextFrame();
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
		case SDLK_KP_6: 	mRotate.y += 10; break;
		case SDLK_KP_4: 	mRotate.y -= 10; break;
		case SDLK_KP_8: 	mRotate.x += 10; break;
		case SDLK_KP_2: 	mRotate.x -= 10; break;
		
		case SDLK_1: mTexture 	= !mTexture; break;
		case SDLK_2: mTriangles = !mTriangles; break;
		case SDLK_3: MD3Viewer_NextAnim(); break;
		case SDLK_9: MD3Viewer_Screenshot("screenshot.bmp"); break;
	}
}

void MD3Viewer_Quit() {
	SDL_GL_DeleteContext(mGLContext);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

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
	int w, h;
	SDL_GetWindowSize(mWindow, &w, &h);
	
	uchar *pixels = (uchar *) malloc(w*h*4);
	
    glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	
	// TODO: rotate pixels / image
	
    SDL_Surface * surface = SDL_CreateRGBSurfaceFrom(
		pixels, w, h, 
		8*4, w*4, 
		0x0000ff, 0x00ff00, 0xff0000, 0);

    SDL_SaveBMP(surface, filename);

    SDL_FreeSurface(surface);
    free(surface);
	
	return true;
}

void MD3Viewer_NextAnim() {
	if( mAnims != NULL ) {
		mAnim++;
		mAnim = mAnim % mAnims->n_anims;
		
		mFrame = 0;
		
		mModel->fps = mAnims->anims[mAnim].fps;
		
		printf("Currentanim(%u): %s\n", mAnim, mAnims->anims[mAnim].name);
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