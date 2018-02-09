#ifndef _RENDERER_H
#define _RENDERER_H

#include "stdafx.h"
#include "utility.h"
#include "backend.h"

namespace Render {
	GL::Context context;
	GL::Texture atlas;
	int vertice_index;
	int init(SDL_Window * window, char * atlas_path, Vector2i base_res, float res_scale);
	void clear(RGBA color);
	void render(Vector2i pos, Vector2i tex_pos, Vector2i tex_size);
	void swap(SDL_Window * window);
};

#endif
