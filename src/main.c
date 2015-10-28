#include "main.h"

int main(int argc, char *args[]) {
	MD3Model model;
	model.scale = DEF_MODEL_SCLAE;
	model.fps	= DEF_FPS;
	
	MD3Anims anims;
	anims.n_anims 	= 0;
	anims.anims 	= NULL;
	
	uint i;
	char *file = NULL, *animFile = NULL;
	
	ushort 
		screen_width 	= DEF_SCREEN_WIDTH, 
		screen_height 	= DEF_SCREEN_HEIGHT;
		
	bool 
		fullscreen 		= DEF_FULLSCREEN, 
		redirect2File 	= DEF_REDIRECT2FILE, 
		debug 			= DEF_DEBUG,
		info			= DEF_INFO;
		
	if( argc == 1 ) {
		fprintf(stderr, "main: Too few arguments!\n");
		printUsage(args[0]);
		return 1;
	}
	if( argc == 2 ) {
		file = args[1];
	} else {
		for(i = 1; i < argc; i++) {
			
			if( args[i][0] != '-' )
				continue;
			
			switch( args[i][1] ) {
				case 'h': printUsage(args[0]); return 0;
				case 'W': screen_width	= atoi(args[++i]); break;
				case 'H': screen_height	= atoi(args[++i]); break;
				case 's': model.scale	= atof(args[++i]); break;
				case 'F': fullscreen 	= true; break;
				case 'r': redirect2File = true; break;
				case 'd': debug			= true; break;
				case 'i': info			= true; break; 
				case 'a': animFile 		= args[++i]; break;
				case 'f': 
				
					if( strncmp(args[i], "-f", 2) == 0 )
						file = args[++i]; 
					else if( strncmp(args[i], "-fps", 4) == 0 )
						model.fps = atoi(args[++i]);
					
					break;
			}
		}
	}
	
	if( redirect2File ) {
		freopen("stdout.txt", "w+", stdout);
		freopen("stderr.txt", "w+", stderr);
	}
	
	model.path = Utils_GetPath(file);
	
	if( MD3Viewer_Init(screen_width, screen_height, fullscreen) ) {
		if( file != NULL && MD3Loader_Load(&model, file) ) {
			
			debug = false;
			if( debug ) {
				uint j;
				
				MD3Loader_PrintHeaderInfo(model.header);
				
				for(i = 0; i < model.header.num_frames; i++) {
					MD3Loader_PrintFrameInfo(model.frames[i]);
				}
				
				for(i = 0; i < model.header.num_tags; i++) {
					MD3Loader_PrintTagInfo(model.tags[i]);
				}
				
				for(i = 0; i < model.header.num_surfaces; i++) {
					MD3Loader_PrintSurfaceInfo(model.surfaces[i]);
					
					for(j = 0; j < model.surfaces[i].num_shaders; j++) {
						MD3Loader_PrintShaderInfo(model.surfaces[i].shaders[j]);
					}
					
					for(j = 0; j < model.surfaces[i].num_triangles; j++) {
						MD3Loader_PrintTriangleInfo(model.surfaces[i].triangles[j]);
					}
					
					for(j = 0; j < model.surfaces[i].num_vertices; j++) {
						MD3Loader_PrintTextCoordInfo(model.surfaces[i].textCoords[j]);
					}
					
					for(j = 0; j < model.surfaces[i].num_vertices; j++) {
						MD3Loader_PrintVertexInfo(model.surfaces[i].vertices[j]);
					}
				} 
			}
			
			if( !info ) {
				MD3Viewer_SetModel(&model);
				
				if( animFile != NULL && MD3Anim_Load(animFile, &anims) ) {
					MD3Viewer_SetAnims(&anims);
					printf("animFile: %s\n", animFile);
					printf("anims: %u\n", anims.n_anims);
					
					uint k;
					for(k = 0; k < anims.n_anims; k++) {
						printf("%u: %s\n", k, anims.anims[k].name);
					}
				} else
					printf("failed to load animFile: %s\n", animFile);
			
				MD3Viewer_Start();
				MD3Viewer_Quit();
			}
			
			MD3Loader_FreeModel(&model);
			if( animFile != NULL )
				MD3Anim_FreeAnims(&anims);
			
		} else
			printf("Failed to load modell!\n");
	} else
		printf("Failed to init viewer!\n");
	
		
	if( redirect2File ) {
		fclose(stdout);
		fclose(stderr);
	}
	
	return 0;
}

void printUsage(char *program) {
	printf("Usage: %s FILE.md3\n", program);
	printf("Usage: %s [OPTIONS] -f FILE.md3", program);
	printf("Options:\n");
	printf("\t-f <file>.md3\t| Set model's file\n");
	printf("\t-a <file.cfg\t| Set model's animation file\n");
	printf("\t-W <width>\t| Set screen's width\n");
	printf("\t-H <height>\t| Set screen's height\n");
	printf("\t-F\t\t| Fullscreen\n");
	printf("\t-s <scale>\t| Set model's scale\n");
	printf("\t-fps <fps>\t| Set FPS (animation may change it)\n");
	printf("\t-r\t\t| Redirect standard outputs, errors to file\n");
	printf("\t-d\t\t| Enable debug mode\n");
	printf("\t-i\t\t| Enable info mode\n");
	printf("\t-h\t\t| Print usage / this\n\n");
}