#include "md3verbose.h"

void MD3Verbose_PrintModelInfo(const MD3Model *model) {
	MD3Verbose_Header(&model->header);
	
	uint i;

	for(i = 0; i < model->header.num_frames; i++) {
		MD3Verbose_Frame(model->frames[i]);
	}

	for(i = 0; i < model->header.num_tags; i++) {
		MD3Verbose_Tag(model->tags[i]);
	}

	for(i = 0; i < model->header.num_surfaces; i++) {
		MD3Verbose_PrintSurfaceInfo(&model->surfaces[i]);
	} 
}

void MD3Verbose_PrintSurfaceInfo(const MD3Surface *surface) {
	MD3Verbose_Surface(surface);
	
	uint i;
	
	for(i = 0; i < surface->num_shaders; i++) {
		MD3Verbose_Shader(surface->shaders[i]);
	}
	
	for(i = 0; i < surface->num_triangles; i++) {
		MD3Verbose_Triangle(surface->triangles[i]);
	}

	for(i = 0; i < surface->num_vertices; i++) {
		MD3Verbose_TextCoord(surface->textCoords[i]);
	}

	for(i = 0; i < surface->num_vertices; i++) {
		MD3Verbose_Vertex(surface->vertices[i]);
	}
}

void MD3Verbose_Start() {
	uint i;
	
	for(i = 0; i < 68; i++) {
		putchar('#');
	}
	
	putchar('\n');
}

void MD3Verbose_End() {
	uint i;
	
	for(i = 0; i < 68; i++) {
		putchar('#');
	}
	
	printf("\n\n");
}

void MD3Verbose_Header(const MD3Header *header) {
	MD3Verbose_Start();
	
	printf("MD3 Header:\n");
	printf("\tIdent: 0x%x\n", 						header->ident);
	printf("\tVersion: %d\n", 						header->version);
	printf("\tName: %s\n", 							header->name);
	printf("\tFlags: %d (0x%X)\n", 					header->flags, header->flags);
	printf("\tNumber of frames: %d\n", 				header->num_frames);
	printf("\tNumber of tags: %d\n", 				header->num_tags);
	printf("\tNumber of surfaces: %d\n", 			header->num_surfaces);
	printf("\tNumber of skins: %d\n", 				header->num_skins);
	printf("\tOffset of frames: %d (0x%X)\n", 		header->ofs_frames, header->ofs_frames);
	printf("\tOffset of tags: %d (0x%X)\n", 		header->ofs_tags, header->ofs_tags);
	printf("\tOffset of surfaces: %d (0x%X)\n", 	header->ofs_surfaces, header->ofs_surfaces);
	printf("\tOffset of end-of-file: %d (0x%X)\n", 	header->ofs_eof, header->ofs_eof);
	
	MD3Verbose_End();
}

void MD3Verbose_Frame(MD3Frame frame) {
	MD3Verbose_Start();
	
	printf("MD3 Frame:\n");
	printf("\tMin bounds: %f %f %f\n", 		frame.min_bounds.x, frame.min_bounds.y, frame.min_bounds.z);
	printf("\tMax bounds: %f %f %f\n", 		frame.max_bounds.x, frame.max_bounds.y, frame.max_bounds.z);
	printf("\tLocal origin: %f %f %f\n", 	frame.origin.x, frame.origin.y,frame.origin.z);
	printf("\tRadius: %f\n", 				frame.radius);
	printf("\tName: %s\n\n", 				frame.name);
}

void MD3Verbose_Tag(MD3Tag tag) {
	printf("MD3 Tag:\n");
	printf("\tName: %s\n", 				tag.name);
	printf("\tOrigin: %f %f %f\n", 		tag.origin.x, tag.origin.y, tag.origin.z);
	printf("\tAxis[0]: %f %f %f\n", 	tag.axis[0].x, tag.axis[0].y, tag.axis[0].z);
	printf("\tAxis[1]: %f %f %f\n", 	tag.axis[1].x, tag.axis[1].y, tag.axis[1].z);
	printf("\tAxis[2]: %f %f %f\n\n", 	tag.axis[2].x, tag.axis[2].y, tag.axis[2].z);
}

void MD3Verbose_Surface(const MD3Surface *surface) {
	MD3Verbose_Start();
	
	printf("MD3 Surface:\n");
	printf("\tIdent: 0x%x\n", 								surface->ident);
	printf("\tName: %s\n", 									surface->name);
	printf("\tFlags: %d (0x%X)\n", 							surface->flags, surface->flags);
	printf("\tNumber of frames: %d\n", 						surface->num_frames);
	printf("\tNumber of shaders: %d\n", 					surface->num_shaders);
	printf("\tNumber of vertices: %d\n", 					surface->num_vertices);
	printf("\tNumber of triangles: %d\n", 					surface->num_triangles);
	printf("\tOffset of triangles: %d (0x%X)\n", 			surface->ofs_triangles, surface->ofs_triangles);
	printf("\tOffset of shaders: %d (0x%X)\n", 				surface->ofs_shaders, surface->ofs_shaders);
	printf("\tOffset of texture coordinates: %d (0x%X)\n", 	surface->ofs_textCoords, surface->ofs_textCoords);
	printf("\tOffset of vertices: %d (0x%X)\n", 			surface->ofs_vertices, surface->ofs_vertices);
	printf("\tOffset of end-of-shader: %d (0x%X)\n", 		surface->ofs_end, surface->ofs_end);
	
	MD3Verbose_End();
}

void MD3Verbose_Shader(MD3Shader shader) {
	printf("MD3 Shader:\n");
	printf("\tName: %s\n", shader.name);
	printf("\tIndex: %d\n\n", shader.index);
}

void MD3Verbose_Triangle(MD3Triangle triangle) {
	printf("MD3 Triangle:\n");
	printf("\tIndexes: %d %d %d\n\n", triangle.indexes[0], triangle.indexes[1], triangle.indexes[2]);
}

void MD3Verbose_TextCoord(MD3TextCoord textCoord) {
	printf("MD3 Texture Coordinate:\n");
	printf("\tS: %f\n", textCoord.s);
	printf("\tT: %f\n\n", textCoord.t);
}

void MD3Verbose_Vertex(MD3Vertex vertex) {
	vector normal = Utils_DecodeVertexNormal(vertex.normal);
	
	printf("MD3 Vertex:\n");
	printf("\tCoordinate: %d %d %d\n", vertex.position.x, vertex.position.y, vertex.position.z);
	printf("\tNormal: %u %u\n", vertex.normal[0], vertex.normal[1]);
	printf("\tNormal: %f %f %f\n\n", normal.x, normal.y, normal.z);
}