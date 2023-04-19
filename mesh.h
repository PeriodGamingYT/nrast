#ifndef __MESH_H
#define __MESH_H
#include "three_d.h"

typedef struct {
	tri3_t *orig_tris;
	tri3_t *tris;
	int tris_length;
} mesh_t;

#define MESH_CUBE_LENGTH 12
#define TRI3_PART(_a, _b, _c, _d, _e, _f, _g, _h, _i) \
	{ { _a, _b, _c }, { _d, _e, _f }, { _g, _h, _i } }

void mesh_clean_slate(mesh_t *mesh) {
	for(int i = 0; i < mesh->tris_length; i++) {
		mesh->tris[i] = mesh->orig_tris[i];
	}
}

mesh_t make_cube_mesh() {
	mesh_t result;

	// https://github.com/OneLoneCoder/Javidx9/blob/master/ConsoleGameEngine/BiggerProjects/Engine3D/OneLoneCoder_olcEngine3D_Part1.cpp
	tri3_t tris[MESH_CUBE_LENGTH] = {
		TRI3_PART(0, 0, 0, 0, 1, 0, 1, 1, 0),
		TRI3_PART(0, 0, 0, 1, 1, 0, 1, 0, 0),
		TRI3_PART(1, 0, 0, 1, 1, 0, 1, 1, 1),
		TRI3_PART(1, 0, 0, 1, 1, 1, 1, 0, 1),
		TRI3_PART(1, 0, 1, 1, 1, 1, 0, 1, 1),
		TRI3_PART(1, 0, 1, 0, 1, 1, 0, 0, 1),
		TRI3_PART(0, 0, 1, 0, 1, 1, 0, 1, 0),
		TRI3_PART(0, 0, 1, 0, 1, 0, 0, 0, 0),
		TRI3_PART(0, 1, 0, 0, 1, 1, 1, 1, 1),
		TRI3_PART(0, 1, 0, 1, 1, 1, 1, 1, 0),
		TRI3_PART(1, 0, 1, 0, 0, 1, 0, 0, 0),
		TRI3_PART(1, 0, 1, 0, 0, 0, 1, 0, 0)
	};

	result.tris = (tri3_t *) malloc(sizeof(tri3_t) * MESH_CUBE_LENGTH);
	result.orig_tris = (tri3_t *) malloc(sizeof(tri3_t) * MESH_CUBE_LENGTH);
	result.tris_length = MESH_CUBE_LENGTH;
	for(int i = 0; i < result.tris_length; i++) {
		result.orig_tris[i] = tris[i];
	}

	mesh_clean_slate(&result);
	return result;
}

void mesh_draw(
	mesh_t *mesh, 
	mat_t *proj, 
	unsigned int color
) {
	for(int i = 0; i < mesh->tris_length; i++) {
		tri3_t tri = mesh->tris[i];
		if(!is_tri_drawable(tri)) {
			continue;
		}

		tri2_t draw_tri = tri3_proj(tri, proj);
		tri_draw(draw_tri, color);
	}
}

#define VEC3_OP_PERFORM(_x, _y) \
	_x.x = _x.x _y.x; \
	_x.y = _x.y _y.y; \
	_x.z = _x.z _y.z

#define TRI3_OP_PERFORM(_x, _y) \
	VEC3_OP_PERFORM(_x.a, _y); \
	VEC3_OP_PERFORM(_x.b, _y); \
	VEC3_OP_PERFORM(_x.c, _y)

void mesh_trans(mesh_t *mesh, vec3_t trans) {
	for(int i = 0; i < mesh->tris_length; i++) {
		TRI3_OP_PERFORM(mesh->tris[i], + trans);
	}
}

#define TRI3_MAT_MUL(_x, _y) \
	_x.a = mul_mat(_x.a, _y); \
	_x.b = mul_mat(_x.b, _y); \
	_x.c = mul_mat(_x.c, _y)	

void mesh_rot(mesh_t *mesh, vec3_t rot) {
	mat_t rot_x = mat_rot_x(rot.x);
	mat_t rot_y = mat_rot_y(rot.y);
	mat_t rot_z = mat_rot_z(rot.z);
	for(int i = 0; i < mesh->tris_length; i++) {
		TRI3_MAT_MUL(mesh->tris[i], &rot_z);
		TRI3_MAT_MUL(mesh->tris[i], &rot_x);
		TRI3_MAT_MUL(mesh->tris[i], &rot_y);
	}
}

void free_mesh(mesh_t *mesh) {
	if(mesh == NULL) {
		return;
	}

	free(mesh->tris);
	free(mesh->orig_tris);
}

#endif
