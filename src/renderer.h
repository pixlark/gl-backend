#ifndef _RENDERER_H
#define _RENDERER_H

#include "stdafx.h"
#include "utility.h"
#include "backend.h"

namespace Render {
	struct Renderer {
		GL::Context context;
		GL::Texture atlas;
		bool changed;
		int init(SDL_Window * window, char * atlas_path, Vector2i base_res, float res_scale);
		void clear(RGBA color);
		void render();
		void swap(SDL_Window * window);
		void end_frame();
	};
	struct Surface {
		Vector2i pos;
		Vector2i tex_coord;
		Vector2i tex_size;
		int vertice_index;
		bool visible;
		void init(Renderer * renderer, Vector2i pos, Vector2i tex_coord, Vector2i tex_size);
		void set_position(Renderer * renderer, Vector2i pos);
		void set_tex_coord(Renderer * renderer, Vector2i tex_coord, Vector2i tex_size);
		void set_visibility(Renderer * renderer, bool visible);
	};
};
 
#endif
