#ifndef _LAB_MATH_H_
#define _LAB_MATH_H_

#include <GL/glew.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef GLfloat *mat4;
typedef GLfloat mat4_t[16];

#define MAT4_SIZE (sizeof(GLfloat)*4*4)

//#define mat_get(m, n) ((*(m))[n])
#define mat_get(m, n) ((m)[(n)])

static inline void mat_multiply(mat4 m2, mat4 m1) {
	mat4_t tmp;

	tmp[0] = ((m1[0]*m2[0])+(m1[1]*m2[4])+(m1[2]*m2[8])+(m1[3]*m2[12]));
	tmp[1] = ((m1[0]*m2[1])+(m1[1]*m2[5])+(m1[2]*m2[9])+(m1[3]*m2[13]));
	tmp[2] = ((m1[0]*m2[2])+(m1[1]*m2[6])+(m1[2]*m2[10])+(m1[3]*m2[14]));
	tmp[3] = ((m1[0]*m2[3])+(m1[1]*m2[7])+(m1[2]*m2[11])+(m1[3]*m2[15]));
	tmp[4] = ((m1[4]*m2[0])+(m1[5]*m2[4])+(m1[6]*m2[8])+(m1[7]*m2[12]));
	tmp[5] = ((m1[4]*m2[1])+(m1[5]*m2[5])+(m1[6]*m2[9])+(m1[7]*m2[13]));
	tmp[6] = ((m1[4]*m2[2])+(m1[5]*m2[6])+(m1[6]*m2[10])+(m1[7]*m2[14]));
	tmp[7] = ((m1[4]*m2[3])+(m1[5]*m2[7])+(m1[6]*m2[11])+(m1[7]*m2[15]));
	tmp[8] = ((m1[8]*m2[0])+(m1[9]*m2[4])+(m1[10]*m2[8])+(m1[11]*m2[12]));
	tmp[9] = ((m1[8]*m2[1])+(m1[9]*m2[5])+(m1[10]*m2[9])+(m1[11]*m2[13]));
	tmp[10] = ((m1[8]*m2[2])+(m1[9]*m2[6])+(m1[10]*m2[10])+(m1[11]*m2[14]));
	tmp[11] = ((m1[8]*m2[3])+(m1[9]*m2[7])+(m1[10]*m2[11])+(m1[11]*m2[15]));
	tmp[12] = ((m1[12]*m2[0])+(m1[13]*m2[4])+(m1[14]*m2[8])+(m1[15]*m2[12]));
	tmp[13] = ((m1[12]*m2[1])+(m1[13]*m2[5])+(m1[14]*m2[9])+(m1[15]*m2[13]));
	tmp[14] = ((m1[12]*m2[2])+(m1[13]*m2[6])+(m1[14]*m2[10])+(m1[15]*m2[14]));
	tmp[15] = ((m1[12]*m2[3])+(m1[13]*m2[7])+(m1[14]*m2[11])+(m1[15]*m2[15]));

	memcpy(m2, tmp, MAT4_SIZE);
}


static inline void mat_identity(mat4 mat) {
    memset(mat, 0, MAT4_SIZE);

	mat[0] = 1;
	mat[5] = 1;
	mat[10] = 1;
	mat[15] = 1;
}

static inline void mat_translate(mat4 mat, GLfloat x, GLfloat y, GLfloat z) {
	mat4_t tmp;

	mat_identity(tmp);

	tmp[12] = x;
	tmp[13] = y;
	tmp[14] = z;

	mat_multiply(mat, tmp);
}

static inline void mat_orthographic(mat4 mat, GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far) {
	mat_identity(mat);

    mat[0] = (2 / (right - left));
	mat[12] = -((right + left)/(right - left));
	mat[5] = (2 / (top - bottom));
	mat[13] = -((top + bottom)/(top - bottom));
	mat[10] = (-2/(far - near));
	mat[14] = ((far + near) / (far - near));
}

static inline float cotf(float r) {
// maybe	return cosf((M_PI/2) - r);
	return cosf(r)/sinf(r);
}

static inline void mat_perspective(mat4 mat, GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far) {
	mat_identity(mat);

	/*
	mat[0] = ((cotf(fov)/2)/aspect);
	mat[5] = ((cotf(fov)/2));
	mat[10] = (far/(far - near));
//	mat[10] = ((near+far)/(near-far));
	mat[11] = -((far * near)/(far - near));
//	mat[11] = -1;
	mat[14] = 1;
	mat[15] = 0;
//	mat[14] = ((2*near*far)/(near-far));
//	mat[15] = 0;
*/
//


	mat[0] = ((cotf(fov)/2)/aspect);
	mat[5] = ((cotf(fov)/2));
	mat[10] = ((far+near)/(near-far));
	mat[11] = -1;
	mat[14] = ((2*far*near)/(near-far));
	mat[15] = 0;

}

static inline void mat_rotate_x(mat4 mat, float radians) {
    mat4_t tmp;

	mat_identity(tmp);

	tmp[5] = cosf(radians);
	tmp[9] = -sinf(radians);
	tmp[6] = sinf(radians);
	tmp[10] = cosf(radians);

	mat_multiply(mat, tmp);
}

static inline void mat_rotate_z(mat4 mat, float radians) {
	mat4_t tmp;

	mat_identity(tmp);

	tmp[0] = cosf(radians);
	tmp[1] = sinf(radians);
	tmp[4] = -sinf(radians);
	tmp[5] = cosf(radians);

	mat_multiply(mat, tmp);
}

static inline void mat_rotate_y(mat4 mat, float radians) {
	mat4_t tmp;

	mat_identity(tmp);

	tmp[0] = cosf(radians);
	tmp[2] = -sinf(radians);
	tmp[8] = sinf(radians);
	tmp[10] = cosf(radians);

	mat_multiply(mat, tmp);
}

static inline void mat_scale(mat4 mat, float x, float y, float z) {
	mat4_t tmp;

	mat_identity(tmp);

	tmp[0] = x;
	tmp[5] = y;
	tmp[10] = z;

	mat_multiply(mat, tmp);
}

static inline void mat_check(mat4 mat) {
	int i;

	for (i = 0; i < MAT4_SIZE; ++i)
		if (mat[i] == INFINITY || mat[i] == NAN) {
			fprintf(stderr, "Found NaN or INF at %d\n", i);
			exit(EXIT_FAILURE);
		}
}

static inline void mat_print(mat4 mat) {
	fputs("[[[\n", stderr);
	fprintf(stderr, "%f\t%f\t%f\t%f\n", mat[0], mat[4], mat[8], mat[12]);
	fprintf(stderr, "%f\t%f\t%f\t%f\n", mat[1], mat[5], mat[9], mat[13]);
	fprintf(stderr, "%f\t%f\t%f\t%f\n", mat[2], mat[6], mat[10], mat[14]);
	fprintf(stderr, "%f\t%f\t%f\t%f\n", mat[3], mat[7], mat[11], mat[15]);
	fputs("]]]\n", stderr);
}

#endif // _LAB_MATH_H_
