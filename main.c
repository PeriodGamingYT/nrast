#include "scene.h"
#define SDL_ASSERT(x, y) \
	ASSERT(x, "%s: %s\n", y, SDL_GetError())

void create_sdl() {
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

scene_t scene;
num old_time = 0;
num new_time = 0;
num elapsed_time = 0;
num frame_time = 0;
void render() {
	scene_draw(&scene);
}

void handle_key_down() {
	const unsigned char *key_state = SDL_GetKeyboardState(NULL);
	num speed = 2.0 * 0.016;
	scene.camera_rot.x -= (
		-key_state[SDL_SCANCODE_I] + key_state[SDL_SCANCODE_K]
	) * speed;

	scene.camera_rot.y -= (
		-key_state[SDL_SCANCODE_L] + key_state[SDL_SCANCODE_J]
	) * speed;
	
	scene.camera_rot.z -= (
		-key_state[SDL_SCANCODE_U] + key_state[SDL_SCANCODE_O]
	) * speed;

	scene.camera_pos.x -= (
		-key_state[SDL_SCANCODE_A] + key_state[SDL_SCANCODE_D]
	) * speed;

	scene.camera_pos.y -= (
		-key_state[SDL_SCANCODE_E] + key_state[SDL_SCANCODE_Q]
	) * speed;
	
	scene.camera_pos.z -= (
		-key_state[SDL_SCANCODE_S] + key_state[SDL_SCANCODE_W]
	) * speed;
}

void step_sdl() {
	new_time = SDL_GetTicks();
	SDL_Event event;
	frame_time = new_time - old_time;
	elapsed_time += frame_time;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_QUIT:
				state.quit = 1;
				break;
		}
	}

	handle_key_down();
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
	old_time = new_time;
}

void free_sdl() {
	free_scene(&scene);
	free(state.data);
	SDL_DestroyTexture(state.texture);
	SDL_DestroyRenderer(state.renderer);
	SDL_DestroyWindow(state.window);
}

int main() {
	create_sdl();
	scene = make_scene();
	mesh_t cube = make_cube_mesh();
	obj_t cube_obj = make_obj(&cube);
	vec3_t pos = { 0, 0, 3 };
	cube_obj.pos = pos;
	add_scene_obj(&scene, &cube_obj);
	while(!state.quit) {
		step_sdl();
	}

	free_sdl();
	return 0;
}
