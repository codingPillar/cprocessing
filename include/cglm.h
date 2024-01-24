#ifndef PROCESSING_CGLM_H
#define PROCESSING_CGLM_H

#include <GL/glew.h>

#define MAT4_SIZE 4

/* ROW WISE MATRIX */
struct Mat4f{
    GLfloat matrix[MAT4_SIZE * MAT4_SIZE];
};
struct Vec3f{
    GLfloat x, y, z;
};

struct Mat4f cglm_identity4();
struct Mat4f cglm_scale4(GLfloat x, GLfloat y, GLfloat z);
struct Mat4f cglm_translate4(GLfloat x, GLfloat y, GLfloat z);
struct Mat4f cglm_transpose4(const struct Mat4f *matrix);
/* COMPUTES result * matrix */
struct Mat4f cglm_multiply4(const struct Mat4f first, const struct Mat4f second);
void cglm_print(const struct Mat4f *matrix);



#endif //PROCESSING_CGLM_H