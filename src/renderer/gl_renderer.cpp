#include "gl_renderer.h"

#include <SDL2/SDL.h>
#include <cstring>
#include "utils/debug.h"

namespace castlekeep
{
namespace render
{
static const char *vertex_shader_source =
    R"(
    #version 330 core
    uniform float width;
    uniform float height;

    layout (location = 0) in vec2 pos;
    layout (location = 1) in float depth;
    layout (location = 2) in vec2 tex_pos_in;
    out vec2 tex_pos;
    void main()
    {
        gl_Position = vec4((pos.x/width)*2-1, (pos.y/height)*-2+1,(depth/16777216)*2-1, 1.0);
        //gl_Position = vec4((pos.x/width)*2-1, (pos.y/height)*-2+1,(depth/65536)*2-1, 1.0);
        tex_pos=tex_pos_in;
    }
    )";
static const char *fragment_shader_source =
    R"(
    #version 330 core
    layout (location = 0) out vec4 output_color;
    in vec2 tex_pos;
    uniform sampler2DRect tex;
    void main()
    {

      vec4 texel =texture(tex,tex_pos);
        if(texel.a<0.5) {
            discard;
        }
      output_color= texel;
    }
    )";

GLRenderSystem::GLRenderSystem(const memory::Arena &arena, SDL_Window *window,
                               size_t max_textures)
    : m_window(window),
      m_gl_context(nullptr),
      m_allocator(arena),
      m_num_textures(0),
      m_textures(max_textures, m_allocator),
      m_vao(0)
{
	SDL_GL_GetDrawableSize(m_window, &m_width, &m_height);
}

GLRenderSystem::~GLRenderSystem()
{
	SDL_GL_DeleteContext(m_gl_context);
}

TextureHandle GLRenderSystem::createTexture(int width, int height,
                                            PixelFormat format,
                                            const std::byte *data)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_RECTANGLE, texture);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA8, width, height, 0, GL_RGBA,
	             GL_UNSIGNED_BYTE, data);
	m_textures[m_num_textures] = texture;
	return static_cast<TextureHandle>(++m_num_textures);
}

int GLRenderSystem::startUp()
{
	char compiler_log[1024];

	m_gl_context = SDL_GL_CreateContext(m_window);
	if (!m_gl_context) {
		DEBUG_ERROR("context");
		return -1;
	}
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
	glCompileShader(vertex_shader);
	GLint status;
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		GLsizei log_length;
		glGetShaderInfoLog(vertex_shader, sizeof(compiler_log), &log_length,
		                   compiler_log);
		fprintf(stderr, "%s", compiler_log);
		DEBUG_ERROR("could not compile vertex shader");
		return -1;
	}
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		GLsizei log_length;
		glGetShaderInfoLog(fragment_shader, sizeof(compiler_log), &log_length,
		                   compiler_log);
		fprintf(stderr, "%s", compiler_log);
		DEBUG_ERROR("could not compile fragment shader");
		return -1;
	}
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	glUseProgram(program);

	GLint width_location = glGetUniformLocation(program, "width");
	if (width_location == -1) {
		DEBUG_ERROR("could not get uniform location");
		return -1;
	}
	GLint height_location = glGetUniformLocation(program, "height");
	if (height_location == -1) {
		DEBUG_ERROR("could not get uniform location");
		return -1;
	}
	glUniform1f(width_location, m_width);
	glUniform1f(height_location, m_height);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, k_vbo_size, nullptr, GL_STREAM_DRAW);
	glVertexAttribPointer(0, 3, GL_SHORT, GL_FALSE, 4 * 2 + 4,
	                      reinterpret_cast<void *>(0));
	glEnableVertexAttribArray(0);
	/*glVertexAttribPointer(1, 1, GL_UNSIGNED_INT, GL_FALSE, 4 * 2 + 4,
	                      reinterpret_cast<void *>(2 * 2));
						  */
	glVertexAttribPointer(1, 1, GL_UNSIGNED_INT, GL_FALSE, 4 * 2 + 4,
	                      reinterpret_cast<void *>(2 * 2));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_UNSIGNED_SHORT, GL_FALSE, 4 * 2 + 4,
	                      reinterpret_cast<void *>(2 * 2 + 4));
	glEnableVertexAttribArray(2);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// glDepthFunc(GL_GREATER);
	glDepthFunc(GL_GEQUAL);
	glEnable(GL_DEPTH_TEST);
	glClearDepthf(0.0);

	return 0;
}

struct Vertex {
	int16_t x;
	int16_t y;
	uint32_t z;
	uint16_t u;
	uint16_t v;
};

void GLRenderSystem::startFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLRenderSystem::draw(const DrawCommandBuffer &buffer)
{
	Vertex *buf = reinterpret_cast<Vertex *>(
	    glMapBufferRange(GL_ARRAY_BUFFER, 0, k_vbo_size,
	                     GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
	DEBUG_ASSERT(buffer.length * 6 * sizeof(Vertex) <= k_vbo_size,
	             "vbo buffer to small");
	for (int i = 0; i < buffer.length; i++) {
		// first triangle
		auto &cmd = buffer.commands[i];

		buf[i * 6 + 0].x = cmd.dst_x;
		buf[i * 6 + 0].y = cmd.dst_y;
		buf[i * 6 + 0].z = cmd.depth;
		buf[i * 6 + 0].u = cmd.src_x;
		buf[i * 6 + 0].v = cmd.src_y;

		buf[i * 6 + 1].x = cmd.dst_x + cmd.width;
		buf[i * 6 + 1].y = cmd.dst_y;
		buf[i * 6 + 1].z = cmd.depth;
		buf[i * 6 + 1].u = cmd.src_x + cmd.width;
		buf[i * 6 + 1].v = cmd.src_y;

		buf[i * 6 + 2].x = cmd.dst_x;
		buf[i * 6 + 2].y = cmd.dst_y + cmd.height;
		buf[i * 6 + 2].z = cmd.depth;
		buf[i * 6 + 2].u = cmd.src_x;
		buf[i * 6 + 2].v = cmd.src_y + cmd.height;

		// second triangle
		buf[i * 6 + 3].x = cmd.dst_x;
		buf[i * 6 + 3].y = cmd.dst_y + cmd.height;
		buf[i * 6 + 3].z = cmd.depth;
		buf[i * 6 + 3].u = cmd.src_x;
		buf[i * 6 + 3].v = cmd.src_y + cmd.height;

		buf[i * 6 + 4].x = cmd.dst_x + cmd.width;
		buf[i * 6 + 4].y = cmd.dst_y;
		buf[i * 6 + 4].z = cmd.depth;
		buf[i * 6 + 4].u = cmd.src_x + cmd.width;
		buf[i * 6 + 4].v = cmd.src_y;

		buf[i * 6 + 5].x = cmd.dst_x + cmd.width;
		buf[i * 6 + 5].y = cmd.dst_y + cmd.height;
		buf[i * 6 + 5].z = cmd.depth;
		buf[i * 6 + 5].u = cmd.src_x + cmd.width;
		buf[i * 6 + 5].v = cmd.src_y + cmd.height;
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindTexture(GL_TEXTURE_RECTANGLE, texture(buffer.texture_id));
	glDrawArrays(GL_TRIANGLES, 0, buffer.length * 6);
}

void GLRenderSystem::endFrame()
{
	SDL_GL_SwapWindow(m_window);
}

GLuint GLRenderSystem::texture(TextureHandle id)
{
	return m_textures[id - 1];
}

};  // namespace render
}  // namespace castlekeep
