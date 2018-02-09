#ifndef _BACKEND_H
#define _BACKEND_H

#include "stdafx.h"
#include "utility.h"

namespace GL {
	struct Texture {
		unsigned char * data;
		int w, h, d;
	};

	struct vSet {
		GLfloat verts[16];
	};
	struct Context {
		// Our stuff
		List<vSet> vertices;
		Vector2i base_res;
		float res_scale;
		int atlas_size;

		// GL stuff
		SDL_GLContext gl_context;
		GLuint vao;
		GLuint vbo;
		GLuint ebo;
		GLuint vertex;
		GLuint fragment;
		GLuint shader_program;
		GLuint tex;
		
		int init(SDL_Window * window, Texture * atlas, Vector2i base_res, float res_scale);
		void upload_vertices(int count);
		Vector2f normalize_position(Vector2i pos);
		vSet get_vset(Vector2i ipos, Vector2i itpos, Vector2i itsize);
		bool set_vertices(int index, Vector2i ipos, Vector2i itpos, Vector2i itsize);
		bool add_vertices(Vector2i ipos, Vector2i itpos, Vector2i itsize);
		void clear(RGBA color);
		void render(int count);
	};
};

#endif
