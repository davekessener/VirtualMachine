#include <cassert>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include <utility>

static const int SCREEN_WIDTH = 512;
static const int SCREEN_HEIGHT = 512;
static const int SCREEN_BPP = 32;

GLuint tex(0);

int loadTexture(const unsigned char *data, int w, int h)
{
    GLuint object;
//    SDL_Surface *image = IMG_Load(fileName);

//    SDL_DisplayFormatAlpha(image);
    
	glGenTextures(1, &object);
    glBindTexture(GL_TEXTURE_2D, object);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    
//	SDL_FreeSurface(image);
    
	return object;
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 1.0, -1.0, 1.0);
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, tex);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex2f(0, 0);
        glTexCoord2f(1, 0);
        glVertex2f(SCREEN_WIDTH, 0);
        glTexCoord2f(1, 1);
        glVertex2f(SCREEN_WIDTH, SCREEN_HEIGHT);
        glTexCoord2f(0, 1);
        glVertex2f(0, SCREEN_HEIGHT);
    glEnd();
    glFlush();
}

typedef std::pair<const unsigned char *, int> param_t;

int gl_main(param_t *(*fn)(void))
{
    SDL_Init(SDL_INIT_EVERYTHING);

	//SDL_Surface *screen = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE | SDL_OPENGL);
	SDL_Window *win(SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL));
	SDL_GL_CreateContext(win);
    bool running = true;
    Uint32 start;
    SDL_Event event;

    init();
    
	while(running)
	{
        start = SDL_GetTicks();

		param_t *p = fn();
		if(p)
		{
			if(tex > 0) glDeleteTextures(1, &tex);
			tex = loadTexture(p->first, p->second & 0xffff, p->second >> 16);
		}
        draw();

        while(SDL_PollEvent(&event))
		{
            switch(event.type)
			{
                case SDL_QUIT:
                    running = false;
                    break;
            }
        }

        //SDL_GL_SwapBuffers();
		SDL_GL_SwapWindow(win);
        
		if(1000 / 60 > SDL_GetTicks() - start)
		{
            SDL_Delay(1000 / 60 - (SDL_GetTicks() - start));
		}
    }

    SDL_Quit();
    
	return 0;
}

