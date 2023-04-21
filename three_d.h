#ifndef THREE_D_H
#define THREE_D_H
#include "tri.h"
#include <math.h>

typedef struct {
	num x, y, z, w;
} vec3_t;

#define PRINT_VEC3(_x) \
	printf("vec3_t %f %f %f\n", _x.x, _x.y, _x.z)

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
		a.z / l,
		1
	};

	return result;
}

vec3_t vec3_add(vec3_t a, vec3_t b) {
	vec3_t result =  {
		a.x + b.x,
		a.y + b.y,
		a.z + b.z,
		1
	};

	return result;
}

vec3_t vec3_sub(vec3_t a, vec3_t b) {
	vec3_t result =  {
		a.x - b.x,
		a.y - b.y,
		a.z - b.z,
		1
	};

	return result;
}

vec3_t vec3_mul(vec3_t a, vec3_t b) {
	vec3_t result =  {
		a.x * b.x,
		a.y * b.y,
		a.z * b.z,
		1
	};

	return result;
}

vec3_t vec3_div(vec3_t a, vec3_t b) {
	vec3_t result =  {
		a.x / b.x,
		a.y / b.y,
		a.z / b.z,
		1
	};

	return result;
}

vec3_t vec3_cross(vec3_t a, vec3_t b) {
	vec3_t result;
	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * a.y - a.y * b.x;
	return result;
}

typedef struct {
	vec3_t a, b, c;
	num intensity;
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
	a._x * mat->m[_y][_z]

vec3_t vec3_div_w(vec3_t a) {
	if(a.w != 0.0) {
		a.x /= a.w;
		a.y /= a.w;
		a.z /= a.w;
	}

	return a;
}

vec3_t mul_mat(vec3_t a, mat_t *mat) {
	vec3_t result;
	result.x = M(x, 0, 0) + M(y, 1, 0) + M(z, 2, 0) + mat->m[3][0];
	result.y = M(x, 0, 1) + M(y, 1, 1) + M(z, 2, 1) + mat->m[3][1];
	result.z = M(x, 0, 2) + M(y, 1, 2) + M(z, 2, 2) + mat->m[3][2];
	result.w = M(x, 0, 3) + M(y, 1, 3) + M(z, 2, 3) + mat->m[3][3];
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

mat_t mat_identity() {
	mat_t result;
	clean_mat(&result);
	result.m[0][0] = 1.0;
	result.m[1][1] = 1.0;
	result.m[2][2] = 1.0;
	result.m[3][3] = 1.0;
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

mat_t mat_trans(vec3_t trans) {
	mat_t result = mat_identity();
	result.m[3][0] = trans.x;
	result.m[3][1] = trans.y;
	result.m[3][2] = trans.z;
	return result;
}

#define M(_x) \
	a.m[x][_x] * b.m[_x][y]

mat_t mat_mul_mat(mat_t a, mat_t b) {
	mat_t result;
	clean_mat(&result);
	for(int y = 0; y < 4; y++) {
		for(int x = 0; x < 4; x++) {
			result.m[y][x] = M(0) + M(1) + M(2) + M(3);
		}
	}

	return result;
}

#undef M
mat_t mat_point_at(vec3_t pos, vec3_t target, vec3_t up) {
	vec3_t new_forward = vec3_sub(target, pos);
	new_forward = vec3_normalize(new_forward);
	num dot = vec3_dot(up, new_forward);
	vec3_t dot_vec = { dot, dot, dot, 0 };
	vec3_t a = vec3_mul(new_forward, dot_vec);
	vec3_t new_up = vec3_sub(up, a);
	new_up = vec3_normalize(new_up);
	vec3_t new_right = vec3_cross(new_up, new_forward);
	mat_t result;
	result.m[0][0] = new_right.x;
	result.m[0][1] = new_right.y;
	result.m[0][2] = new_right.z;
	result.m[0][3] = 0.0;
	result.m[1][0] = new_up.x;
	result.m[1][1] = new_up.y;
	result.m[1][2] = new_up.z;
	result.m[1][3] = 0.0;
	result.m[2][0] = new_forward.x;
	result.m[2][1] = new_forward.y;
	result.m[2][2] = new_forward.z;
	result.m[2][3] = 0.0;
	result.m[3][0] = pos.x;
	result.m[3][1] = pos.y;
	result.m[3][2] = pos.z;
	result.m[3][3] = 1.0;
	return result;
}

mat_t mat_quick_inv(mat_t mat) {
	mat_t result;
	result.m[0][0] = mat.m[0][0];
	result.m[0][1] = mat.m[1][0];
	result.m[0][2] = mat.m[2][0];
	result.m[0][3] = 0.0;
	result.m[1][0] = mat.m[0][1];
	result.m[1][1] = mat.m[1][1];
	result.m[1][2] = mat.m[2][1];
	result.m[1][3] = 0.0;
	result.m[2][0] = mat.m[0][2];
	result.m[2][1] = mat.m[1][2];
	result.m[2][2] = mat.m[2][2];
	result.m[2][3] = 0.0;
	result.m[3][0] = mat.m[0][3];
	result.m[3][1] = mat.m[1][3];
	result.m[3][2] = mat.m[2][3];
	result.m[3][3] = 1.0;
	result.m[3][0] = -(mat.m[3][0] * result.m[0][0] + mat.m[3][1] * result.m[1][0] + mat.m[3][2] * result.m[2][0]);
	result.m[3][1] = -(mat.m[3][0] * result.m[0][1] + mat.m[3][1] * result.m[1][1] + mat.m[3][2] * result.m[2][1]);
	result.m[3][2] = -(mat.m[3][0] * result.m[0][2] + mat.m[3][1] * result.m[1][2] + mat.m[3][2] * result.m[2][2]);
	result.m[3][3] = 1.0;
	return result;
}

vec3_t tri3_normal(tri3_t tri) {

	// https://replit.com/@Arabica/3DEngine?v=1#script.js
	vec3_t l1 = {
		tri.b.x - tri.a.x,
		tri.b.y - tri.a.y,
		tri.b.z - tri.a.z,
		1
	};
	
	vec3_t l2 = {
		tri.c.x - tri.a.x,
		tri.c.y - tri.a.y,
		tri.c.z - tri.a.z,
		1
	};
	
	vec3_t n = {
		(l1.y * l2.z) - (l1.z * l2.y),
		(l1.z * l2.x) - (l1.x * l2.z),
		(l1.x * l2.y) - (l1.y * l2.x),
		1
	};
	
	num l = sqrt(
		n.x * n.x +
		n.y * n.y +
		n.z * n.z
	);
	
	n.x /= l;
	n.y /= l;
	n.z /= l;
	return n;
}

int is_tri_drawable(tri3_t tri) {
	vec3_t n = tri3_normal(tri);

	// https://replit.com/@Arabica/3DEngine?v=1#script.js
	return (
		n.x * tri.a.x +
		n.y * tri.a.y +
		n.z * tri.a.z
	) < 0.0;
}

// https://github.com/OneLoneCoder/Javidx9/blob/master/ConsoleGameEngine/BiggerProjects/Engine3D/OneLoneCoder_olcEngine3D_Part3.cpp
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
	vec3_t t_vec = { t, t, t, 1 };
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

tri3_t tri3_div_w(tri3_t tri) {
	tri3_t result;
	result.a = vec3_div_w(tri.a);
	result.b = vec3_div_w(tri.b);
	result.c = vec3_div_w(tri.c);
	return result;	
}

tri3_t tri3_proj(tri3_t tri, mat_t *proj) {
	tri3_t proj_tri;
	proj_tri.a = mul_mat(tri.a, proj);
	proj_tri.b = mul_mat(tri.b, proj);
	proj_tri.c = mul_mat(tri.c, proj);
	proj_tri = tri3_div_w(proj_tri);
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

vec3_t vec3_rot(vec3_t a, vec3_t rot) {
	mat_t rot_x = mat_rot_x(rot.x);
	mat_t rot_y = mat_rot_y(rot.y);
	mat_t rot_z = mat_rot_z(rot.z);
	vec3_t result = a;
	result = mul_mat(a, &rot_x);
	result = mul_mat(result, &rot_y);
	result = mul_mat(result, &rot_z);
	return result;	
}

vec3_t center_tri(tri3_t tri) {
	vec3_t result = {
		3 / (tri.a.x + tri.b.x + tri.c.x),
		3 / (tri.a.y + tri.b.y + tri.c.y),
		3 / (tri.a.z + tri.b.z + tri.c.z),
		1
	};

	return result;
}

vec3_t int_to_vec3(unsigned int a) {
	vec3_t result = {
		(num)((unsigned char)(a)),
		(num)((unsigned char)(a >> 8)),
		(num)((unsigned char)(a >> 16)),
		1
	};

	return result;
}

#endif
