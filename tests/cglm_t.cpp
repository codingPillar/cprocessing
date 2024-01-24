#include <gtest/gtest.h>

extern "C"{
#include "../include/cglm.h"
}

TEST(cglm, identity){
    struct Mat4f identity = cglm_identity4();
    for(unsigned int i = 0; i < MAT4_SIZE; i++){
        for(unsigned int j = 0; j < MAT4_SIZE; j++){
            if(i == j) ASSERT_EQ(identity.matrix[i * MAT4_SIZE + j], 1.0f);
            else ASSERT_EQ(identity.matrix[i * MAT4_SIZE + j], 0.0f);
        }
    }
}

TEST(cglm, scale){
    struct Mat4f scale = cglm_scale4(1.0f, 2.0f, 3.0f);
    GLfloat diagonal[] = {1.0f, 2.0f, 3.0f}; 
    for(unsigned int i = 0; i < MAT4_SIZE; i++){
        for(unsigned int j = 0; j < MAT4_SIZE; j++){
            if(i == j) ASSERT_EQ(scale.matrix[i * MAT4_SIZE + j], diagonal[i]);
            else ASSERT_EQ(scale.matrix[i * MAT4_SIZE + j], 0.0f);
        }
    }
}

TEST(cglm, trasposeIdentity){
    struct Mat4f identity = cglm_identity4();
    struct Mat4f transpose = cglm_transpose4(&identity);
    for(unsigned int i = 0; i < MAT4_SIZE; i++){
        for(unsigned int j = 0; j < MAT4_SIZE; j++)
            ASSERT_EQ(identity.matrix[i * MAT4_SIZE + j], transpose.matrix[i * MAT4_SIZE + j]);
    }
}

TEST(cglm, traspose){
    struct Mat4f scale = cglm_scale4(1.0f, 2.5f, 3.5f);
    struct Mat4f transpose = cglm_transpose4(&scale);
    for(unsigned int i = 0; i < MAT4_SIZE; i++){
        for(unsigned int j = 0; j < MAT4_SIZE; j++)
            ASSERT_EQ(scale.matrix[i * MAT4_SIZE + j], transpose.matrix[j * MAT4_SIZE + i]);
    }
}

TEST(cglm, multiplyIdentity){
    struct Mat4f scale = cglm_scale4(1.0f, 2.5f, 3.5f);
    struct Mat4f identity = cglm_identity4();
    struct Mat4f result = cglm_multiply4(scale, identity);
    for(unsigned int i = 0; i < MAT4_SIZE; i++){
        for(unsigned int j = 0; j < MAT4_SIZE; j++)
            ASSERT_EQ(result.matrix[i * MAT4_SIZE + j], scale.matrix[i * MAT4_SIZE + j]);
    }
}