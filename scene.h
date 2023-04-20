#ifndef __SCENE_H
#define __SCENE_H
#include "light.h"
typedef struct {
	mesh_t *mesh;
	vec3_t pos;
	vec3_t rot;
	vec3_t scale;
	unsigned int color;
} obj_t;

obj_t make_obj(mesh_t *mesh) {
	vec3_t zero = { 0, 0, 0 };
	vec3_t one = { 1, 1, 1 };
	obj_t result = {
		mesh,
		zero,
		zero,
		one,
		rgb(255, 255, 255)
	};

	return result;
}

void free_obj(obj_t *obj) {
	if(obj->mesh != NULL) {
		free_mesh(obj->mesh);
		obj->mesh = NULL;
	}
}

void obj_draw(
	obj_t *obj, 
	mat_t *proj, 
	vec3_t camera_pos,
	vec3_t camera_rot
) {
	mesh_clean_slate(obj->mesh);
	mesh_rot(obj->mesh, obj->rot);
	mesh_scale(obj->mesh, obj->scale);
	mesh_trans(obj->mesh, obj->pos);
	mesh_trans(obj->mesh, camera_pos);
	mesh_rot(obj->mesh, camera_rot);
	mesh_draw(obj->mesh, proj, obj->color);
}

typedef struct {
	obj_t **objs;
	int objs_size;
	vec3_t camera_pos;
	vec3_t camera_rot;
	mat_t proj;

	// TODO: Lighting data in scene.
} scene_t;

scene_t make_scene() {
	vec3_t zero = { 0, 0, 0 };
	scene_t result = {
		NULL,
		0,
		zero,
		zero,
		make_mat_proj()
	};

	return result;
}

void add_scene_obj(scene_t *scene, obj_t *obj) {
	scene->objs_size++;
	scene->objs = (obj_t **) realloc(
		scene->objs,
		scene->objs_size * sizeof(obj_t *)
	);

	scene->objs[scene->objs_size - 1] = obj;
}

void free_scene(scene_t *scene) {
	if(scene->objs != NULL) {
		for(int i = 0; i < scene->objs_size; i++) {
			if(scene->objs[i] == NULL) {
				continue;
			}
			
			free_obj(scene->objs[i]);
			scene->objs[i] = NULL;
		}

		free(scene->objs);
		scene->objs = NULL;
	}
}

void scene_draw(scene_t *scene) {
	for(int i = 0; i < scene->objs_size; i++) {
		obj_draw(
			scene->objs[i], 
			&scene->proj, 
			scene->camera_pos,
			scene->camera_rot
		);
	}
}

#endif
