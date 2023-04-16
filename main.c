#include "mesh.h"
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

mesh_t cube;
mat_t proj;
// num old_time = 0;
// num new_time = 0;
// num elapsed_time = 0;
void render() {
	// put your rendering code here.
	// little test below, that's all.
	// render_func_here <-- for text editor, makes it easy to find this function.
	// new_time = SDL_GetTicks();
	mesh_clean_slate(&cube);
	// num frame_time = (new_time - old_time);
	// elapsed_time += frame_time;
	// vec3_t rot = { 1, 0, 0 };
	// tri3_t eg_tri = TRI3_PART(0, 0, 1, 1, 0, 0, 1, 1, 1);
	// mat_t rot_x = mat_rot_x(PI * 2);
	// TRI3_MAT_MUL(eg_tri, &rot_x);
	// PRINT_TRI3(eg_tri);
	// tri2_t draw_tri = tri3_proj(eg_tri, &proj);
	// PRINT_TRI2(draw_tri);
	// tri_draw(draw_tri, rgb_combine(255, 255, 255));
	vec3_t trans = { 0, 0, 3 };
	mesh_trans(&cube, trans);
	mesh_draw(&cube, &proj, rgb_combine(255, 255, 255));
	// old_time = new_time;
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
	free_mesh(&cube);
	free(state.data);
	SDL_DestroyTexture(state.texture);
	SDL_DestroyRenderer(state.renderer);
	SDL_DestroyWindow(state.window);
}

int main() {
	create_sdl();
	cube = make_cube_mesh();
	proj = make_mat_proj();
	MAIN_LOOP() {
		step_sdl();
	}

	free_sdl();
	return 0;
}
