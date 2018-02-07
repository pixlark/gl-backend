#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "../utility.h"
#include "../renderer.h"

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
	renderer.init(window, "atlas.png", base_res, res_scale);
	
	Render::Surface surf;
	surf.init(&renderer, Vector2i(0, 0), Vector2i(0, 16), Vector2i(16, 32));
	
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
					surf.set_tex_coord(&renderer, Vector2i(0, 0), Vector2i(16, 16));
				} else if (event.button.button == SDL_BUTTON_RIGHT) {
					surf.set_visibility(&renderer, false);
				}
			} break;
			case SDL_MOUSEBUTTONUP: {
				if (event.button.button == SDL_BUTTON_LEFT) {
					surf.set_tex_coord(&renderer, Vector2i(0, 16), Vector2i(16, 32));
				} else if (event.button.button == SDL_BUTTON_RIGHT) {
					surf.set_visibility(&renderer, true);
				}
			} break;
			}
		}
		// Update
		surf.set_position(&renderer, Vector2i((surf.pos.x + 1) % base_res.x, surf.pos.y));
		
		// Rendering
		renderer.clear(RGBA(0, 0, 0, 0xFF));
		renderer.render();
		renderer.swap(window);
		renderer.end_frame();
		SDL_Delay(16);
	}
	
	return 0;
}
