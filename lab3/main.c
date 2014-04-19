#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <time.h>
#include "init.h"
#include "wavefront.h"
#include "shader.h"
#include "math.h"

void error(int err, const char *desc) {
	fprintf(stderr, "%d: %s\n", err, desc);
}

GLfloat dx, dy;
GLfloat rx, ry;
GLfloat scale;

void key(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (action != GLFW_RELEASE) {
		switch (key) {
			case GLFW_KEY_W:
				dy += 0.1f;
				break;
			case GLFW_KEY_S:
				dy -= 0.1f;
				break;
			case GLFW_KEY_A:
				dx -= 0.1f;
				break;
			case GLFW_KEY_D:
				dx += 0.1f;
				break;
			case GLFW_KEY_J:
				ry -= 0.1f;
				break;
			case GLFW_KEY_L:
				ry += 0.1f;
				break;
			case GLFW_KEY_I:
				rx -= 0.1f;
				break;
			case GLFW_KEY_K:
				rx += 0.1f;
				break;
			case GLFW_KEY_E:
				scale += 0.05f;
				break;
			case GLFW_KEY_Q:
				scale -= 0.05f;
				break;
		}
	}
}

int main(int argc, char **argv) {
	GLFWwindow *window;
	GLenum err;
	GLint major, minor;
	GLuint vert, frag, vb, fb, vao, prog;
	wavefront_obj_t *obj;
	mat4_t trans, tmp;
	int verts, w, h;
#ifdef DEBUG_PARSE
	int i, j;
#endif // DEBUG_PARSE

	dx = dy = rx = ry = 0;
	scale = 1;

	window = init_window("Lab 3");

	if (!window)
		return EXIT_FAILURE;

	err = init_opengl();

	if (err != GLEW_OK) {
		fprintf(stderr, "GLEW: %s\n", glewGetErrorString(err));
		return EXIT_FAILURE;
	}

	glfwSetKeyCallback(window, key);

	glfwGetFramebufferSize(window, &w, &h);

	glViewport(0, 0, w, h);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

#ifdef DEBUG_GL
	fprintf(stderr, "OpenGL version %d.%d\n", major, minor);
#endif // DEBUG_GL

	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	glfwSwapInterval(1);

	obj = wavefront_load("monkeyobj.obj");
#ifdef DEBUG_PARSE
	for (i = 0; i < obj->_nvertex; ++i)
		for (j = 0; j < 3; ++j)
			printf("V[%d][%d]: %f\n", i, j, obj->vertices[i].loc[j]);
	for (i = 0; i < obj->_nface; ++i)
		for (j = 0; j < obj->faces[i]._nvertex; ++j)
			printf("F[%d][%d]: %d\n", i, j, obj->faces[i].vertices[j]);
#endif //DEBUG_PARSE

	verts = wavefront_num_verts(obj);
	printf("%d verts\n", verts);
	vb = wavefront_get_gl_vertex_buffer(obj);
	fb = wavefront_get_gl_face_buffer(obj);
	shader_load(GL_VERTEX_SHADER, "vertex.glsl", &vert);
	shader_load(GL_FRAGMENT_SHADER, "fragment.glsl", &frag);

	if(!shader_link(vert, frag, &prog)) {
		fputs("Linking failed!\n", stderr);
		return EXIT_FAILURE;
	}

	glUseProgram(prog);

	glDeleteShader(vert);
	glDeleteShader(frag);

	free(obj);


    glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vb);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fb);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		trans = mat_orthographic(-1, 1, -1, 1, 100, -100);
		mat_perspective(trans, 60, ((float)w)/((float)h), .1f, 100);
		mat_identity(tmp);
		mat_translate(tmp, dx, dy, -10.f);
		mat_multiply(trans, tmp);
		mat_identity(tmp);
		mat_rotate_x(tmp, rx);
   		mat_multiply(trans, tmp);
		mat_identity(tmp);
		mat_rotate_y(tmp, ry);
		mat_multiply(trans, tmp);
		mat_identity(tmp);
		mat_scale(tmp, scale, scale, scale);
		mat_multiply(trans, tmp);
		glUniformMatrix4fv(glGetUniformLocation(prog, "trans"), 1, GL_FALSE, trans);
		glDrawElements(GL_TRIANGLES, verts, GL_UNSIGNED_INT, NULL);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return EXIT_SUCCESS;
}
