#include "../include/cglm.h"

#include "stdio.h"

struct Mat4f cglm_identity4(){
    struct Mat4f result = {0};
    for(unsigned int i = 0; i < MAT4_SIZE; i++) result.matrix[MAT4_SIZE * i + i] = 1.0f;
    return result;
}

struct Mat4f cglm_scale4(GLfloat x, GLfloat y, GLfloat z){
    struct Mat4f scale = {0};
    GLfloat diagonal[] = {x, y, z, 1.0f};
    for(unsigned int i = 0; i < MAT4_SIZE; i++)
        scale.matrix[MAT4_SIZE * i + i] = diagonal[i];
    return scale;
}

struct Mat4f cglm_translate4(GLfloat x, GLfloat y, GLfloat z){
    struct Mat4f translation = cglm_identity4();
    GLfloat vect[] = {x, y, z};
    for(unsigned int i = 0; i < MAT4_SIZE - 1; i++) translation.matrix[(i + 1) * MAT4_SIZE - 1] = vect[i];
    return translation;
}

struct Mat4f cglm_transpose4(const struct Mat4f *matrix){
    struct Mat4f result = {0};
    for(unsigned int i = 0; i < MAT4_SIZE; i++){
        for(unsigned int j = 0; j < MAT4_SIZE; j++)
            result.matrix[i * MAT4_SIZE + j] = matrix->matrix[j * MAT4_SIZE + i];
    }
    return result;
}

struct Mat4f cglm_multiply4(const struct Mat4f first, const struct Mat4f second){
    struct Mat4f result = {0};
    for(unsigned int i = 0; i < MAT4_SIZE; i++){
        for(unsigned int j = 0; j < MAT4_SIZE; j++){
            float sum = 0.f;
            for(unsigned int k = 0; k < MAT4_SIZE; k++)
                sum += first.matrix[i * MAT4_SIZE + k] * second.matrix[k * MAT4_SIZE + j];
            result.matrix[i * MAT4_SIZE + j] = sum;
        }
    }
    return result;
}

void cglm_print(const struct Mat4f *matrix){
    printf("[\n");
    for(unsigned int i = 0; i < MAT4_SIZE; i++){
        for(unsigned int j = 0; j < MAT4_SIZE; j++){
            printf("%f, ", matrix->matrix[i * MAT4_SIZE + j]);
        }
        printf("\n");
    }
    printf("]\n");
}