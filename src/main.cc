#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "stdafx.h"
#include "utility.h"
#include "backend.h"

struct Renderer {
	GL::Context context;
	GL::Texture atlas;
	bool changed;
	int init(SDL_Window * window, char * atlas_path, Vector2i base_res, float res_scale)
	{
		changed = false;
		atlas.data = stbi_load(atlas_path, &atlas.w, &atlas.h, &atlas.d, 0);
		if (atlas.data == NULL) return 1;
		if (context.init(window, &atlas, base_res, res_scale) != 0) return 1;
		return 0;
	}
	void clear(RGBA color)
	{
		if (!changed) return;
		glClearColor(
			color.r / 256.0,
			color.g / 256.0,
			color.b / 256.0,
			color.a / 256.0);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	void render()
	{
		if (!changed) return;
		context.upload_vertices();
		context.render();
	}
	void swap(SDL_Window * window)
	{
		if (!changed) return;
		SDL_GL_SwapWindow(window);
	}
	void end_frame()
	{
		changed = false;
	}
};

struct Surface {
	Vector2i pos;
	Vector2i tex_coord;
	Vector2i tex_size;
	int vertice_index;
	bool visible;
	void init(Renderer * renderer, Vector2i pos, Vector2i tex_coord, Vector2i tex_size)
	{
		renderer->changed = true;
		visible = true;
		this->pos = pos;
		this->tex_coord = tex_coord;
		this->tex_size = tex_size;
		renderer->context.add_vertices(pos, tex_coord, tex_size);
		vertice_index = renderer->context.vertices.len - 1;
	}
	void set_position(Renderer * renderer, Vector2i pos)
	{
		this->pos = pos;
		if (visible) {
			renderer->changed = true;
			renderer->context.set_vertices(vertice_index, pos, tex_coord, tex_size);
		}
	}
	void set_tex_coord(Renderer * renderer, Vector2i tex_coord, Vector2i tex_size)
	{
		this->tex_coord = tex_coord;
		this->tex_size  = tex_size;
		if (visible) {
			renderer->changed = true;
			renderer->context.set_vertices(vertice_index, pos, tex_coord, tex_size);
		}
	}
	void set_visibility(Renderer * renderer, bool visible)
	{
		if (this->visible == visible) return;
		renderer->changed = true;
		if (visible) {
			renderer->context.set_vertices(vertice_index, pos, tex_coord, tex_size);
		} else {
			renderer->context.set_vertices(vertice_index, Vector2i(-tex_size.x, -tex_size.y), tex_coord, tex_size);
		}
		this->visible = visible;
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

	Renderer renderer;
	renderer.init(window, "atlas.png", base_res, res_scale);
	
	Surface surf;
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
				//surf.set_visibility(&renderer, false);
				surf.set_tex_coord(&renderer, Vector2i(0, 0), Vector2i(16, 16));
			} break;
			case SDL_MOUSEBUTTONUP: {
				//surf.set_visibility(&renderer, true);
				surf.set_tex_coord(&renderer, Vector2i(0, 16), Vector2i(16, 32));
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
