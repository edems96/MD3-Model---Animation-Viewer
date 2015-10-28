#ifndef MD3STRUCTS_H
#define MD3STRUCTS_H

#include "types.h"

typedef struct MD3Frame {
	vector min_bounds;
	vector max_bounds;
	vector origin;
	float radius;
	uchar name[16];
} MD3Frame;

typedef struct MD3Tag {
	uchar name[64];
	vector origin;
	vector axis[3];
} MD3Tag;

typedef struct MD3Shader {
	char name[64];
	int index;
} MD3Shader;

typedef struct MD3Triangle {
	int indexes[3];
} MD3Triangle;

typedef struct MD3TextCoord {
	float s, t;
} MD3TextCoord;

typedef struct MD3Vertex {
	svector position;
	uchar normal[2];
} MD3Vertex;

typedef struct MD3Surface {
	int ident;
	uchar name[64];
	int flags;
	
	int num_frames;
	int num_shaders;
	int num_vertices;
	int num_triangles;
	
	int ofs_triangles;
	int ofs_shaders;
	int ofs_textCoords;
	int ofs_vertices;
	int ofs_end;
	
	MD3Shader		*shaders;
	MD3Triangle 	*triangles;
	MD3TextCoord 	*textCoords;
	MD3Vertex		*vertices;
	
	uint 			*textures;
} MD3Surface;

typedef struct MD3Header {
	int ident;
	int version;
	uchar name[64];
	int flags;
	
	// number of ...
	int num_frames;
	int num_tags;
	int num_surfaces;
	int num_skins;
	
	// offset of ...
	int ofs_frames;
	int ofs_tags;
	int ofs_surfaces;
	int ofs_eof;
} MD3Header;

typedef struct MD3Model {
	MD3Header 	header;
	
	MD3Frame 	*frames;
	MD3Tag		*tags;
	MD3Surface 	*surfaces;
	
	float 		scale;
	uint		fps;
	const char	*path;
} MD3Model;

#endif