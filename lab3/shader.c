#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "shader.h"
#include "io.h"

int shader_load(GLenum type, const char *file, GLuint *id) {
	GLuint shader;
	GLint status, len;
	char *data;

	assert((type == GL_VERTEX_SHADER || type == GL_FRAGMENT_SHADER) && file != NULL && id != NULL);

	data = io_read_file(file);

	if (!data)
		return 0;

	shader = glCreateShader(type);
	glShaderSource(shader, 1, (const GLchar **)&data, NULL);
	glCompileShader(shader);

#ifdef DEBUG_GL
	fprintf(stderr, "Compile %s\n", file);
	fprintf(stderr, "Shader {\n%s\n}", *((const GLchar **)&data));
#endif // DEBUG_GL

	free(data);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE) {
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
		data = malloc(len);
		if (!data) {
			puts("Insufficient memory!");
			glDeleteShader(shader);
			return 0;
		}
		glGetShaderInfoLog(shader, len, NULL, data);
		fputs("*** SHADER COMPILATION FAILED ***\n", stderr);
		fprintf(stderr, "%s\n", data);
		free(data);
		return 0;
	}

	*id = shader;

	return 1;
}

int shader_link(GLenum vert, GLenum frag, GLuint *program) {
	GLenum prog;
	GLint status;

	prog = glCreateProgram();

	glAttachShader(prog, vert);
	glAttachShader(prog, frag);
	glLinkProgram(prog);

	glGetProgramiv(prog, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
		return 0;

	*program = prog;

	return 1;
}
