#include "md3loader.h"

#ifdef DMALLOC
#include "debugmalloc.h"
#endif

bool MD3Loader_Load(MD3Model *model, const char *file) {
	if( model == NULL )
		return false;
	
	FILE *f = fopen(file, "rb");
	
	if( !f ) {
		fprintf(stderr, "MD3Loader_Load: Failed to open file: %s\n!", file);
		fclose(f);
		
		return false;   
	}
	
	if( !MD3Loader_ReadHeader(f, &model->header) ) {
		fprintf(stderr, "MD3Loader_Load: Failed to read MD3 file header!\n");
		fclose(f);
		
		return false;
	}
	
	if( !MD3Loader_ReadFrames(f, model) ) {
		fprintf(stderr, "MD3Loader_Load: Failed to read MD3 file frames!\n");
		fclose(f);
		
		return false;
	}
	
	if( !MD3Loader_ReadTags(f, model) ) {
		fprintf(stderr, "MD3Loader_Load: Failed to read MD3 file tags!\n");
		fclose(f);
		
		return false;
	}
	
	if( !MD3Loader_ReadSurfaces(f, model) ) {
		fprintf(stderr, "MD3Loader_Load: Failed to read MD3 file surfaces!\n");
		return false;
	} 
	
	fclose(f);
	
	return true;
}

bool MD3Loader_ReadHeader(FILE *f, MD3Header *header) {
	if( fread(header, sizeof(MD3Header), 1, f) != 1 )
		return false;
	
	if( header->ident != MD3_IDENT ) {
		fprintf(stderr, "MD3Loader_Load: Wrong file ident! Yours: 0x%X, correct: 0x%X!\n", header->ident, MD3_IDENT);
		return false;
	}
	
	if( header->version != MD3_VERSION ) {
		fprintf(stderr, "MD3Loader_Load: Wrong file version! Yours: %d, correct: %d!\n", header->version, MD3_VERSION);
		return false;
	}
	
	return true;
}

bool MD3Loader_ReadFrames(FILE *f, MD3Model *model) {
	model->frames = (MD3Frame *) malloc(sizeof(MD3Frame) * model->header.num_frames);
	
	if( !model->frames ) {
		fprintf(stderr, "MD3Loader_ReadFrames: Failed to allocate memory for frames!\n");
		return false;
	}
	
	if( fseek(f, model->header.ofs_frames, SEEK_SET) ) {
		fprintf(stderr, "MD3Loader_ReadFrames: Failed to seek in file!\n");
		return false;
	}
	
	return fread(model->frames, sizeof(MD3Frame), model->header.num_frames, f) == model->header.num_frames;
}

bool MD3Loader_ReadTags(FILE *f, MD3Model *model) {
	model->tags = (MD3Tag *) malloc(sizeof(MD3Tag) * model->header.num_tags * model->header.num_frames);
	
	if( !model->tags ) {
		fprintf(stderr, "MD3Loader_ReadTags: Failed to allocate memory for tags!\n");
		return false;
	}
	
	if( fseek(f, model->header.ofs_tags, SEEK_SET) ) {
		fprintf(stderr, "MD3Loader_ReadTags: Failed to seek in file!\n");
		return false;
	}
	
	return fread(model->tags, sizeof(MD3Tag), model->header.num_tags * model->header.num_frames, f) == model->header.num_tags * model->header.num_frames;
}

bool MD3Loader_ReadSurfaces(FILE *f, MD3Model *model) {
	lint s_start;
	uint i;
	
	model->surfaces = (MD3Surface *) malloc(sizeof(MD3Surface) * model->header.num_surfaces);
	
	if( !model->surfaces ) {
		fprintf(stderr, "MD3Loader_ReadSurfaces: Failed to allocate memory for surfaces!\n");
		return false;
	}
	
	if( fseek(f, model->header.ofs_surfaces, SEEK_SET) ) {
		fprintf(stderr, "MD3Loader_ReadSurfaces: Failed to seek in file!\n");
		return false;
	}
	
	for(i = 0; i < model->header.num_surfaces; i++) {
		s_start = ftell(f);
		
		if( fread(&model->surfaces[i], sizeof(MD3Surface) - 16, 1, f) != 1 ) {
			fprintf(stderr, "MD3Loader_ReadSurfaces: Failed to read surface' header!\n");
			return false;
		}
		
		if( model->surfaces[i].ident != MD3_SURFACE_IDENT ) {
			fprintf(stderr, "MD3Loader_ReadSurfaces: Wrong surface's ident! Yours: 0x%X, correct: 0x%X!\n", model->surfaces[i].ident, MD3_SURFACE_IDENT);
			return false;
		}
		
		fseek(f, s_start + model->surfaces[i].ofs_shaders, SEEK_SET);
		
		if( !MD3Loader_ReadShaders(f, &model->surfaces[i]) ) {
			fprintf(stderr, "MD3Loader_ReadSurfaces: Failed to read surface's shaders!\n");
			return false;
		}
		
		if( !MD3Loader_LoadTextures(f, model, &model->surfaces[i]) ) {
			fprintf(stderr, "MD3Loader_ReadSurfaces: Failed to load surface's texures!\n");
			return false;
		}
		
		fseek(f, s_start + model->surfaces[i].ofs_triangles, SEEK_SET);
		
		if( !MD3Loader_ReadTriangles(f, &model->surfaces[i]) ) {
			fprintf(stderr, "MD3Loader_ReadSurfaces: Failed to read surface's triangles!\n");
			return false;
		}
		
		fseek(f, s_start + model->surfaces[i].ofs_textCoords, SEEK_SET);
		
		if( !MD3Loader_ReadTextCoords(f, &model->surfaces[i]) ) {
			fprintf(stderr, "MD3Loader_ReadSurfaces: Failed to read surface's texture coordinates!\n");
			return false;
		}
		
		fseek(f, s_start + model->surfaces[i].ofs_vertices, SEEK_SET);
		
		if( !MD3Loader_ReadVertices(f, &model->surfaces[i]) ) {
			fprintf(stderr, "MD3Loader_ReadSurfaces: Failed to read surface's vertices!\n");
			return false;
		}
	}
	
	return true;
}

bool MD3Loader_ReadShaders(FILE *f, MD3Surface *surface) {
	surface->shaders = (MD3Shader *) malloc(sizeof(MD3Shader) * surface->num_shaders);
	
	if( !surface->shaders ) {
		fprintf(stderr, "MD3Loader_ReadShaders: Failed to allocate memory for shaders!\n");
		return false;
	}
	
	return fread(surface->shaders, sizeof(MD3Shader), surface->num_shaders, f) == surface->num_shaders;
}

bool MD3Loader_ReadTriangles(FILE *f, MD3Surface *surface) {
	surface->triangles = (MD3Triangle *) malloc(sizeof(MD3Triangle) * surface->num_triangles);
	
	if( !surface->triangles ) {
		fprintf(stderr, "MD3Loader_ReadTriangles: Failed to allocate memory for triangles!");
		return false;
	}
	
	return fread(surface->triangles, sizeof(MD3Triangle), surface->num_triangles, f) == surface->num_triangles;
}

bool MD3Loader_ReadTextCoords(FILE *f, MD3Surface *surface) {
	surface->textCoords = (MD3TextCoord *) malloc(sizeof(MD3TextCoord) * surface->num_vertices);
	
	if( !surface->textCoords ) {
		fprintf(stderr, "MD3Loader_ReadTextCoords: Failed to allocate memory for texture coordinates!\n");
		return false;
	}
	
	return fread(surface->textCoords, sizeof(MD3TextCoord), surface->num_vertices, f) == surface->num_vertices;
}

bool MD3Loader_ReadVertices(FILE *f, MD3Surface *surface) {
	surface->vertices = (MD3Vertex *) malloc(sizeof(MD3Vertex) * surface->num_vertices * surface->num_frames);
	
	if( !surface->vertices ) {
		fprintf(stderr, "MD3Loader_ReadVertices: Failed to allocate memory for vertices!\n");
		return false;
	}

	return fread(surface->vertices, sizeof(MD3Vertex), surface->num_vertices * surface->num_frames, f) == surface->num_vertices * surface->num_frames;
}

bool MD3Loader_LoadTextures(FILE *f, MD3Model *model, MD3Surface *surface) {
	const char *path;
	uint i;
	
	surface->textures = (uint *) malloc(sizeof(uint*) * surface->num_shaders);
	
	if( !surface->textures ) {
		fprintf(stderr, "MD3Loader_LoadTextures: Failed to allocate memory for textures!\n");
		return false;
	}
	
	for(i = 0; i < surface->num_shaders; i++) {
		surface->textures[i] = 0; // default value
		
		path = Utils_FileInPath(model->path, Utils_GetFile(surface->shaders[i].name));
		path = Utils_GetFileWithoutExt(path);
		
		if( path ) {
			if( !Texture_Load(&surface->textures[i], Utils_Implode(path, ".jpg")) ) {
				if( !Texture_Load(&surface->textures[i], Utils_Implode(path, ".tga")) ) {
					if( !Texture_Load(&surface->textures[i], Utils_Implode(path, ".bmp")) ) {
						if( !Texture_Load(&surface->textures[i], Utils_Implode(path, ".png")) ) {
							fprintf(stderr, "MD3Loader_LoadTextures: Failed to load texture '%s'!\n", path);
						}
					}
				}
			}
		} else
			fprintf(stderr, "MD3Loader_LoadTextures: Failed to load texture '%s'!\n", path);
		
	}
	
	//free(path);
	
	return true;
}

void MD3Loader_FreeSurface(MD3Surface *surface) {
	free(surface->shaders);
	free(surface->triangles);
	free(surface->textCoords);
	free(surface->vertices);
	free(surface->textures);
}

void MD3Loader_FreeModel(MD3Model *model) {
	uint i;
	
	free(model->frames);
	free(model->tags);
	
	for(i = 0; i < model->header.num_surfaces; i++) {
		MD3Loader_FreeSurface(&model->surfaces[i]);
		//free(model->surfaces[i]);
	}
	
	free(model->surfaces);
	
	//free(model->path);
}