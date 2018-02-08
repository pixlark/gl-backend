#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <math.h>

#include "../utility.h"
#include "../renderer.h"

float delta_time = 0.0167;

int walk_anim[] = {
	1, 2, 3, 2,
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

	Render::Renderer renderer;
	{
		List<char> exe_path = get_exe_dir();
		exe_path.cat("..\\atlas.png", 13, 1);
		printf("Loading atlas from %s\n", exe_path.arr);
	
		renderer.init(window, exe_path.arr, base_res, res_scale);

		exe_path.dealloc();
	}
	
	Render::Surface surf;
	surf.init(&renderer, Vector2i(0, 0), Vector2i(0, 16), Vector2i(16, 32));

	Vector2f    mario_pos(0, 0);
	float       mario_speed  = 60;
	bool        small_mario  = false;
	int         anim_frame   = 0;
	const float frame_length = 0.3;
	float       frame_timer  = frame_length;

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
					small_mario = true;
				} else if (event.button.button == SDL_BUTTON_RIGHT) {
					surf.set_visibility(&renderer, false);
				}
			} break;
			case SDL_MOUSEBUTTONUP: {
				if (event.button.button == SDL_BUTTON_LEFT) {
					small_mario = false;
				} else if (event.button.button == SDL_BUTTON_RIGHT) {
					surf.set_visibility(&renderer, true);
				}
			} break;
			}
		}
		// Update
		mario_pos = Vector2f(fmod(mario_pos.x + mario_speed * delta_time, base_res.x), base_res.y - surf.tex_size.y);
		surf.set_position(&renderer, (Vector2i) mario_pos);
		if (small_mario) {
			surf.set_tex_coord(&renderer,
				Vector2i(16 * walk_anim[anim_frame], 0),
				Vector2i(16, 16));
		} else {
			surf.set_tex_coord(&renderer,
				Vector2i(16 * walk_anim[anim_frame], 16),
				Vector2i(16, 32));
		}
		frame_timer -= delta_time;
		if (frame_timer <= 0) {
			anim_frame++;
			anim_frame %= 4;
			frame_timer = frame_length;
		}
		
		// Rendering
		renderer.clear(RGBA(0, 0, 0, 0xFF));
		renderer.render();
		renderer.swap(window);
		renderer.end_frame();
		
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
