#ifndef THREE_D_H
#define THREE_D_H
#include "tri.h"
#include <math.h>

typedef struct {
	num x, y, z;
} vec3_t;

#define PRINT_VEC3(_x) \
	printf("vec3_t %f %f %f\n", _x.x, _x.y, _x.z)

typedef struct {
	vec3_t a, b, c;
} tri3_t;

#define PRINT_TRI3(_x) \
	printf("tri3_t\n"); \
	PRINT_VEC3(_x.a); \
	PRINT_VEC3(_x.b); \
	PRINT_VEC3(_x.c)

typedef struct {
	num m[4][4];
} mat_t;

#define PRINT_MAT(_x) \
	printf("mat_t\n"); \
	for(int i = 0; i < 4; i++) { \
		for(int j = 0; j < 4; j++) { \
			printf("%f ", (_x).m[i][j]); \
		} \
		\
		printf("\n"); \
	}

#define PI 3.14159
#define M(_x, _y, _z) \
	i._x * mat->m[_y][_z]

vec3_t mul_mat(
	vec3_t i,
	mat_t *mat
) {
	vec3_t result;
	result.x = M(x, 0, 0) + M(y, 1, 0) + M(z, 2, 0) + mat->m[3][0];
	result.y = M(x, 0, 1) + M(y, 1, 1) + M(z, 2, 1) + mat->m[3][1];
	result.z = M(x, 0, 2) + M(y, 1, 2) + M(z, 2, 2) + mat->m[3][2];
	num w = M(x, 0, 3) + M(y, 1, 3) + M(z, 2, 3) + mat->m[3][3];
	if(w != 0.0) {
		result.x /= w;
		result.y /= w;
		result.z /= w;
	}

	return result;
}

#undef M
void clean_mat(mat_t *mat) {
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			mat->m[i][j] = 0.0;
		}
	}
}

mat_t make_mat_proj() {
	mat_t result;
	clean_mat(&result);
	
	// https://replit.com/@Arabica/3DEngine?v=1#script.js
	num near = 1;
	num far = 100.0;
	num fov = 90.0;
	num fov_rad = 1.0 / tanf(fov * 0.5 / 180.0 * PI);
	num aspect_ratio = ((num)(SCREEN_HEIGHT) / (num)(SCREEN_WIDTH)) * fov_rad;
	num q = far / (far - near);
	result.m[0][0] = aspect_ratio * fov_rad;
	result.m[1][1] = fov_rad;
	result.m[2][2] = q;
	result.m[3][2] = -near * q;
	result.m[2][3] = 1.0;
	return result;
}

num deg(num x) {
	num result =  (x * PI) / 180.0;
	return result;
}

mat_t mat_rot_x(num theta) {
	mat_t result;
	clean_mat(&result);
	theta *= 0.5;
	result.m[0][0] = 1.0;
	result.m[1][1] = cos(theta);
	result.m[1][2] = sin(theta);
	result.m[2][1] = -sin(theta);
	result.m[2][2] = cos(theta);
	result.m[3][3] = 1.0;
	return result;
}

mat_t mat_rot_y(num theta) {
	mat_t result;
	clean_mat(&result);
	result.m[0][0] = cos(theta);
	result.m[0][2] = sin(theta);
	result.m[2][0] = -sin(theta);
	result.m[1][1] = 1.0;
	result.m[2][2] = cos(theta);
	result.m[3][3] = 1;
	return result;
}

mat_t mat_rot_z(num theta) {
	mat_t result;
	clean_mat(&result);
	result.m[0][0] = cos(theta);
	result.m[0][1] = sin(theta);
	result.m[1][0] = -sin(theta);
	result.m[1][1] = cos(theta);
	result.m[2][2] = 1.0;
	result.m[3][3] = 1.0;
	return result;
}

int is_tri_drawable(tri3_t tri) {
	if(
		tri.a.z <= 0 ||
		tri.b.z <= 0 ||
		tri.c.z <= 0		
	) {
		return 0;
	}

	// https://replit.com/@Arabica/3DEngine?v=1#script.js
	vec3_t l1 = {
		tri.b.x - tri.a.x,
		tri.b.y - tri.a.y,
		tri.b.z - tri.a.z
	};
	
	vec3_t l2 = {
		tri.c.x - tri.a.x,
		tri.c.y - tri.a.y,
		tri.c.z - tri.a.z
	};
	
	vec3_t n = {
		(l1.y * l2.z) - (l1.z * l2.y),
		(l1.z * l2.x) - (l1.x * l2.z),
		(l1.x * l2.y) - (l1.y * l2.x)
	};
	
	num l = sqrt(
		n.x * n.x +
		n.y * n.y +
		n.z * n.z
	);
	
	n.x /= l;
	n.y /= l;
	n.z /= l;
	return (
		n.x * tri.a.x +
		n.y * tri.a.y +
		n.z * tri.a.z
	) < 0.0;
}

// https://github.com/OneLoneCoder/Javidx9/blob/master/ConsoleGameEngine/BiggerProjects/Engine3D/OneLoneCoder_olcEngine3D_Part3.cpp
num vec3_dot(vec3_t a, vec3_t b) {
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

num vec3_length(vec3_t a) {
	return sqrt(vec3_dot(a, a));
}

vec3_t vec3_normalize(vec3_t a) {
	num l = vec3_length(a);
	vec3_t result = {
		a.x / l,
		a.y / l,
		a.z / l
	};

	return result;
}

typedef struct {
	tri3_t a;
	tri3_t b;
	int tri_count;
} clip_t;


num vec3_plane_dist(vec3_t p, vec3_t plane_p, vec3_t plane_n) {
	p = vec3_normalize(p);
	return vec3_dot(plane_n, p) - vec3_dot(plane_n, plane_p);
}

#define TRI_PTR_INDEX(_x, _y, _z) \
	switch(_x) { \
		case 0: \
			_z = &_y.a; \
			break; \
		\
		case 1: \
			_z = &_y.b; \
			break; \
		\
		case 2: \
			_z = &_y.c; \
			break; \
		\
	}

#define ARRAY_SIZE(_x) \
	((int)(sizeof(_x) / sizeof((_x)[0])))

vec3_t vec3_add(vec3_t a, vec3_t b) {
	vec3_t result =  {
		a.x + b.x,
		a.y + b.y,
		a.z + b.z
	};

	return result;
}

vec3_t vec3_sub(vec3_t a, vec3_t b) {
	vec3_t result =  {
		a.x - b.x,
		a.y - b.y,
		a.z - b.z
	};

	return result;
}

vec3_t vec3_mul(vec3_t a, vec3_t b) {
	vec3_t result =  {
		a.x * b.x,
		a.y * b.y,
		a.z * b.z
	};

	return result;
}

vec3_t vec3_intersect_plane(
	vec3_t plane_p, 
	vec3_t plane_n, 
	vec3_t start, 
	vec3_t end
) {
	plane_n = vec3_normalize(plane_n);
	num plane_d = -vec3_dot(plane_n, plane_p);
	num ad = vec3_dot(start, plane_n);
	num bd = vec3_dot(end, plane_n);
	num t = (-plane_d - ad) / (bd - ad);
	vec3_t start_to_end = vec3_sub(end, start);
	vec3_t t_vec = { t, t, t };
	vec3_t intersect = vec3_mul(start_to_end, t_vec);
	return vec3_add(start, intersect);
}

clip_t tri3_clip(
	vec3_t plane_p, 
	vec3_t plane_n, 
	tri3_t tri
) {
	plane_n = vec3_normalize(plane_n);
	vec3_t inside[3];
	int inside_size = 0;
	vec3_t outside[3];
	int outside_size = 0;
	num d[] = {
		vec3_plane_dist(tri.a, plane_p, plane_n),
		vec3_plane_dist(tri.b, plane_p, plane_n),
		vec3_plane_dist(tri.c, plane_p, plane_n)
	};

	// this is probaly gonna segfault.
	for(int i = 0; i < ARRAY_SIZE(d); i++) {
		vec3_t *xside = d[i] >= 0 ? inside : outside;
		int *xside_size = d[i] >= 0 ? &inside_size : &outside_size;
		vec3_t *tri_ptr;
		TRI_PTR_INDEX(i, tri, tri_ptr);
		xside[(*xside_size)++] = *tri_ptr;
	}

	clip_t result;
	if(inside_size == 0) {
		result.tri_count = 0;
		return result;
	}

	if(inside_size == 3) {
		result.a = tri;
		result.tri_count = 1;
		return result;
	}

	if(inside_size == 1 && outside_size == 2) {
		result.a.a = inside[0];
		result.a.b = vec3_intersect_plane(plane_p, plane_n, inside[0], outside[0]);
		result.a.c = vec3_intersect_plane(plane_p, plane_n, inside[0], outside[1]);
		result.tri_count = 1;
		return result;
	}

	result.a.a = inside[0];
	result.a.b = inside[1];
	result.a.c = vec3_intersect_plane(plane_p, plane_n, inside[0], outside[0]);
	result.b.a = inside[1];
	result.b.b = result.a.c;
	result.b.c = vec3_intersect_plane(plane_p, plane_n, inside[1], outside[0]);
	result.tri_count = 2;
	return result;
}

tri3_t tri3_proj(tri3_t tri, mat_t *proj) {
	tri3_t proj_tri;
	proj_tri.a = mul_mat(tri.a, proj);
	proj_tri.b = mul_mat(tri.b, proj);
	proj_tri.c = mul_mat(tri.c, proj);
	proj_tri.a.x++; proj_tri.a.y++;
	proj_tri.b.x++; proj_tri.b.y++;
	proj_tri.c.x++; proj_tri.c.y++;
	proj_tri.a.x *= 0.5 * SCREEN_WIDTH; proj_tri.a.y *= 0.5 * SCREEN_HEIGHT;
	proj_tri.b.x *= 0.5 * SCREEN_WIDTH; proj_tri.b.y *= 0.5 * SCREEN_HEIGHT;
	proj_tri.c.x *= 0.5 * SCREEN_WIDTH; proj_tri.c.y *= 0.5 * SCREEN_HEIGHT;
	return proj_tri;
}

tri2_t tri3_to_tri2(tri3_t tri) {
	tri2_t result = {
		.a = EXPAND_VEC2(tri.a),
		.b = EXPAND_VEC2(tri.b),
		.c = EXPAND_VEC2(tri.c),	
	};

	return result;
}

#endif
