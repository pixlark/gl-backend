#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "renderer.h"

int Render::Renderer::init(SDL_Window * window, char * atlas_path, Vector2i base_res, float res_scale)
{
	//changed = false;
	atlas.data = stbi_load(atlas_path, &atlas.w, &atlas.h, &atlas.d, 0);
	if (atlas.data == NULL) return 1;
	if (context.init(window, &atlas, base_res, res_scale) != 0) return 1;
	return 0;
}

void Render::Renderer::clear(RGBA color)
{
	//if (!changed) return;
	context.clear(color);
}

void Render::Renderer::render(Render::Surface surface)
{
	//if (!changed) return;
	if (!context.set_vertices(vertice_index++, surface.pos, surface.tex_coord, surface.tex_size)) {
		context.add_vertices(surface.pos, surface.tex_coord, surface.tex_size);
	}
}

void Render::Renderer::swap(SDL_Window * window)
{
	//if (!changed) return;
	context.upload_vertices();
	context.render();
	SDL_GL_SwapWindow(window);
}

void Render::Renderer::end_frame()
{
	//changed = false;
	vertice_index = 0;
}

void Render::Surface::init(Renderer * renderer, Vector2i pos, Vector2i tex_coord, Vector2i tex_size)
{
	visible = true;
	this->pos = pos;
	this->tex_coord = tex_coord;
	this->tex_size = tex_size;
	//renderer->context.add_vertices(pos, tex_coord, tex_size);
	//vertice_index = renderer->context.vertices.len - 1;
}
void Render::Surface::set_position(Renderer * renderer, Vector2i pos)
{
	this->pos = pos;
	if (visible) {
		//renderer->changed = true;
		//renderer->context.set_vertices(vertice_index, pos, tex_coord, tex_size);
	}
}
void Render::Surface::set_tex_coord(Renderer * renderer, Vector2i tex_coord, Vector2i tex_size)
{
	this->tex_coord = tex_coord;
	this->tex_size  = tex_size;
	if (visible) {
		//renderer->changed = true;
		//renderer->context.set_vertices(vertice_index, pos, tex_coord, tex_size);
	}
}
void Render::Surface::set_visibility(Renderer * renderer, bool visible)
{
	if (this->visible == visible) return;
	//renderer->changed = true;
	if (visible) {
		//renderer->context.set_vertices(vertice_index, pos, tex_coord, tex_size);
	} else {
		//renderer->context.set_vertices(vertice_index, Vector2i(-tex_size.x, -tex_size.y), tex_coord, tex_size);
	}
	this->visible = visible;
}

