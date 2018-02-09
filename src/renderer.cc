#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "renderer.h"

int Render::init(SDL_Window * window, char * atlas_path, Vector2i base_res, float res_scale)
{
	vertice_index = 0;
	atlas.data = stbi_load(atlas_path, &atlas.w, &atlas.h, &atlas.d, 0);
	if (atlas.data == NULL) return 1;
	if (context.init(window, &atlas, base_res, res_scale) != 0) return 1;
	return 0;
}

void Render::clear(RGBA color)
{
	context.clear(color);
}

void Render::render(Vector2i pos, Vector2i tex_pos, Vector2i tex_size)
{
	if (!context.set_vertices(vertice_index++, pos, tex_pos, tex_size)) {
		context.add_vertices(pos, tex_pos, tex_size);
	}
}

void Render::swap(SDL_Window * window)
{
	context.upload_vertices(vertice_index);
	context.render(vertice_index);
	SDL_GL_SwapWindow(window);
	vertice_index = 0;
}
