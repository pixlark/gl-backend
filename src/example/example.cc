#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <math.h>

#include "../utility.h"
#include "../render.h"

float delta_time = 0.0167;

int walk_anim[] = {
	1, 2, 3, 2,
};

struct Mario {
	const float speed = 60;
	// Positioning and Textures
	Vector2f real_pos;
	Vector2i screen_pos;
	Vector2f scale;
	Vector2i tex_pos;
	Vector2i tex_size;
	// Flags
	bool small;
	bool visible;
	// Animation
	const float frame_length = 0.3;
	float frame_timer;
	int anim_frame;
	void init()
	{
		real_pos   = Vector2f(0, 0);
		screen_pos = Vector2i(0, 0);
		scale      = Vector2f(1, 1);
		small = false;
		visible = true;
		anim_frame = 0;
	}
};

int main()
{
	Vector2i base_res(256, 240);
	float res_scale = 4.0;
	
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window * window = SDL_CreateWindow("opengl backend test",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		base_res.x * res_scale, base_res.y * res_scale,
		SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL);

	{
		List<char> exe_path = get_exe_dir();
		exe_path.cat("..\\atlas.png", 13, 1);
		printf("Loading atlas from %s\n", exe_path.arr);
	
		Render::init(window, exe_path.arr, base_res, res_scale);

		exe_path.dealloc();
	}

	Mario mario;
	mario.init();

	uint32_t last_time = SDL_GetPerformanceCounter();
	SDL_Event event;
	bool running = true;
	while (running) {
		while (SDL_PollEvent(&event) != 0) {
			switch (event.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_MOUSEBUTTONDOWN: {
				if (event.button.button == SDL_BUTTON_LEFT) {
					mario.small = true;
				} else if (event.button.button == SDL_BUTTON_RIGHT) {
					mario.visible = false;
				}
			} break;
			case SDL_KEYDOWN: {
				if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
					mario.scale = Vector2f(2, 1);
				}
			} break;
			case SDL_MOUSEBUTTONUP: {
				if (event.button.button == SDL_BUTTON_LEFT) {
					mario.small = false;
				} else if (event.button.button == SDL_BUTTON_RIGHT) {
					mario.visible = true;
				}
			} break;
			case SDL_KEYUP: {
				if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
					mario.scale = Vector2f(1, 1);
				}
			} break;
			}
		}
		// Texture
		if (mario.small) {
			mario.tex_pos  = Vector2i(16 * walk_anim[mario.anim_frame], 0);
			mario.tex_size = Vector2i(16, 16);
		} else {
			mario.tex_pos  = Vector2i(16 * walk_anim[mario.anim_frame], 16);
			mario.tex_size = Vector2i(16, 32);
		}

		// Position
		mario.real_pos = Vector2f(fmod(mario.real_pos.x + mario.speed * delta_time, base_res.x), base_res.y - mario.tex_size.y);
		mario.screen_pos = (Vector2i) mario.real_pos;

		// Animation
		mario.frame_timer -= delta_time;
		if (mario.frame_timer <= 0) {
			mario.anim_frame++;
			mario.anim_frame %= 4;
			mario.frame_timer = mario.frame_length;
		}
		
		// Rendering
		Render::clear(RGBA(0, 0, 0, 0xFF));
		if (mario.visible) {
			Render::render(mario.screen_pos, mario.tex_pos, mario.tex_size, mario.scale);
		}
		Render::swap(window);
		
		// Manage framerate
		{
			uint32_t new_time = SDL_GetPerformanceCounter();
			delta_time = (float) (new_time - last_time) / SDL_GetPerformanceFrequency();
			last_time = new_time;
		}
		if (delta_time < 1.0 / 60.0) {
			// Lock to 60
			SDL_Delay(((1.0 / 60.0) - delta_time) * 1000);
		}
	}
	return 0;
}
