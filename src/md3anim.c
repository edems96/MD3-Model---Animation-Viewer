#include "md3anim.h"

#ifdef DMALLOC
#include "debugmalloc.h"
#endif

bool MD3Anim_Load(const char *file, MD3Anims *anims) {
	FILE *f = fopen(file, "r");
	
	if( !f ) {
		fprintf(stderr, "MD3Anim_Load: Failed to open animation's configure file: '%s'!\n", file);
		return false;
	}
	
	while( !feof(f) ) {
		char buffer[LINE_BUFFER_SIZE];
		int n = 0;
		
		while( !feof(f) && n < LINE_BUFFER_SIZE - 1 ) {
			char ch = fgetc(f);
			
			if( ch == '\n' || ch == EOF )
				break;

			buffer[n++] = ch;
		}
		
		if( n > 0 ) {
			buffer[n] = 0;

			MD3Anim anim;
			int r = sscanf(
				buffer,
				"%hu\t%hu\t%hu\t%hu\t\t// %s",
				&anim.firstFrame,
				&anim.numFrames,
				&anim.loopingFrames, 
				&anim.fps,
				anim.name);
				
			if( r == 5 ) 
				MD3Anim_PushAnim(anims, anim);
		}
	}
	
	fclose(f);
	
	return true;
}

bool MD3Anim_PushAnim(MD3Anims *anims, MD3Anim anim) {
	if( !anims )
		return false;
	
	anims->n_anims++;
	
	if( anims->anims == NULL )
		anims->anims = (MD3Anim *) malloc(sizeof(MD3Anim) * anims->n_anims);
	else
		anims->anims = (MD3Anim *) realloc(anims->anims, sizeof(MD3Anim) * anims->n_anims);
	
	if( !anims->anims )
		return false;
	
	anims->anims[anims->n_anims - 1] = anim;
	
	return true;
}

void MD3Anim_FreeAnims(MD3Anims *anims) {
	/*uint i;
	
	if( anims != NULL ) {
		for(i = 0; i < anims->n_anims; i++) {
			free(&anims->anims[i]);
		}
	} */
	
	free(anims->anims);
}