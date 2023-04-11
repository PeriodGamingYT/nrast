#ifndef __TRI_H
#define __TRI_H
#include "pixel.h"
typedef float num;
#define PRINT_NUM(x) \
	printf("%f\n", x)

#define PRINT_INT(x) \
	printf("%d\n", x)

// todo, rewrite code AAAAA
typedef struct {
	num x, y;
} vec2_t;

#define PRINT_VEC2(_x) \
	printf("vec2_t %f %f\n", _x.x, _x.y);

#define EXPAND_VEC2(_x) \
	{ _x.x, _x.y }

int bool_to_pos_neg(int x) {
	return (x * 2) - 1;
}

num slope_vec(vec2_t a, vec2_t b) {
	if(a.y > b.y) {
		vec2_t temp = a;
		a = b;
		b = temp;
	}
	
	return (b.y - a.y) / (b.x - a.x);
}

// bad code TODO
vec2_t point_at_y(vec2_t a, vec2_t b, num y) {
	if(a.y > b.y) {
		vec2_t temp = a;
		a = b;
		b = temp;
	}
	
	num slope = slope_vec(a, b);
	num x = (y - a.y) / slope;
	vec2_t result = {
		a.x + (slope * x),
		y
	};

	return result;
}

void pixel_set_vec2(vec2_t x, unsigned int color) {
	pixel_set((int) x.x, (int) x.y, color);
}

void horiz_line(
	num start_y,
	num start_x,
	num stop_x,
	unsigned int color
) {
	if(start_x > stop_x) {
		num temp = start_x;
		start_x = stop_x;
		stop_x = temp;
	}
	
	for(int i = (int) start_x; i < (int) stop_x; i++) {
		pixel_set(i, (int) start_y, color);
	}
}

#define ABS(x) \
	((x) < 0 ? (-(x)) : (x))

typedef struct {
	vec2_t a, b, c;

// weird place to put a comment, lol.
// TRIangle 2D Type
} tri2_t;

#define PRINT_TRI2(x) \
	printf("tri2_t\n"); \
	PRINT_VEC2(x.a); \
	PRINT_VEC2(x.b); \
	PRINT_VEC2(x.c);

#define SWAP_TRI2(_x, _y, _z) \
	vec2_t temp = _x._y; \
	_x._y = _x._z; \
	_x._z = temp;

// probaly a better way to do this, being honest.
// don't know what though, so it will stay like this 
// for a little bit.
#define NEED_SWAP_TRI2(_x) \
	(_x) ? !is_upside_down : is_upside_down

tri2_t correct_tri(tri2_t tri) {

	// dealing with an edge case. warning: this is code 
	// is vomit inducing. you have been warned.
	int is_upside_down = 0;
	if(
		(tri.c.y == tri.b.y && tri.a.y > tri.b.y) ||
		(tri.c.y == tri.a.y && tri.b.y > tri.a.y) ||
		(tri.b.y == tri.a.y && tri.c.y > tri.a.y) 
	) {
		is_upside_down = 1;
	}
	
	if(NEED_SWAP_TRI2(tri.a.y > tri.b.y)) {
		SWAP_TRI2(tri, a, b);
	}

	if(NEED_SWAP_TRI2(tri.a.y > tri.c.y)) {
		SWAP_TRI2(tri, a, c);
	}

	if(tri.b.x > tri.c.x) {
		SWAP_TRI2(tri, b, c);
	}

	return tri;
}

#define MIN(x, y) \
	((x) < (y) ? (x) : (y))

#define MAX(x, y) \
	((x) > (y) ? (x) : (y))

#define TRI_REDUCE_COORD(_x, _y, _z) \
	_y(_x.a._z, _y(_x.b._z, _x.c._z))

void pixel_set_tri(tri2_t x) {
	pixel_set_vec2(x.a, rgb_combine(255, 0, 0));
	pixel_set_vec2(x.b, rgb_combine(0, 255, 0));
	pixel_set_vec2(x.c, rgb_combine(0, 0, 255));
}

void tri_draw(tri2_t tri, unsigned int color) {
	if(
		tri.a.y == tri.b.y &&
		tri.b.y == tri.c.y
	) {
		horiz_line(
			tri.a.y,
			TRI_REDUCE_COORD(tri, MIN, x),
			TRI_REDUCE_COORD(tri, MAX, x),
			color
		);

		return;
	}

	tri2_t corrected = correct_tri(tri);
	
}

#endif
