#ifndef __PIXEL_H
#define __PIXEL_H
#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include "common.h"
#define ASSERT(x, ...) \
	if(!(x)) { \
		fprintf(stderr, __VA_ARGS__); \
		exit(1); \
	}

#define SCREEN_WIDTH 384
#define SCREEN_HEIGHT 216
struct {
	SDL_Window *window;
	SDL_Texture *texture;
	SDL_Renderer *renderer;
	unsigned int *data;
	int quit;
} state;

unsigned int rgb(
	unsigned char r,
	unsigned char g,
	unsigned char b
) {
	return r | (g << 8) | (b << 16);
}

void pixel_set(int x, int y, unsigned int color) {
	if(
		x < 0 ||
		y < 0 ||
		x >= SCREEN_WIDTH ||
		y >= SCREEN_HEIGHT
	) {
		return;
	}

	// sdl2 starts from the bottom to the top.
	// so setting y "upside down" is needed.
	state.data[
		y * SCREEN_WIDTH + x
	] = color;
}

#endif
