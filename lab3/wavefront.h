#ifndef _LAB_WAVEFRONT_H_
#define _LAB_WAVEFRONT_H_

#include <GL/glew.h>

typedef struct vertex {
	GLfloat loc[3];
} vertex_t;

typedef struct face {
	GLuint *vertices;
	int _nvertex, _vsize;
} face_t;

typedef struct wavefront_obj {
	vertex_t *vertices;
	face_t *faces;
	int _nvertex, _vsize, _nface, _fsize;
} wavefront_obj_t;

wavefront_obj_t *wavefront_load(const char *);
int wavefront_parse(wavefront_obj_t *, char *);
void wavefront_add_vertex(wavefront_obj_t *, vertex_t *);
void wavefront_add_face(wavefront_obj_t *, face_t *);
GLuint wavefront_get_gl_vertex_buffer(wavefront_obj_t *);
GLuint wavefront_get_gl_face_buffer(wavefront_obj_t *);

#endif // _LAB_WAVEFRONT_H_
