#include "main.h"

int main(int argc, char *args[]) {
	Config config;
	Config_Init(&config);
	
	uint i;
	char *file = NULL, *animFile = NULL;
		
	if( argc == 1 ) {
		fprintf(stderr, "Too few arguments!\n");
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
				case 'W': config.screen_width	= atoi(args[++i]); break;
				case 'H': config.screen_height	= atoi(args[++i]); break;
				case 's': config.model_scale	= atof(args[++i]); break;
				case 'F': config.fullscreen 	= true; break;
				case 'r': config.redirect2File 	= true; break;
				case 'd': config.debug			= true; break;
				case 'i': config.info			= true; break; 
				case 'c': config.configFile		= args[++i]; break;
				case 'a': animFile 				= args[++i]; break;
				case 'f': 
				
					if( strncmp(args[i], "-f", 2) == 0 )
						file = args[++i]; 
					else if( strncmp(args[i], "-fps", 4) == 0 )
						config.fps = atoi(args[++i]);
					
					break;
			}
		}
	}
	
	Config_Load(&config, config.configFile);
	Config_Check(&config);
	Config_Print(config);
	
	MD3Model model;
	model.scale = config.model_scale;
	model.fps	= config.fps;
	
	MD3Anims anims;
	anims.n_anims 	= 0;
	anims.anims 	= NULL;
	
	if( config.redirect2File ) {
		if( config.stdoutFile )
			freopen(config.stdoutFile, "w+", stdout);
		
		if( config.stderrFile )
			freopen(config.stderrFile, "w+", stderr);
	}
	
	model.path = Utils_GetPath(file);
	
	if( MD3Viewer_Init(&config) ) {
		if( file != NULL && MD3Loader_Load(&model, file) ) {
			
			if( config.debug ) {
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
			
			if( !config.info ) {
				MD3Viewer_SetModel(&model);
				
				if( animFile != NULL ) {
					
					if( MD3Anim_Load(animFile, &anims) )
						MD3Viewer_SetAnims(&anims);
					else
						fprintf(stderr, "Failed to load animation file: '%s'\n", animFile);
				} 
					
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
	
		
	if( config.redirect2File ) {
		if( config.stdoutFile )
			fclose(stdout);
		
		if( config.stderrFile )
			fclose(stderr);
	}
	
	return 0;
}

void printUsage(char *program) {
	printf("Usage: %s FILE.md3\n", program);
	printf("Usage: %s [OPTIONS] -f FILE.md3", program);
	printf("Options:\n");
	printf("\t-f <file>.md3\t| Set model's file\n");
	printf("\t-a <file.cfg>\t| Set model's animation file\n");
	printf("\t-c <file.cfg>\t| Load configuration file\n");
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