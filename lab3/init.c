#include "init.h"
#include <stdlib.h>
#include <stdio.h>

static void error(int code, const char *desc) {
	fprintf(stderr, "%d: %s\n", code, desc);
}

static void resize(GLFWwindow *window, int w, int h) {
	glViewport(0, 0, w, h);
}

GLFWwindow *init_window(const char *title) {
	GLFWwindow *window;

	glfwSetErrorCallback(error);

	if (!glfwInit())
		return NULL;

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	window = glfwCreateWindow(640, 480, title, NULL, NULL);

	if (!window) {
		glfwTerminate();
		return NULL;
	}

	glfwSetWindowSizeCallback(window, resize);

	glfwMakeContextCurrent(window);

	return window;
}

GLenum init_opengl() {
	GLenum ret;
	glewExperimental = GL_TRUE;

	ret = glewInit();

	if (ret == GLEW_OK)
		glViewport(0, 0, 640, 480);

	return ret;
}
