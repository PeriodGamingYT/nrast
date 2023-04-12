#include "three_d.h"
#define SDL_ASSERT(x, y) \
	ASSERT(x, "%s: %s\n", y, SDL_GetError())

#define MAIN_LOOP() \
	while(!state.quit)

void create_sdl() {
	// again, stolen from jdh.
	SDL_ASSERT(
		!SDL_Init(SDL_INIT_VIDEO),
		"can't init"
	);

	SDL_CreateWindowAndRenderer(
		1280,
		720,
		0,
		&state.window,
		&state.renderer
	);

	state.texture = SDL_CreateTexture(
		state.renderer,
		SDL_PIXELFORMAT_ABGR8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH,
		SCREEN_HEIGHT
	);

	SDL_ASSERT(
		state.texture,
		"can't create texture"
	);

	state.data = malloc(SCREEN_WIDTH * SCREEN_HEIGHT * 4);
}

// yes this is a hack, lol.
int is_computed = 0;
tri2_t draw_tri;
void render() {

	// put your rendering code here.
	// little test below, that's all.
	// render_func_here <-- for text editor, makes it easy to find this function.
	if(!is_computed) {
		tri3_t tri = {
			{ 0, 1, 1 },
			{ 0, 0, 1 },
			{ 1, 1, 1 }
		};
	
		mat_t proj = make_mat_proj(100);
		draw_tri = tri3_proj(tri, &proj);
		is_computed = 1;
	}

	tri_draw(draw_tri, rgb_combine(255, 255, 255));
}

void step_sdl() {
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_QUIT:
				state.quit = 1;
				break;
		}
	}

	memset(state.data, 0, SCREEN_WIDTH * SCREEN_HEIGHT * 4);
	render();
	SDL_UpdateTexture(
		state.texture,
		NULL,
		state.data,
		SCREEN_WIDTH * 4
	);
	
	SDL_RenderCopyEx(
		state.renderer,
		state.texture,
		NULL,
		NULL,
		0,
		NULL,
		SDL_FLIP_VERTICAL
	);

	SDL_RenderPresent(state.renderer);
}

void free_sdl() {
	free(state.data);
	SDL_DestroyTexture(state.texture);
	SDL_DestroyRenderer(state.renderer);
	SDL_DestroyWindow(state.window);
}

int main() {
	create_sdl();
	MAIN_LOOP() {
		step_sdl();
	}

	free_sdl();
	return 0;
}
