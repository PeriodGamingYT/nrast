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
	num speed = COMMON_SPEED_AMOUNT * frame_time;
	vec3_t rot_inc_z = { 0, 0, -PI, 1  };
	vec3_t temp_rot =  vec3_add(scene.camera_rot, rot_inc_z);
	num rot_y_key = (-key_state[SDL_SCANCODE_L] + key_state[SDL_SCANCODE_J]) * speed;
	num horiz_key = (-key_state[SDL_SCANCODE_A] + key_state[SDL_SCANCODE_D]) * speed;
	num vertical_key = (-key_state[SDL_SCANCODE_Q] + key_state[SDL_SCANCODE_E]) * speed;
	num forward_key = (-key_state[SDL_SCANCODE_W] + key_state[SDL_SCANCODE_S]) * speed;
	vec3_t rot_y = { 0, rot_y_key, 0, 1 };
	vec3_t pos_x = { horiz_key, 0, 0, 1 };
	vec3_t pos_y = { 0, vertical_key, 0, 1  };
	vec3_t pos_z = { 0, 0, forward_key, 1 };
	scene.camera_rot = vec3_add(scene.camera_rot, vec3_rot(rot_y, temp_rot));
	scene.camera_pos = vec3_add(scene.camera_pos, vec3_rot(pos_x, temp_rot));
	scene.camera_pos = vec3_add(scene.camera_pos, vec3_rot(pos_y, temp_rot));
	scene.camera_pos = vec3_add(scene.camera_pos, vec3_rot(pos_z, temp_rot));
	if(key_state[SDL_SCANCODE_R]) {
		vec3_t zero = { 0, 0, 0, 1 };
		scene.camera_pos = zero;
		scene.camera_rot = zero;
	}
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
	memset(depth_buffer, (int) COMMON_Z_FAR, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(num));
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

	// whenever something goes wrong, i test each part of the buggy code
	// one by one, COMMON_DEBUG_ALONE helps me do that. 
	#ifdef COMMON_DEBUG_ALONE
		return 0;
	#else
		create_sdl();
		scene = make_scene();
		mesh_t obj_mesh = load_obj("mesh.obj");
		obj_t obj = make_obj(&obj_mesh);
		vec3_t obj_pos = { 0, 0, 3, 1 };
		obj.pos = obj_pos;
		add_scene_obj(&scene, &obj);
		vec3_t ambient_pos = { -1, 0, 3, 1 };
		light_t ambient = {
			AMBIENT,
			ambient_pos,
			0.2
		};

		add_scene_light(&scene, &ambient);
		vec3_t dir_top = { 0, -1, 0, 1 };
		vec3_t dir_left = { -1, 0, 0, 1 };
		vec3_t dir_forward = { 0, 0, -1, 1 };
		light_t dir_light_top = {
			DIRECTIONAL,
			dir_top,
			0.3
		};

		light_t dir_light_left = {
			DIRECTIONAL,
			dir_left,
			0.4
		};

		light_t dir_light_forward = {
			DIRECTIONAL,
			dir_forward,
			0.5
		};

		add_scene_light(&scene, &dir_light_top);
		add_scene_light(&scene, &dir_light_left);
		add_scene_light(&scene, &dir_light_forward);
		vec3_t camera_pos = { 10, -3, -15, 1 };
		vec3_t camera_rot = { 0, deg(135), 0, 1 };
		scene.camera_pos = camera_pos;
		scene.camera_rot = camera_rot;
		while(!state.quit) {
			step_sdl();
		}

		free_sdl();
		return 0;
	#endif
}
