#include "backend.h"
x
void gl_shader_error(GLuint shader, char * section)
{
	printf("OPENGL SHADER ERROR (%s):\n", section);
	char err_buf[512];
	glGetShaderInfoLog(shader, 512, NULL, err_buf);
	printf("\t%s\n", err_buf);
}

int _gl_report_error(char * file, int line)
{
	int code = glGetError();
	int ret = code;
 again:
	if (code == GL_NO_ERROR) return ret;
	printf("OPENGL ERROR %s:%d\n\t", file, line);
	switch(code) {
	case GL_INVALID_ENUM:
		printf("GL_INVALID_ENUM\n\tBad value in enum argument.\n");
		break;
	case GL_INVALID_VALUE:
		printf("GL_INVALID_VALUE\n\tNumeric argument out of range.\n");
		break;
	case GL_INVALID_OPERATION:
		printf("GL_INVALID_OPERATION\n\tOperation not allowed in current state.\n");
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		printf("GL_INVALID_FRAMEBUFFER_OPERATION\n\tOperation not allowed because frame buffer is not complete.\n");
		break;
	case GL_OUT_OF_MEMORY:
		printf("GL_OUT_OF_MEMORY\n\tOpenGL ran out of memory.\n");
		break;
	}
	code = glGetError();
	goto again;
}

#define gl_report_error() _gl_report_error(__FILE__, __LINE__)
#define GL_CHECK(instruction) if (gl_report_error() != 0) instruction

char * vertex_shader_text =
	"#version 330 core\n"
	"in vec2 i_position;\n"
	"in vec2 i_tex_coord;\n"
	"out vec2 v_tex_coord;\n"
	"void main()\n"
	"{\n"
	"    gl_Position = vec4(i_position, 0.0, 1.0);\n"
	"    v_tex_coord = i_tex_coord;"
	"}\n";

char * fragment_shader_text =
	"#version 330 core\n"
	"in vec2 v_tex_coord;\n"
	"out vec4 f_color;\n"
	"uniform sampler2D tex;\n"
	"void main()\n"
	"{\n"
	"    f_color = texture(tex, v_tex_coord) * vec4(1.0, 1.0, 1.0, 1.0);\n"
	"}\n";

GLuint elements[] = {
	0, 1, 2,
	2, 3, 0,
};

int GL::Context::init(SDL_Window * window, Texture * atlas, Vector2i base_res, float res_scale)
{
	// Our stuff
	vertices.alloc();
	atlas_size = atlas->w;
	this->base_res = base_res;
	this->res_scale = res_scale;

	// GL stuff
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	
	gl_context = SDL_GL_CreateContext(window);
	glewExperimental = GL_TRUE;
	glewInit();
	GL_CHECK(return 1);
		
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GL_CHECK(return 1);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
	GL_CHECK(return 1);
	
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertex_shader_text, NULL);
	glCompileShader(vertex);
	{
		GLint status;
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE) {
			gl_shader_error(vertex, "vertex");
			return 1;
		}
	}
	GL_CHECK(return 1);

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragment_shader_text, NULL);
	glCompileShader(fragment);
	{
		GLint status;
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE) {
			gl_shader_error(fragment, "fragment");
			return 1;
		}
	}
	GL_CHECK(return 1);

	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex);
	glAttachShader(shader_program, fragment);
	glBindFragDataLocation(shader_program, 0, "f_color");
	glLinkProgram(shader_program);
	glUseProgram(shader_program);
	GL_CHECK(return 1);

	GLint pos_attrib = glGetAttribLocation(shader_program, "i_position");
	if (pos_attrib == -1) {
		printf("ERROR: %s:%d\n", __FILE__, __LINE__);
		return 1;
	}
	glEnableVertexAttribArray(pos_attrib);
	glVertexAttribPointer(pos_attrib, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, 0);
	GL_CHECK(return 1);

	GLint tex_attrib = glGetAttribLocation(shader_program, "i_tex_coord");
	if (tex_attrib == -1) {
		printf("ERROR: %s:%d\n", __FILE__, __LINE__);
		return 1;
	}
	glEnableVertexAttribArray(tex_attrib);
	glVertexAttribPointer(tex_attrib, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (void*) (sizeof(GLfloat) * 2));
	GL_CHECK(return 1);

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlas->w, atlas->h,
		0, GL_RGBA, GL_UNSIGNED_BYTE, atlas->data);
	GL_CHECK(return 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	return 0;
}
void GL::Context::upload_vertices(int count)
{
	glBufferData(GL_ARRAY_BUFFER, sizeof(vSet) * count, vertices.arr, GL_STATIC_DRAW);
}
Vector2f GL::Context::normalize_position(Vector2i pos)
{
	return Vector2f(
		((float) (pos.x * res_scale * 2) / (base_res.x * res_scale)) - 1.0,
		-((float) (pos.y * res_scale * 2) / (base_res.y * res_scale)) + 1.0);
}
GL::vSet GL::Context::get_vset(Vector2i ipos, Vector2i itpos, Vector2i itsize)
{
	Vector2f pos = normalize_position(ipos);
	Vector2f size(
		((float) itsize.x * 2) / base_res.x,
		((float) itsize.y * 2) / base_res.y);
	Vector2f tpos(
		(float) itpos.x / atlas_size,
		(float) itpos.y / atlas_size);
	Vector2f tsize(
		(float) itsize.x / atlas_size,
		(float) itsize.y / atlas_size);

	vSet set;
	set.verts[ 0] = (pos.x);
	set.verts[ 1] = (pos.y);
	set.verts[ 2] = (tpos.x);
	set.verts[ 3] = (tpos.y);
	
	set.verts[ 4] = (pos.x + size.x);
	set.verts[ 5] = (pos.y);
	set.verts[ 6] = (tpos.x + tsize.x);
	set.verts[ 7] = (tpos.y);
	
	set.verts[ 8] = (pos.x + size.x);
	set.verts[ 9] = (pos.y - size.y);
	set.verts[10] = (tpos.x + tsize.x);
	set.verts[11] = (tpos.y + tsize.y);
		
	set.verts[12] = (pos.x);
	set.verts[13] = (pos.y - size.y);
	set.verts[14] = (tpos.x);
	set.verts[15] = (tpos.y + tsize.y);

	return set;
}

bool GL::Context::set_vertices(int index, Vector2i ipos, Vector2i itpos, Vector2i itsize) {
	if (index >= vertices.len) return false;
	vertices[index] = get_vset(ipos, itpos, itsize);
	return true;
}

bool GL::Context::add_vertices(Vector2i ipos, Vector2i itpos, Vector2i itsize) {
	vertices.push(get_vset(ipos, itpos, itsize));
	return true;
}

void GL::Context::clear(RGBA color)
{
	glClearColor(
		color.r / 256.0,
		color.g / 256.0,
		color.b / 256.0,
		color.a / 256.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void GL::Context::render(int count)
{
	for (int i = 0; i < count; i++) {
		glDrawElementsBaseVertex(
			GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, i*4);
	}
}
