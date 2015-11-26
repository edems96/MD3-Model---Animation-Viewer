#include "main.h"

#ifdef DMALLOC
#include "debugmalloc.h"
#endif

int main(int argc, char *args[]) {
	Config config;
	
	Config_Init(&config);
	
	if( !Config_FromArgs(&config, argc, args) )
		return 1;
	
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
	
	model.path = Utils_GetPath(config.modelFile);
	
	if( MD3Viewer_Init(&config) ) {
		if( config.modelFile != NULL && MD3Loader_Load(&model, config.modelFile) ) {
			
			if( config.debug ) {
				MD3Verbose_PrintModelInfo(&model);
			}
			
			if( !config.info ) {
				MD3Viewer_SetModel(&model);
				
				if( config.animFile != NULL ) {
					
					if( MD3Anim_Load(config.animFile, &anims) )
						MD3Viewer_SetAnims(&anims);
					else
						fprintf(stderr, "Failed to load animation file: '%s'\n", config.animFile);
				} 
				
				#ifdef DMALLOC
					debugmalloc_naplofajl("memlog.txt");
					debugmalloc_dump();
				#endif
					
				MD3Viewer_Start();
				MD3Viewer_Quit();
			}
			
			MD3Loader_FreeModel(&model);
			if( config.animFile != NULL )
				MD3Anim_FreeAnims(&anims);
			
		} else
			fprintf(stderr, "Failed to load modell!\n");
	} else
		fprintf(stderr, "Failed to init viewer!\n");
	
		
	if( config.redirect2File ) {
		if( config.stdoutFile )
			fclose(stdout);
		
		if( config.stderrFile )
			fclose(stderr);
	}
	
	return 0;
}