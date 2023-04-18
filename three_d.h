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
	
	// https://github.com/OneLoneCoder/Javidx9/blob/master/ConsoleGameEngine/BiggerProjects/Engine3D/OneLoneCoder_olcEngine3D_Part1.cpp <-- not me lol
	// https://replit.com/@Arabica/3DEngine?v=1#script.js <-- hey that's me!
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
	result.m[1][1] = cosf(theta);
	result.m[1][2] = sinf(theta);
	result.m[2][1] = -sinf(theta);
	result.m[2][2] = cosf(theta);
	result.m[3][3] = 1.0;
	return result;
}

mat_t mat_rot_y(num theta) {
	mat_t result;
	clean_mat(&result);
	result.m[0][0] = cosf(theta);
	result.m[0][2] = sinf(theta);
	result.m[2][0] = -sinf(theta);
	result.m[1][1] = 1.0;
	result.m[2][2] = cosf(theta);
	result.m[3][3] = 1;
	return result;
}

mat_t mat_rot_z(num theta) {
	mat_t result;
	clean_mat(&result);
	result.m[0][0] = cosf(theta);
	result.m[0][1] = sinf(theta);
	result.m[1][0] = -sinf(theta);
	result.m[1][1] = cosf(theta);
	result.m[2][2] = 1.0;
	result.m[3][3] = 1.0;
	return result;
}

int is_tri_drawable(tri3_t tri) {

	// https://replit.com/@Arabica/3DEngine?v=1#script.js <-- me lol.
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

tri2_t tri3_proj(
	tri3_t tri,
	mat_t *proj
) {
	tri3_t proj_tri;
	proj_tri.a = mul_mat(tri.a, proj);
	proj_tri.b = mul_mat(tri.b, proj);
	proj_tri.c = mul_mat(tri.c, proj);
	tri2_t result = {
		.a = EXPAND_VEC2(proj_tri.a),
		.b = EXPAND_VEC2(proj_tri.b),
		.c = EXPAND_VEC2(proj_tri.c),	
	};

	result.a.x++; result.a.y++;
	result.b.x++; result.b.y++;
	result.c.x++; result.c.y++;
	result.a.x *= 0.5 * SCREEN_WIDTH; result.a.y *= 0.5 * SCREEN_HEIGHT;
	result.b.x *= 0.5 * SCREEN_WIDTH; result.b.y *= 0.5 * SCREEN_HEIGHT;
	result.c.x *= 0.5 * SCREEN_WIDTH; result.c.y *= 0.5 * SCREEN_HEIGHT;
	return result;
}

#endif
