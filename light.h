#ifndef __LIGHT_H
#define __LIGHT_H
#include "three_d.h"

enum light_type {
	DIRECTIONAL = 0,
	AMBIENT
};

typedef struct {
	enum light_type type;
	vec3_t pos;
	num intensity;
} light_t;

#define CLAMP(_x, _y, _z) \
	(MAX(_x, MIN(_y, _z)))

num compute_lighting(
	vec3_t normal,
	light_t **lights,
	int lights_size
) {
	num intensity = 0;
	for(int i = 0; i < lights_size; i++) {
		switch(lights[i]->type) {
			case DIRECTIONAL:
				num light_length = vec3_length(lights[i]->pos);
				vec3_t light_length_vec = { light_length, light_length, light_length, 1 };
				vec3_t light_dir = vec3_mul(lights[i]->pos, light_length_vec);
				intensity += CLAMP(0, 1, vec3_dot(normal, light_dir)) * lights[i]->intensity;
				break;
			
			case AMBIENT:
				intensity += lights[i]->intensity;
				break;
		}
	}

	//intensity = CLAMP(0, 1, intensity);
	return intensity;
}

#endif
