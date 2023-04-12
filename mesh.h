#ifndef __MESH_H
#define __MESH_H
#include "three_d.h"

typedef struct {
	tri3_t *tris;
	int tris_length;
} mesh_t;

#define MESH_CUBE_LENGTH 12

// goofy and ugly ahh macro
#define TRI3_PART(_a, _b, _c, _d, _e, _f, _g, _h, _i) \
	{ { _a, _b, _c }, { _d, _e, _f }, { _g, _h, _i } }

mesh_t make_cube_mesh() {
	mesh_t result;

	// https://github.com/OneLoneCoder/Javidx9/blob/master/ConsoleGameEngine/BiggerProjects/Engine3D/OneLoneCoder_olcEngine3D_Part1.cpp
	tri3_t tris[MESH_CUBE_LENGTH] = {
		TRI3_PART(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f),
		TRI3_PART(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f),
		TRI3_PART(1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f),
		TRI3_PART(1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f),
		TRI3_PART(1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f),
		TRI3_PART(1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f),
		TRI3_PART(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f),
		TRI3_PART(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
		TRI3_PART(0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),
		TRI3_PART(0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f),
		TRI3_PART(1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),
		TRI3_PART(1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f)
	};

	result.tris = tris;
	result.tris_length = MESH_CUBE_LENGTH;
	return result;
}

void mesh_draw(
	mesh_t *mesh, 
	mat_t *proj, 
	unsigned int color
) {
	for(int i = 0; i < mesh->tris_length; i++) {
		tri2_t draw_tri = tri3_proj(mesh->tris[i], proj);
		tri_draw(draw_tri, color);
	}
}

#endif
