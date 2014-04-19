#ifndef _LAB_SHADER_H_
#define _LAB_SHADER_H_

#include <GL/glew.h>

int shader_load(GLenum, const char *, GLuint *);
int shader_link(GLenum, GLenum, GLuint *);

#endif // _LAB_SHADER_H_
