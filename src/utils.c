#include "utils.h"

vector Utils_DecodeVertexNormal(uchar normal[2]) {
	float lat = normal[0] * (2 * M_PI) / 255;
	float lng = normal[1] * (2 * M_PI) / 255;
	
	vector norm = {
			cos(lng) * sin(lat), 
			sin(lng) * sin(lat),
			cos(lat) 
			};
			
	return norm;
}

const char *Utils_Implode(const char *a, const char *b) {
	uint l 		= strlen(a) + strlen(b);
	char *c  	= (char*) malloc(sizeof(char) * (l + 1));
	
	if( !c )
		return NULL;
	
	strcpy(c, a);
	strcat(c, b);
	c[l] = 0;
	
	return c;
}

const char *Utils_FileInPath(const char *path, const char *filename) {
	uint pathLength 	= strlen(path);
	uint newLength 		= pathLength + strlen(filename);
	char *fmt;
	
	if( path[pathLength - 1] != '/' && path[pathLength - 1] != '\\' ) {
		newLength++;
		fmt = "%s/%s";
	} else
		fmt = "%s%s";
	
	char *realPath = (char*) malloc(sizeof(char) * (newLength + 1));
	
	if( !realPath )
		return NULL;
	
	sprintf(realPath, fmt, path, filename);
	realPath[newLength] = 0;
	
	return realPath;
}

const char *Utils_GetPath(const char *fullPath) {
	int i = 0, lastSlash = -1;
	
	while( fullPath[i] != 0 ) {
		if( fullPath[i] == '/' || fullPath[i] == '\\' )
			lastSlash = i;
		
		i++;
	}
	
	if( lastSlash == -1 )
		return NULL;
	
	char *path = (char *) malloc(sizeof(char) * (lastSlash + 2));
	
	if( !path ) 
		return NULL;
	
	strncpy(path, fullPath, lastSlash + 1);
	path[lastSlash + 1] = 0;
	
	return path;
}

const char *Utils_GetFile(const char *fullPath) {
	int i = 0, j, lastSlash = -1;
	
	while( fullPath[i] != 0 ) {
		if( fullPath[i] == '/' || fullPath[i] == '\\' )
			lastSlash = i;
		
		i++;
	}
	
	if( lastSlash == -1 )
		return fullPath;
	
	char *filename = (char *) malloc(sizeof(char) * (i - lastSlash));
	
	if( !filename )
		return NULL;
	
	for(j = 0; j < (i - lastSlash - 1); j++) {
		filename[j] = fullPath[lastSlash + j + 1];
	}
	
	filename[j] = 0;
	
	return filename;
}

const char *Utils_GetFileWithoutExt(const char *filename) {
	int i = 0, k = 0;
	
	while( filename[i] != 0 ) {
		if( filename[i] == '.' )
			k = i;
		
		i++;
	}
	
	if( k == 0 )
		return filename;
	
	char *new = (char *) malloc(sizeof(char) * (k+1));
	
	if( !new )
		return NULL;
	
	strncpy(new, filename, k);
	new[k] = 0;
	
	return new;
}

const char *Utils_GetTimedFilename(const char *prefix, const char *extension) {
	time_t rawTime;
	struct tm *timer;
	
	time(&rawTime);
	timer = localtime(&rawTime);
	
	char *filename = (char *) malloc(sizeof(char) * (strlen(prefix) + strlen(extension) + 14 + 7 + 1));
	
	if( !filename )
		return NULL;
	
	int n = sprintf(
		filename, 
		"%s_%d_%d_%d_%d_%d_%d.%s", 
		prefix,
		timer->tm_year + 1900,
		timer->tm_mon + 1,
		timer->tm_mday,
		timer->tm_hour,
		timer->tm_min,
		timer->tm_sec,
		extension);
	
	filename[n] = 0;
		
	return filename;
}