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

void bottom_flat_tri_draw(tri2_t tri, unsigned int color) {
	num inv_slope_1 = (tri.b.x - tri.a.x) / (tri.b.y - tri.a.y);
	num inv_slope_2 = (tri.c.x - tri.a.x) / (tri.c.y - tri.a.y);
	num cur_x_1 = tri.a.x;
	num cur_x_2 = tri.a.x;
	for(int y = tri.a.y; y <= tri.b.y; y++) {
		horiz_line(
			y,
			(int) cur_x_1,
			(int) cur_x_2 + 1,
			color
		);

		cur_x_1 += inv_slope_1;
		cur_x_2 += inv_slope_2;
	}
}

void top_flat_tri_draw(tri2_t tri, unsigned int color) {
	num inv_slope_1 = (tri.a.x - tri.b.x) / (tri.a.y - tri.b.y);
	num inv_slope_2 = (tri.a.x - tri.c.x) / (tri.a.y - tri.c.y);
	num cur_x_1 = tri.a.x;
	num cur_x_2 = tri.a.x;
	for(int y = tri.a.y; y >= tri.b.y; y--) {
		horiz_line(
			y,
			(int) cur_x_1,
			(int) cur_x_2 + 1,
			color
		);

		cur_x_1 -= inv_slope_1;
		cur_x_2 -= inv_slope_2;
	}
}

void tri_draw(tri2_t bad_tri, unsigned int color) {
	tri2_t tri = correct_tri(bad_tri);
	if(tri.b.y == tri.c.y) {
		bottom_flat_tri_draw(tri, color);
		return;
	}

	if(tri.a.y > tri.b.y) {
		top_flat_tri_draw(tri, color);
		return;
	}

	vec2_t mid_point = {
		tri.a.x + (int)( 
			(
				(num)(tri.b.y - tri.a.y) / 
				(num)(tri.c.y - tri.a.y)
			) * (tri.c.x - tri.a.x)
		),
		tri.b.y
	};

	tri2_t bottom_flat = {
		tri.a,
		tri.b,
		mid_point
	};

	bottom_flat_tri_draw(bottom_flat, color);
	tri2_t top_flat = {
		tri.c,
		tri.b,
		mid_point
	};

	top_flat_tri_draw(top_flat, color);
}

#endif
