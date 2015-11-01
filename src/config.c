#include "config.h"

void Config_Init(Config *config) {
	config->screen_width 	= DEF_SCREEN_WIDTH;
	config->screen_height 	= DEF_SCREEN_HEIGHT;
	config->fullscreen		= DEF_FULLSCREEN;
	config->fps				= DEF_FPS;
	config->redirect2File 	= DEF_REDIRECT2FILE;
	config->debug			= DEF_DEBUG;
	config->info			= DEF_INFO;
	config->configFile		= DEF_CONFIG_FILE;
	
	config->stdoutFile		= (char *) malloc(sizeof(char) * FILE_BUFFER_SIZE);
	config->stderrFile		= (char *) malloc(sizeof(char) * FILE_BUFFER_SIZE);
	
	if( config->stdoutFile )
		strcpy(config->stdoutFile, DEF_STDOUT_FILE);
	
	if( config->stderrFile )
		strcpy(config->stderrFile, DEF_STDERR_FILE);
}

bool Config_Load(Config *config, const char *filename) {
	if( !config || !filename )
		return false;
	
	FILE *f = fopen(filename, "r");
	
	if( !f ) {
		fprintf(stderr, "Config_Load: Failed to load '%s' config file!\n", filename);
		return false;
	}
	
	char ch, buffer[LINE_BUFFER_SIZE];
	uint n;
	
	while( !feof(f) ) {
		n = 0;
		
		while( !feof(f) && n < LINE_BUFFER_SIZE - 1 ) {
			ch = fgetc(f);

			if( ch == '#' ) {
				while( !feof(f) && ch != '\n' && ch != EOF ) {
					ch = fgetc(f);
				}
				
				break;
			} else if( ch == '\n' || ch == EOF )
				break;
			
			buffer[n++] = ch;
		}
		
		if( n > 0 ) {
			buffer[n] = 0;
			
			if( strncmp(buffer, "screen-width=", 13) == 0 ) {
				sscanf(buffer, "screen-width=%hu", &config->screen_width);
			} else if( strncmp(buffer, "screen-height=", 14) == 0 ) {
				sscanf(buffer, "screen-height=%hu", &config->screen_height);
			} else if( strncmp(buffer, "fps=", 4) == 0 ) {
				sscanf(buffer, "fps=%hu", &config->fps);
			} else if( strncmp(buffer, "model-scale=", 12) == 0 ) {
				sscanf(buffer, "model-scale=%f", &config->model_scale);
			} else if( strncmp(buffer, "fullscreen=", 11) == 0 ) {
				sscanf(buffer, "fullscreen=%u", &config->fullscreen);
			} else if( strncmp(buffer, "redirect-to-file=", 17) == 0 ) {
				sscanf(buffer, "redirect-to-file=%u", &config->redirect2File);
			} else if( strncmp(buffer, "debug=", 6) == 0 ) {
				sscanf(buffer, "debug=%u", &config->debug);
			} else if( strncmp(buffer, "info=", 5) == 0 ) {
				sscanf(buffer, "info=%u", &config->info);
			}else if( strncmp(buffer, "std-out=", 8) == 0 ) {
				config->stdoutFile = (char *) realloc(config->stdoutFile, sizeof(char) * FILE_BUFFER_SIZE);
				
				if( config->stdoutFile ) 
					sscanf(buffer, "std-out=%s", config->stdoutFile);
				
			} else if( strncmp(buffer, "std-err=", 8) == 0 ) {
				config->stderrFile = (char *) realloc(config->stderrFile, sizeof(char) * FILE_BUFFER_SIZE);
				
				if( config->stderrFile )
					sscanf(buffer, "std-err=%s", config->stderrFile);
			}
		}
	}
	
	fclose(f);
	
	return true;
}

void Config_Check(Config *config) {
	if( config->screen_width < MIN_SCREEN_WIDTH || config->screen_height > MAX_SCREEN_WIDTH )
		config->screen_width = DEF_SCREEN_WIDTH;
	
	if( config->screen_height < MIN_SCREEN_HEIGHT || config->screen_height > MAX_SCREEN_HEIGHT )
		config->screen_height = DEF_SCREEN_HEIGHT;
	
	if( config->fps < MIN_FPS || config->fps > MAX_FPS )
		config->fps = DEF_FPS;
	
	if( config->model_scale < MIN_MODEL_SCALE || config->model_scale > MAX_MODEL_SCALE )
		config->model_scale = DEF_MODEL_SCALE;
	
	if( config->redirect2File ) {
		if( config->stdoutFile == NULL || !strlen(config->stdoutFile) ) {
			config->stdoutFile = (char *) realloc(config->stdoutFile, sizeof(char) * strlen(DEF_STDOUT_FILE));
			
			if( config->stdoutFile )
				strcpy(config->stdoutFile, DEF_STDOUT_FILE);
		}

		if( config->stderrFile == NULL || !strlen(config->stderrFile) ) {
			config->stderrFile = (char *) realloc(config->stderrFile, sizeof(char) * strlen(DEF_STDERR_FILE));
			
			if( config->stderrFile )
				strcpy(config->stderrFile, DEF_STDERR_FILE);
		}
	}
}

void Config_Print(Config config) {
	printf("Config:\n");
	printf("\tScreen width: %u\n", 				config.screen_width);
	printf("\tScreen height: %u\n", 			config.screen_height);
	printf("\tFullscreen: %s\n",				config.fullscreen ? "yes" : "no");
	printf("\tFPS: %u\n",						config.fps);
	printf("\tModel scale: %f\n",				config.model_scale);
	printf("\tRedirect output to file: %s\n", 	config.redirect2File ? "yes" : "no");
	printf("\tDebug mode: %s\n",				config.debug ? "yes" : "no");
	printf("\tInfo mode: %s\n",					config.info ? "yes" : "no");
	printf("\tConfig file: %s\n",				config.configFile);
	printf("\tStdOut file: %s\n",				config.stdoutFile == NULL ? "NULL" : config.stdoutFile);
	printf("\tStdErr file: %s\n",				config.stderrFile == NULL ? "NULL" : config.stderrFile);
}