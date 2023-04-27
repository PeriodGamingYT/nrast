#ifndef __SCENE_H
#define __SCENE_H
#include "mesh.h"
typedef struct {
	mesh_t *mesh;
	vec3_t pos;
	vec3_t rot;
	vec3_t scale;
	unsigned int color;
} obj_t;

obj_t make_obj(mesh_t *mesh) {
	vec3_t zero = { 0, 0, 0, 1 };
	vec3_t one = { 1, 1, 1, 1 };
	obj_t result = {
		mesh,
		zero,
		zero,
		one,
		rgb(255, 255, 255)
	};

	return result;
}

typedef struct {
	char **args;
	int arg_size;
} args_t;

args_t split_string(char *bad_string, char *delim) {
	args_t result = {
		NULL,
		0
	};

	// copied because gcc makes bad_string read-only, resulting in a 
	// segfault, this circumvents this problem.
	char *string = (char *) malloc(sizeof(char) * strlen(bad_string));
	memcpy(string, bad_string, sizeof(char) * strlen(bad_string));
	char *token = strtok(string, delim);
	while(token != NULL && token[0] != '\0') {
		int token_size = strlen(token);
		char *copy_token = (char *) malloc(sizeof(char) * (token_size + 1));
		memcpy(copy_token, token, sizeof(char) * token_size);
		copy_token[token_size] = '\0';
		result.arg_size++;
		result.args = (char **) realloc(
			result.args,
			result.arg_size * sizeof(char *)
		);

		result.args[result.arg_size - 1] = copy_token;
		token = strtok(NULL, delim);
	}

	free(string);
	return result;
}

int int_exponet(int x) {
	int exponet = 1;
	while(exponet < x) {
		exponet *= 10;
	}

	return exponet;
}

int string_to_int_len(char *string) {
	int i = 0;
	for(; string[i] >= '0' && string[i] <= '9'; i++);
	return i;
}

int string_to_int(char *string) {
	int result = 0;
	int exponet = pow(10, string_to_int_len(string) - 1);
	for(
		int i = 0; 
		string[i] >= '0' && 
		string[i] <= '9' &&
		exponet > 0; 
		i++
	) {
		result += (string[i] - '0') * exponet;
		exponet /= 10;
	}

	return result;
}

// this does give kinda inaccurate numbers, though this is the hardwares fault.
// to be more specific, IEEE-754s standard of floating point numbers kinda sucks
// at having accurate floating point numbers. So it's IEEs fault.
num string_to_num(char *string) {
	int is_negative = 0;
	if(string[0] == '-') {
		string++;
		is_negative = 1;
	}

	int split_point = 0;
	for(int i = 0; string[i] != 0; i++) {
		if(string[i] == '.') {
			split_point = i;
			break;
		}
	}

	if(split_point == 0) {
		return string_to_int(string) * bool_to_pos_neg(is_negative);
	}

	num big = string_to_int(string);
	num small = string_to_int(&string[split_point + 1]);
	return bool_to_pos_neg(!is_negative) * (big + (small / int_exponet(small)));
}

void free_args(args_t *args) {
	for(int i = 0; i < args->arg_size; i++) {
		free(args->args[i]);
	}

	free(args->args);
}

typedef struct {
	vec3_t *verts;
	int verts_size;
} obj_state_t;

void free_obj_state(obj_state_t *obj_state) {
	free(obj_state->verts);
}

typedef void (*obj_handler_t)(args_t *, obj_state_t *, mesh_t *);
typedef struct {
	char *prefix;
	obj_handler_t callback;
} obj_callback_t;

void obj_callback_v(args_t *args, obj_state_t *obj_state, mesh_t *mesh) {
	obj_state->verts_size++;
	obj_state->verts = (vec3_t *) realloc(
		obj_state->verts,
		obj_state->verts_size * sizeof(vec3_t)
	);

	vec3_t vert = {
		string_to_num(args->args[1]),
		string_to_num(args->args[2]),
		string_to_num(args->args[3]),
		1
	};
	
	obj_state->verts[obj_state->verts_size - 1] = vert;
}

#define EXPAND_VEC3(_x) \
	{ _x.x, _x.y, _x.z, _x.w }

void obj_callback_f(args_t *args, obj_state_t *obj_state, mesh_t *mesh) {
	int f[] = {
		string_to_int(args->args[1]),
		string_to_int(args->args[2]),
		string_to_int(args->args[3])
	};

	mesh->tris_size++;
	mesh->orig_tris = (tri3_t *) realloc(
		mesh->orig_tris,
		mesh->tris_size * sizeof(tri3_t)
	);

	mesh->tris = (tri3_t *) realloc(
		mesh->tris,
		mesh->tris_size * sizeof(tri3_t)
	);

	tri3_t tri = {
		EXPAND_VEC3(obj_state->verts[f[0] - 1]),
		EXPAND_VEC3(obj_state->verts[f[1] - 1]),
		EXPAND_VEC3(obj_state->verts[f[2] - 1]),
		0
	};

	mesh->orig_tris[mesh->tris_size - 1] = tri;	
}


mesh_t load_obj(char *file_name) {
	FILE *obj_file = fopen(file_name, "r");
	if(obj_file == NULL) {
		fprintf(stderr, "can't load .obj file\n");
		exit(1);
	}

	mesh_t mesh = {
		NULL,
		NULL,
		0
	};
	
	obj_callback_t obj_callbacks[] = {
		{ "v", obj_callback_v },
		{ "f", obj_callback_f }	
	};

	obj_state_t obj_state = {
		NULL,
		0
	};
	
	while(!feof(obj_file)) {
		char *curr_line = NULL;
		size_t line_size = 0;
		getline(&curr_line, &line_size, obj_file);
		if(curr_line[0] == 0) {
			break;
		}
		
		args_t args = split_string(curr_line, " ");
		for(int i = 0; i < ARRAY_SIZE(obj_callbacks); i++) {
			if(strcmp(args.args[0], obj_callbacks[i].prefix) == 0) {
				obj_callbacks[i].callback(&args, &obj_state, &mesh);
			}
		}

		free_args(&args);
	}

	free_obj_state(&obj_state);
	fclose(obj_file);
	return mesh;
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
	vec3_t camera_rot,
	light_t **lights,
	int lights_size
) {
	mesh_clean_slate(obj->mesh);
	mesh_rot(obj->mesh, obj->rot);
	mesh_scale(obj->mesh, obj->scale);
	mesh_trans(obj->mesh, obj->pos);
	mesh_bake(
		obj->mesh,
		lights,
		lights_size
	);

	mesh_trans(obj->mesh, camera_pos);
	mesh_rot(obj->mesh, camera_rot);
	mesh_draw(
		obj->mesh, 
		proj,
		obj->color
	);
}

typedef struct {
	obj_t **objs;
	int objs_size;
	vec3_t camera_pos;
	vec3_t camera_rot;
	mat_t proj;
	light_t **lights;
	int lights_size;
} scene_t;

scene_t make_scene() {
	vec3_t zero = { 0, 0, 0, 1 };
	scene_t result = {
		NULL,
		0,
		zero,
		zero,
		make_mat_proj(),
		NULL,
		0
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

void add_scene_light(scene_t *scene, light_t *light) {
	scene->lights_size++;
	scene->lights = (light_t **) realloc(
		scene->lights,
		scene->lights_size * sizeof(light_t *)
	);

	scene->lights[scene->lights_size - 1] = light;
}

void free_scene(scene_t *scene) {
	for(int i = 0; i < scene->objs_size; i++) {
		free_obj(scene->objs[i]);
	}

	free(scene->objs);
	free(scene->lights);
}

void scene_draw(scene_t *scene) {
	for(int i = 0; i < scene->objs_size; i++) {
		obj_draw(
			scene->objs[i], 
			&scene->proj, 
			scene->camera_pos,
			scene->camera_rot,
			scene->lights,
			scene->lights_size
		);
	}
}

#endif
