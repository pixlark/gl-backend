#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <math.h>

#include "../utility.h"
#include "../render.h"

RGBA pallette_index[] = {
	{106, 107, 004, 255},
	{177, 052, 037, 255},
	{227, 157, 037, 255}
};

RGBA pallette_swap[] = {
	{255, 000, 000, 255},
	{000, 255, 000, 255},
	{000, 000, 255, 255}
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

	bool running = true;
	SDL_Event event;
	while (running) {
		while (SDL_PollEvent(&event) != 0) {
			switch (event.type) {
			case SDL_QUIT:
				running = false;
				break;
			}
		}
	}
	
	return 0;
}
