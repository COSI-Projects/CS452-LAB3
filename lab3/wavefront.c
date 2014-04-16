#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "wavefront.h"
#include "io.h"

wavefront_obj_t *wavefront_load(const char *file) {
	wavefront_obj_t *obj;
	char *data;

	data = io_read_file(file);

	if (!data)
		return NULL;

	obj = malloc(sizeof(wavefront_obj_t));

    memset(obj, 0, sizeof(wavefront_obj_t));

	if(wavefront_parse(obj, data)) {
		free(obj);
		return NULL;
	}

	free(data);

	return obj;
}

static vertex_t *parse_vertex(char *line) {
	char *part, *save;
	vertex_t *vert;
	int i;

	vert = malloc(sizeof(vertex_t));

	i = 0;

	part = strtok_r(line, " ", &save);

	do {
		vert->loc[i] = strtof(line, NULL);
	} while ((part = strtok_r(NULL, " ", &save)));

#ifdef DEBUG_PARSE
	for (i = 0; i < 3; ++i)
		printf("V[%d]: %f\n", i, vert->loc[i]);
#endif //DEBUG_PARSE

	return vert;
}

static face_t *parse_face(char *line) {
	char *part, *save;
	int i;
	face_t *face;

	face = malloc(sizeof(face_t));
	memset(face, 0, sizeof(face_t));
	face->vertices = malloc(sizeof(GLuint));
	face->_vsize = 1;
	i = 0;

	part = strtok_r(line, " ", &save);

	do {
		if (i+1 > face->_vsize) {
			face->vertices = realloc(face->vertices, 2*face->_vsize*sizeof(GLuint));
			face->_vsize *= 2;
		}
		face->vertices[i++] = atoi(part);
	} while ((part = strtok_r(NULL, " ", &save)));

	face->_nvertex = i;

#ifdef OPTIMIZE
	face->vertices = realloc(face->vertices, face->_nvertex * sizeof(GLuint));
	face->_vsize = face->_nvertex;
#ifdef DEBUG_MEM
	puts("*** OPTIMIZED ***");
#endif // DEBUG_MEM
#endif // OPTIMIZE

#ifdef DEBUG_MEM
	puts("*** Face stats ***");
	printf("V: %d (cap %d)\n", face->_nvertex, face->_vsize);
#endif // DEBUG_MEM

#ifdef DEBUG_PARSE
	for (i = 0; i < 3; ++i)
		printf("FV[%d]: %d\n", i, face->vertices[i]);
#endif //DEBUG_PARSE

	return face;
}

int wavefront_parse(wavefront_obj_t *obj, char *data) {
	char *line, *save;

	assert(obj != NULL && data != NULL);

	line = strtok_r(data, "\n", &save);

	do {
		if (line[1] == ' ') {
			switch (line[0]) {
				case 'v':
					wavefront_add_vertex(obj, parse_vertex(line+2));
					break;
				case 'f':
					wavefront_add_face(obj, parse_face(line+2));
					break;
				default:
					break;
			}
		}
	} while((line = strtok_r(NULL, "\n", &save)));

#ifdef OPTIMIZE
	obj->vertices = realloc(obj->vertices, obj->_nvertex * sizeof(vertex_t));
	obj->_vsize = obj->_nvertex;
	obj->faces = realloc(obj->faces, obj->_nface * sizeof(face_t));
	obj->_fsize = obj->_nface;
#ifdef DEBUG_MEM
	puts("*** OPTIMIZED ***");
#endif // DEBUG_MEM
#endif // OPTIMIZE

#ifdef DEBUG_MEM
	puts("*** Wavefront stats ***");
	printf("V: %d (cap %d)\n", obj->_nvertex, obj->_vsize);
	printf("F: %d (cap %d)\n", obj->_nface, obj->_fsize);
#endif // DEBUG_MEM

	return 0;
}

void wavefront_add_vertex(wavefront_obj_t *obj, vertex_t *v) {
	assert(obj != NULL && v != NULL);
	if (obj->vertices == NULL) {
		obj->vertices = malloc(sizeof(vertex_t));
		obj->_vsize = 1;
	}
	if (obj->_nvertex+1 > obj->_vsize) {
		obj->vertices = realloc(obj->vertices, 2*sizeof(vertex_t)*obj->_vsize);
		obj->_vsize *= 2;
	}
	memcpy(obj->vertices + (obj->_nvertex++), v, sizeof(vertex_t));
}

void wavefront_add_face(wavefront_obj_t *obj, face_t *f) {
	assert(obj != NULL && f != NULL);
	if (obj->faces == NULL) {
		obj->faces = malloc(sizeof(face_t));
		obj->_fsize = 1;
	}
	if (obj->_nface+1 > obj->_fsize) {
		obj->faces = realloc(obj->faces, 2*sizeof(face_t)*obj->_fsize);
		obj->_fsize *= 2;
	}
	memcpy(obj->faces + (obj->_nface++), f, sizeof(face_t));
}

GLuint wavefront_get_gl_vertex_buffer(wavefront_obj_t *obj) {
	GLuint buf;
	int n, i;

	n = 3 * obj->_nvertex;

	glGenBuffers(1, &buf);

	glBindBuffer(GL_ARRAY_BUFFER, buf);

	glBufferData(GL_ARRAY_BUFFER, n * sizeof(GLfloat), NULL, GL_STATIC_DRAW);

	for (i = 0; i < obj->_nvertex; ++i)
		glBufferSubData(GL_ARRAY_BUFFER, i*3*sizeof(GLfloat), 3*sizeof(GLfloat), obj->vertices[i].loc);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return buf;
}

GLuint wavefront_get_gl_face_buffer(wavefront_obj_t *obj) {
	GLuint buf;
	int i, n, nf;

	nf = 0;

	glGenBuffers(1, &buf);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf);

	for (i = 0; i < obj->_nface; ++i)
		n += obj->faces[i]._nvertex;

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, n * sizeof(GLuint), NULL, GL_STATIC_DRAW);

	for (i = 0; i < obj->_nface; ++i) {
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, nf * sizeof(GLuint), obj->faces[i]._nvertex * sizeof(GLuint), obj->faces[i].vertices);
			printf("Buffering %d things (%d)\n", obj->faces[i]._nvertex, nf);
			nf += obj->faces[i]._nvertex;
	}

	printf("There were %d faces\n", i);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return buf;
}
