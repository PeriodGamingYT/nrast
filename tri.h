#ifndef __TRI_H
#define __TRI_H
#include "pixel.h"
typedef float num;
#define PRINT_NUM(_x) \
	printf("%f\n", _x)

#define PRINT_INT(_x) \
	printf("%d\n", _x)

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

void pixel_set_vec2(vec2_t x, unsigned int color) {
	pixel_set((int) x.x, (int) x.y, color);
}

void horiz_line(
	num start_y,
	num start_x,
	num stop_x,
	num *depth_buffer,
	num w,
	unsigned int color
) {
	if(start_x > stop_x) {
		num temp = start_x;
		start_x = stop_x;
		stop_x = temp;
	}

	start_x--;
	stop_x++;	
	for(int i = (int) start_x; i < (int) stop_x; i++) {
		if(w < depth_buffer[((int) start_y * SCREEN_WIDTH) + i]) {
			continue;
		}
		
		pixel_set(i, (int) start_y, color);
		w = depth_buffer[((int) start_y * SCREEN_WIDTH) + i];
	}
}

// https://www.geeksforgeeks.org/bresenhams-line-generation-algorithm/
#define SWAP_XY_VEC2(_x) \
	temp = _x.x; \
	_x.x = _x.y; \
	_x.y = temp

void line_part(
	vec2_t start,
	vec2_t end,
	vec2_t delta,
	num decide,
	unsigned int color
) {
	num pk = 2 * delta.y - delta.x;
	for(num i = 0; i <= delta.x; i++) {
		start.x < end.x ? start.x++ : start.x--;
		vec2_t temp_vec2 = start;
		if(decide != 0) {
			num temp;
			SWAP_XY_VEC2(temp_vec2);
		}
		
		if(pk < 0) {
			pixel_set(temp_vec2.x, temp_vec2.y, color);
			pk += 2 * delta.y;
			continue;
		}

		start.y < end.y ? start.y++ : start.y--;
		pixel_set(temp_vec2.x, temp_vec2.y, color);
		pk += 2 * delta.y - 2 * delta.x;
	}
}

#define ABS(_x) \
	((_x) > 0 ? (_x) : (-(_x)))

void line(
	vec2_t start,
	vec2_t end,
	unsigned int color
) {
	vec2_t delta = {
		ABS(end.x - start.x),
		ABS(end.y - start.y)
	};

	num decide = 0;
	if(delta.x < delta.y) {
		num temp;
		SWAP_XY_VEC2(start);
		SWAP_XY_VEC2(end);
		SWAP_XY_VEC2(delta);
		decide = 1;
	}

	line_part(start, end, delta, decide, color);
}

typedef struct {
	vec2_t a, b, c;
} tri2_t;

#define PRINT_TRI2(_x) \
	printf("tri2_t\n"); \
	PRINT_VEC2(_x.a); \
	PRINT_VEC2(_x.b); \
	PRINT_VEC2(_x.c);

#define SWAP_TRI2(_x, _y, _z) \
	vec2_t temp = _x._y; \
	_x._y = _x._z; \
	_x._z = temp;

#define NEED_SWAP_TRI2(_x) \
	(_x) ? !is_upside_down : is_upside_down

#define MIN(_x, _y) \
	((_x) < (_y) ? (_x) : (_y))

#define MAX(_x, _y) \
	((_x) > (_y) ? (_x) : (_y))

#define CLAMP_WIDTH(_x) \
	MAX(0, MIN(_x, SCREEN_WIDTH + 1))

#define CLAMP_HEIGHT(_x) \
	MAX(0, MIN(_x, SCREEN_HEIGHT + 1))

#define VEC2_OP_PERFORM(_x, _y, _z) \
	_x.x = _y(_x.x); \
	_x.y = _z(_x.y)

#define TRI2_OP_PERFORM(_x, _y, _z) \
	VEC2_OP_PERFORM(_x.a, _y, _z); \
	VEC2_OP_PERFORM(_x.b, _y, _z); \
	VEC2_OP_PERFORM(_x.c, _y, _z)

#define FLOOR_TRI(_x) \
	(num)((int)(_x))

tri2_t correct_tri(tri2_t tri) {
	TRI2_OP_PERFORM(
		tri, 
		CLAMP_WIDTH, 
		CLAMP_HEIGHT
	);
	
	TRI2_OP_PERFORM(
		tri,
		FLOOR_TRI,
		FLOOR_TRI
	);
	
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
	
	if(NEED_SWAP_TRI2(tri.b.y > tri.c.y)) {
		SWAP_TRI2(tri, b, c);
	}

	return tri;
}

void pixel_set_tri(tri2_t x) {
	pixel_set_vec2(x.a, rgb(255, 0, 0));
	pixel_set_vec2(x.b, rgb(0, 255, 0));
	pixel_set_vec2(x.c, rgb(0, 0, 255));
}

void line_tri(tri2_t tri, unsigned int color) {
	line(tri.a, tri.b, color);
	line(tri.b, tri.c, color);
	line(tri.c, tri.a, color);
}

void bottom_flat_tri_draw(
	tri2_t tri, 
	num *depth_buffer,
	num w,
	unsigned int color
) {
	num inv_slope_1 = (tri.b.x - tri.a.x) / (tri.b.y - tri.a.y);
	num inv_slope_2 = (tri.c.x - tri.a.x) / (tri.c.y - tri.a.y);
	num cur_x_1 = tri.a.x;
	num cur_x_2 = tri.a.x;
	for(int y = tri.a.y; y <= tri.b.y; y++) {
		horiz_line(
			y,
			(int) cur_x_1,
			(int) cur_x_2 + 1,
			depth_buffer,
			w,
			color
		);

		cur_x_1 += inv_slope_1;
		cur_x_2 += inv_slope_2;
	}
}

void top_flat_tri_draw(
	tri2_t tri,
	num *depth_buffer,
	num w,
	unsigned int color
) {
	num inv_slope_1 = (tri.a.x - tri.b.x) / (tri.a.y - tri.b.y);
	num inv_slope_2 = (tri.a.x - tri.c.x) / (tri.a.y - tri.c.y);
	num cur_x_1 = tri.a.x;
	num cur_x_2 = tri.a.x;
	for(int y = tri.a.y; y >= tri.b.y; y--) {
		horiz_line(
			y,
			(int) cur_x_1,
			(int) cur_x_2 + 1,
			depth_buffer,
			w,
			color
		);

		cur_x_1 -= inv_slope_1;
		cur_x_2 -= inv_slope_2;
	}
}

void tri_draw(
	tri2_t bad_tri,
	num *depth_buffer,
	num w,
	unsigned int color
) {
	tri2_t tri = correct_tri(bad_tri);
	if(tri.b.y == tri.c.y && tri.a.y < tri.b.y) {
		bottom_flat_tri_draw(tri, depth_buffer, w, color);
		return;
	}

	if(tri.b.y == tri.c.y && tri.a.y > tri.b.y) {
		top_flat_tri_draw(tri, depth_buffer, w, color);
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

	tri2_t top_flat = {
		tri.c,
		tri.b,
		mid_point
	};

	#ifdef COMMON_FILL_TRI
		bottom_flat_tri_draw(bottom_flat, depth_buffer, w, color);
		top_flat_tri_draw(top_flat, depth_buffer, w, color);
	#endif
	
	#ifdef COMMON_DEBUG_MODE
		line_tri(tri, color);
		pixel_set_tri(tri);
	#endif
}

#endif
