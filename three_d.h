#ifndef THREE_D_H
#define	THREE_D_H
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
	if(w != 0) {
		result.x /= w;
		result.y /= w;
		result.z /= w;		
	}

	return result;
}

#undef M
#define M result.m
mat_t make_mat_proj() {
	mat_t result;

	// might be redundant, not too sure though.
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			result.m[i][j] = 0;
		}
	}

	// https://github.com/OneLoneCoder/Javidx9/blob/master/ConsoleGameEngine/BiggerProjects/Engine3D/OneLoneCoder_olcEngine3D_Part1.cpp
	float fNear = 0.5f;
	float fFar = 1000.0f;
	float fFov = 90.0f;
	float fAspectRatio = (float) SCREEN_HEIGHT / (float) SCREEN_WIDTH;
	float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);
	M [0][0] = fAspectRatio * fFovRad;
	M [1][1] = fFovRad;
	M [2][2] = fFar / (fFar - fNear);
	M [3][2] = (-fFar * fNear) / (fFar - fNear);
	M [2][3] = 1.0f;
	M [3][3] = 0.0f;
	return result;
}

#undef M
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

	// this code is an eye-sore.
	result.a.x++; result.a.y++;
	result.b.x++; result.b.y++;
	result.c.x++; result.c.y++;
	result.a.x *= 0.5 * SCREEN_WIDTH; result.a.y *= 0.5 * SCREEN_HEIGHT;
	result.b.x *= 0.5 * SCREEN_WIDTH; result.b.y *= 0.5 * SCREEN_HEIGHT;
	result.c.x *= 0.5 * SCREEN_WIDTH; result.c.y *= 0.5 * SCREEN_HEIGHT;
	return result;
}

#endif
