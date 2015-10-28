#include "texture.h"

bool Texture_Load(uint *id, const char *file) {
	SDL_Surface *surface, *image;
	
	surface = IMG_Load(file);
	
	if( !surface ) {
		fprintf(stderr, "Texture_Load: Failed to load image: '%s'!\n", file); 
		return false;
	}
		
	SDL_PixelFormat format;
	format.format = SDL_PIXELFORMAT_RGBA8888;
	format.palette = NULL;
	format.BitsPerPixel = 32;
	format.BytesPerPixel = 4;
	format.Rmask = 0xff000000;
	format.Gmask = 0x00ff0000;
	format.Bmask = 0x0000ff00;
	format.Amask = 0x000000ff;
	format.Rloss = 0;
	format.Gloss = 0;
	format.Bloss = 0;
	format.Aloss = 0;
	format.Rshift = 0;
	format.Gshift = 0;
	format.Bshift = 0;
	format.Ashift = 0;
	
	image = SDL_ConvertSurface(surface, &format, 0);

	if( image == NULL ) {
		fprintf(stderr, "Texture_Load: Failed to convert image: '%s'!\n", file);
		SDL_FreeSurface(surface);
		
		return false;
	}

	glGenTextures(1, id);
    glBindTexture(GL_TEXTURE_2D, *id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, image->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	SDL_FreeSurface(image);
	SDL_FreeSurface(surface);
	
	return true;
}