#include <stdbool.h>

#include "../include/cprocessing.h"
#include "../include/cglm.h"

#define GLSL_POSITION_ATTRIB_NAME "vertexPosition"
#define GLSL_COLOR_ATTRIB_NAME "vertexColor"
#define GLSL_MVP_UNIFORM_NAME "mv"

#define MAX_VBO_COUNT 4

struct Shape{
    GLuint vao;
    GLuint vbo[MAX_VBO_COUNT];
    GLuint count;
};

static struct DrawingContext{
    unsigned int width;
    unsigned int height;
    GLfloat r; 
    GLfloat g; 
    GLfloat b;
    GLuint basicProgramID;
    struct Shape triangle;
    struct Shape rectangle;
    struct Shape ellipse;
} drawingContext = {0};

static const char *BASIC_VERTEX_SHADER = 
    "#version 330 core\n"
    "uniform mat4 " GLSL_MVP_UNIFORM_NAME ";\n"
    "layout (location=0) in vec3 " GLSL_POSITION_ATTRIB_NAME ";\n"
    "layout (location=1) in vec3 " GLSL_COLOR_ATTRIB_NAME ";\n"
    "out vec3 color;\n"
    "void main(){\n"
    "   gl_Position = mv * vec4(vertexPosition, 1.);\n"
    "   color = vertexColor;\n"
    "}";

static const char *BASIC_FRAGMENT_SHADER = 
    "#version 330 core\n"
    "in vec3 color;\n"
    "out vec4 fragColor;\n"
    "void main(){\n"
    "   fragColor = vec4(color, 1.0);\n"
    "}";

/* SHAPES DATA */
GLfloat triangleVerteces[] = { -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.f, 0.5f, 0.0f };
GLfloat triangleColor[]    = {  1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.f, 1.0f, 0.0f };

GLfloat rectangleVerteces[] = { -0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f, -0.5f, 0.5f, 0.0f,
                                -0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f,  0.5f,-0.5f, 0.0f };

static void get_shader_error(GLuint shader){
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if(status == GL_TRUE) return;
    GLint length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    GLchar message[length];
    glGetShaderInfoLog(shader, length, NULL, message);
    printf("%s\n", message);
}

static void get_program_error(GLuint program){
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(status == GL_TRUE) return;
    GLint length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    GLchar message[length];
    glGetProgramInfoLog(program, length, NULL, message);
    printf("%s\n", message);
}

static void init_basic_program(){
    GLuint vsid = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vsid, 1, &BASIC_VERTEX_SHADER, NULL);
    glCompileShader(vsid);
    GLuint fsid = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fsid, 1, &BASIC_FRAGMENT_SHADER, NULL);
    glCompileShader(fsid);
    get_shader_error(vsid);
    get_shader_error(fsid);
    drawingContext.basicProgramID = glCreateProgram();
    glAttachShader(drawingContext.basicProgramID, vsid);
    glAttachShader(drawingContext.basicProgramID, fsid);
    glLinkProgram(drawingContext.basicProgramID);
    get_program_error(drawingContext.basicProgramID);
}

static void enable_vertex_array(struct Shape *shape, GLuint index, GLfloat *data, GLuint byteSize){
    glBindVertexArray(shape->vao);
    glGenBuffers(1, &shape->vbo[shape->count]);
    glBindBuffer(GL_ARRAY_BUFFER, shape->vbo[shape->count]);
    glBufferData(GL_ARRAY_BUFFER, byteSize, data, GL_STATIC_DRAW);
    glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(index);
    shape->count++;
}

static void init_cprocessing(){
    /* CREATE BASIC PROGRAM */
    init_basic_program();

    GLuint positionLocation = glGetAttribLocation(drawingContext.basicProgramID, GLSL_POSITION_ATTRIB_NAME);
    GLuint colorLocation = glGetAttribLocation(drawingContext.basicProgramID, GLSL_COLOR_ATTRIB_NAME);
    /* SETUP VAO FOR TRIANGLE */
    glGenVertexArrays(1, &drawingContext.triangle.vao);
    enable_vertex_array(&drawingContext.triangle, positionLocation, triangleVerteces, sizeof(triangleVerteces));
    enable_vertex_array(&drawingContext.triangle, colorLocation, triangleColor, sizeof(triangleColor));

    /* SETUP VAO FOR RECTANGLE */
    glGenVertexArrays(1, &drawingContext.rectangle.vao);
    enable_vertex_array(&drawingContext.rectangle, positionLocation, rectangleVerteces, sizeof(rectangleVerteces));
}

int cproc_init_window(struct CprocWindow *window, unsigned int width, unsigned int height){
    drawingContext.width = width;
    drawingContext.height = height;
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        return INIT_WINDOW_ERROR;
    }
    window->window = SDL_CreateWindow("Cprocessing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
    if(!window->window){
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return INIT_WINDOW_ERROR;
    }
    window->context = SDL_GL_CreateContext(window->window);
    if(!window->context){
        printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
        return INIT_WINDOW_ERROR;
    }
    glewExperimental = GL_TRUE; 
    GLenum glewError = glewInit();
    if(glewError != GLEW_OK){
        printf( "Error initializing GLEW! %s\n", glewGetErrorString( glewError ) );
        return INIT_WINDOW_ERROR;
    }
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    printf("OPENGL VERSION: %d.%d\n", major, minor);
    init_cprocessing();
    return 0;
}

int cproc_close_window(struct CprocWindow *window){
    SDL_DestroyWindow(window->window);
    SDL_Quit();
    return 0;
}

void cproc_loop(struct CprocWindow *window, LoopCallback callback){
    bool quit = false;
    SDL_Event e;
    while( !quit ){
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 ){
            if( e.type == SDL_QUIT ) quit = true;
        }

        if(callback) callback(window);
        SDL_GL_SwapWindow(window->window);
        //SDL_Delay(10);
    }
}

/* DRAWING UTILITIES FUNCTION IMPLEMENTATIONS */
void fillStyle(GLfloat red, GLfloat green, GLfloat blue){
    drawingContext.r = red;
    drawingContext.g = green;
    drawingContext.b = blue;
}

static void drawHelper(GLuint program, struct Shape shape, struct Vec3f translation, struct Vec3f scale, GLuint vertexCount){
    glUseProgram(program);
    glBindVertexArray(shape.vao);
    glVertexAttrib3f(glGetAttribLocation(program, GLSL_COLOR_ATTRIB_NAME), 
        drawingContext.r, drawingContext.g, drawingContext.b);
    GLuint mvLocation = glGetUniformLocation(drawingContext.basicProgramID, GLSL_MVP_UNIFORM_NAME);
    struct Mat4f mv = cglm_identity4();
    mv = cglm_multiply4(mv, cglm_translate4(translation.x, translation.y, translation.z));
    mv = cglm_multiply4(mv, cglm_scale4(scale.x, scale.y, scale.z));
    glUniformMatrix4fv(mvLocation, 1, GL_TRUE, mv.matrix);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void fillTriangle(int x, int y, int base, int height){
    struct Vec3f translation = {.x = 2 * x/(float)drawingContext.width, .y = 2 * y/(float)drawingContext.height, .z = 0.f};
    struct Vec3f scale = {.x = 2 * base/(float)drawingContext.width, .y = 2 * height/(float)drawingContext.height, .z = 1.f};
    drawHelper(drawingContext.basicProgramID, drawingContext.triangle, translation, scale, 3);
}

void fillRect(int x, int y, int width, int height){
    struct Vec3f translation = {.x = 2 * x/(float)drawingContext.width, .y = 2 * y/(float)drawingContext.height, .z = 0.f};
    struct Vec3f scale = {.x = 2 * width / (float)drawingContext.width, .y = 2 * height / (float)drawingContext.height, .z = 1.f};
    drawHelper(drawingContext.basicProgramID, drawingContext.rectangle, translation, scale, 6);
}