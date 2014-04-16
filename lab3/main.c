#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "init.h"
#include "wavefront.h"
#include "shader.h"

void error(int err, const char *desc) {
	fprintf(stderr, "%d: %s\n", err, desc);
}

int main(int argc, char **argv) {
	GLFWwindow *window;
	GLenum err;
	GLint major, minor;
	GLuint shader;
	wavefront_obj_t *obj;
#ifdef DEBUG_PARSE
	int i, j;
#endif // DEBUG_PARSE

	window = init_window("Lab 3");

	if (!window)
		return EXIT_FAILURE;

	err = init_opengl();

	if (err != GLEW_OK) {
		fprintf(stderr, "GLEW: %s\n", glewGetErrorString(err));
		return EXIT_FAILURE;
	}

	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

#ifdef DEBUG_GL
	fprintf(stderr, "OpenGL version %d.%d\n", major, minor);
#endif // DEBUG_GL

	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	glfwSwapInterval(1);

	obj = wavefront_load("tetrahedron.obj");
#ifdef DEBUG_PARSE
	for (i = 0; i < obj->_nvertex; ++i)
		for (j = 0; j < 3; ++j)
			printf("V[%d][%d]: %f\n", i, j, obj->vertices[i].loc[j]);
	for (i = 0; i < obj->_nface; ++i)
		for (j = 0; j < obj->faces[i]._nvertex; ++j)
			printf("F[%d][%d]: %d\n", i, j, obj->faces[i].vertices[j]);
#endif //DEBUG_PARSE

	wavefront_get_gl_vertex_buffer(obj);
	wavefront_get_gl_face_buffer(obj);
	shader_load(GL_VERTEX_SHADER, "vertex.glsl", &shader);

	free(obj);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return EXIT_SUCCESS;
}
