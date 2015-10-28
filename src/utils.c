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