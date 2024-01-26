#include <stdio.h>

#include "../include/cprocessing.h"
#include "../include/cglm.h"

#include <GL/glew.h>

#define WIDTH 1080
#define HEIGHT 720

unsigned int time = 0;

int loop_callback(struct CprocWindow *window){
    (void) window;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    fillStyle(1.0f, 0.f, 0.f);
    {   /* TRIANGLE */
        fillTriangle(-WIDTH / 2.f + 200 , sinf(time/50.f) * HEIGHT / 2.f, 200.f, 200.f);
    }
    {   /* RECTANGLE */
        fillRect(WIDTH / 2.f - 200, sinf(time/50.f) * HEIGHT / 2.f, 200.f, 200.f);
    }
    {   /* ELLIPSE */
        fillEllipse(0.f, 0.f, 100.f, sinf(time/100.f) * 200.f);
    }
    time++;
    return 0;
}

int main(){
    printf("WELCOME RENDERER\n");

    struct CprocWindow window;
    if(cproc_init_window(&window, WIDTH, HEIGHT) < 0) return -1;

    /* MAIN LOOP */
    cproc_loop(&window, loop_callback);

    cproc_close_window(&window);

    return 0;
}