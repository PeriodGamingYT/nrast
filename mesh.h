#ifndef __MESH_H
#define __MESH_H
#include "light.h"

typedef struct {
	tri3_t *orig_tris;
	tri3_t *tris;
	int tris_size;
} mesh_t;

#define MESH_CUBE_LENGTH 12
#define TRI3_PART(_a, _b, _c, _d, _e, _f, _g, _h, _i) \
	{ { _a, _b, _c, 1.0 }, { _d, _e, _f, 1.0 }, { _g, _h, _i, 1.0 }, 1.0 }

void mesh_clean_slate(mesh_t *mesh) {
	for(int i = 0; i < mesh->tris_size; i++) {
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
	result.tris_size = MESH_CUBE_LENGTH;
	for(int i = 0; i < result.tris_size; i++) {
		result.orig_tris[i] = tris[i];
	}

	mesh_clean_slate(&result);
	return result;
}

#define CLIP_PREP_ASSIGN(_a, _b, _c, _d, _e, _f) \
	plane_p.x = _a; \
	plane_p.y = _b; \
	plane_p.z = _c; \
	plane_n.x = _d; \
	plane_n.y = _e; \
	plane_n.z = _f

#define TRI3_MAT_MUL(_x, _y) \
	_x.a = mul_mat(_x.a, _y); \
	_x.b = mul_mat(_x.b, _y); \
	_x.c = mul_mat(_x.c, _y)	


num depth_buffer[SCREEN_WIDTH * SCREEN_HEIGHT] = { 0 };
void mesh_bake(
	mesh_t *mesh,
	light_t **lights,
	int lights_size
) {
	for(int i = 0; i < mesh->tris_size; i++) {
		vec3_t normal = tri3_normal(mesh->tris[i]);
		mesh->tris[i].intensity = compute_lighting(
			normal,
			lights,
			lights_size
		);
	}
}

void mesh_draw(
	mesh_t *mesh, 
	mat_t *proj, 
	unsigned int color
) {
	tri3_t *drawable_tris = (tri3_t *) malloc(sizeof(tri3_t));
	int drawable_tris_size = 0;
	for(int i = 0; i < mesh->tris_size; i++) {
		tri3_t tri = mesh->tris[i];
		if(!is_tri_drawable(tri)) {
			continue;
		}

		vec3_t plane_p;
		vec3_t plane_n;
		CLIP_PREP_ASSIGN(0, 0, 0.1, 0, 0, 1);
		clip_t clipped = tri3_clip(plane_p, plane_n, tri);
		clipped.a = tri3_proj(clipped.a, proj);
		clipped.b = tri3_proj(clipped.b, proj);
		drawable_tris_size += clipped.tri_count;
		drawable_tris = (tri3_t *) realloc(
			drawable_tris,
			sizeof(tri3_t) * drawable_tris_size
		);
		
		switch(clipped.tri_count) {
			case 0:
				break;

			case 1:
				drawable_tris[drawable_tris_size - 1] = clipped.a;
				break;

			case 2:
				drawable_tris[drawable_tris_size - 2] = clipped.a;
				drawable_tris[drawable_tris_size - 1] = clipped.b;
				break;
		}
	}

	tri3_t *drawn_tris = (tri3_t *) malloc(0);
	tri3_t no_tri = TRI3_PART(0, 0, 0, 0, 0, 0, 0, 0, 0);
	int drawn_tris_size = 0;
	for(int i = 0; i < drawable_tris_size; i++) {
		drawn_tris_size++;
		drawn_tris = (tri3_t *) realloc(
			drawn_tris,
			sizeof(tri3_t) * drawn_tris_size
		);

		drawn_tris[drawn_tris_size - 1] = drawable_tris[i];
		int new_tris = 1;
		for(int p = 0; p < 4; p++) {
			while(new_tris > 0) {
				tri3_t test = drawn_tris[drawn_tris_size - 1];
				drawn_tris[drawn_tris_size - 1] = no_tri;
				drawn_tris_size--;
				drawn_tris = (tri3_t *) realloc(
					drawn_tris,
					sizeof(tri3_t) * drawn_tris_size
				);

				new_tris--;
				vec3_t plane_p;
				vec3_t plane_n;
				switch(p) {
					case 0:
						CLIP_PREP_ASSIGN(0, 0, 0, 0, 1, 0);
						break;

					case 1:
						CLIP_PREP_ASSIGN(0, (num) SCREEN_HEIGHT - 1, 0, 0, -1, 0);
						break;

					case 2:
						CLIP_PREP_ASSIGN(0, 0, 0, 1, 0, 0);
						break;

					case 3:
						CLIP_PREP_ASSIGN((num) SCREEN_WIDTH - 1, 0, 0, -1, 0, 0);
						break;
				}

				clip_t clipped = tri3_clip(plane_p, plane_n, test);
				drawn_tris_size += clipped.tri_count;
				drawn_tris = (tri3_t *) realloc(
					drawn_tris,
					sizeof(tri3_t) * drawn_tris_size
				);
				
				switch(clipped.tri_count) {
					case 0:
						break;

					case 1:
						drawn_tris[drawn_tris_size - 1] =  clipped.a;
						break;

					case 2:
						drawn_tris[drawn_tris_size - 2] = clipped.a;
						drawn_tris[drawn_tris_size - 1] = clipped.b;
						break;
				}
			}

			new_tris = drawn_tris_size;
		}
	}
	
	for(int i = 0; i < drawn_tris_size; i++) {

		// Short for Light Intensity. Named this to make intensity_vec short.
		num li = drawn_tris[i].intensity;
		vec3_t intensity_vec = { li, li, li, 1 };
		tri2_t draw_tri = tri3_to_tri2(drawn_tris[i]);
		vec3_t color_vec = vec3_mul(
			intensity_vec, 
			int_to_vec3(color)
		);
		
		tri_draw(
			draw_tri, 
			depth_buffer, 
			drawn_tris[i].a.w, 
			rgb(
				(unsigned char) color_vec.x,
				(unsigned char) color_vec.y,
				(unsigned char) color_vec.z
			)
		);
	}

	free(drawable_tris);
	free(drawn_tris);
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
	mat_t trans_mat = mat_trans(trans);
	for(int i = 0; i < mesh->tris_size; i++) {
		TRI3_MAT_MUL(mesh->tris[i], &trans_mat);
	}
}

void mesh_scale(mesh_t *mesh, vec3_t scale) {
	for(int i = 0; i < mesh->tris_size; i++) {
		TRI3_OP_PERFORM(mesh->tris[i], * scale);
	}
}

void mesh_rot(mesh_t *mesh, vec3_t rot) {
	mat_t rot_x = mat_rot_x(rot.x);
	mat_t rot_y = mat_rot_y(rot.y);
	mat_t rot_z = mat_rot_z(rot.z);
	for(int i = 0; i < mesh->tris_size; i++) {
		TRI3_MAT_MUL(mesh->tris[i], &rot_x);
		TRI3_MAT_MUL(mesh->tris[i], &rot_y);
		TRI3_MAT_MUL(mesh->tris[i], &rot_z);
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
