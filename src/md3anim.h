#ifndef ANIMATION_H
#define ANIMATION_H

#include <stdio.h>
#include <stdlib.h>

#include "types.h"

#define LINE_BUFFER_SIZE 128

typedef struct MD3Anim {
	char name[16];
	ushort firstFrame, numFrames, loopingFrames, fps;
} MD3Anim;

typedef struct MD3Anims {
	uint n_anims;
	MD3Anim *anims;
} MD3Anims;

bool MD3Anim_Load(const char *file, MD3Anims *anims);
bool MD3Anim_PushAnim(MD3Anims *anims, MD3Anim anim);
void MD3Anim_FreeAnims(MD3Anims *anims);

#endif