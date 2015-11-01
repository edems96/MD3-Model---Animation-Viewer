#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"

// screen width 
#define DEF_SCREEN_WIDTH 800
#define MIN_SCREEN_WIDTH 200
#define MAX_SCREEN_WIDTH 2560

// screen height
#define DEF_SCREEN_HEIGHT 600
#define MIN_SCREEN_HEIGHT 200
#define MAX_SCREEN_HEIGHT 200

#define DEF_FULLSCREEN false
#define DEF_REDIRECT2FILE false
#define DEF_DEBUG false
#define DEF_INFO false

#define DEF_MODEL_SCALE 1.0f / 64.0f
#define MIN_MODEL_SCALE 0.000001
#define MAX_MODEL_SCALE 10000000

#define DEF_FPS 20
#define MIN_FPS 1
#define MAX_FPS 333

#define DEF_CONFIG_FILE "config.cfg"
#define DEF_STDOUT_FILE "stdout.txt"
#define DEF_STDERR_FILE "stderr.txt"

#define FILE_BUFFER_SIZE 128
#define LINE_BUFFER_SIZE 128

typedef struct Config {
	ushort screen_width;
	ushort screen_height;
	ushort fps;
	
	float model_scale;
	
	bool fullscreen;
	bool redirect2File;
	bool debug;
	bool info;
	
	char *configFile;
	char *stdoutFile;
	char *stderrFile;
} Config;

void Config_Init(Config *config);
bool Config_Load(Config *config, const char *filename);
void Config_Check(Config *config);
void Config_Print(Config config);

#endif