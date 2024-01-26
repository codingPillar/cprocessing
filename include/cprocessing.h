#ifndef C_PROCESSING_H
#define C_PROCESSING_H

#include <stdint.h>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define INIT_WINDOW_ERROR -1

#define TWO_PI (2.f * 3.1415f)

struct CprocWindow{
    SDL_Window *window;
    SDL_GLContext context;
};

typedef int (*LoopCallback)(struct CprocWindow *window);

/* WINDOW CONSTRUCTION */
int cproc_init_window(struct CprocWindow *window, unsigned int width, unsigned int height);
int cproc_close_window(struct CprocWindow *window);
void cproc_loop(struct CprocWindow *window, LoopCallback callback);

/* DRAWING UTILITIES (RESPECTING THE PROCESSING NAMING SCHEME) */
void fillStyle(GLfloat red, GLfloat green, GLfloat blue);
void fillTriangle(int x, int y, int base, int height);
void fillRect(int x, int y, int width, int height);
void fillEllipse(int x, int y, float a, float b);

#endif //C_PROCESSING_H