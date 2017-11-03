#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>

#include "pt_console.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

#define NUM_COLS 80
#define NUM_ROWS 45

void render_screen (SDL_Renderer *renderer,
                    SDL_Texture *screen,
                    PT_Console *console)
{
  PT_ConsoleClear (console);
  PT_ConsolePutCharAt (console, '@', 10, 10, 0xFFFFFFFF, 0x000000FF);

  if (screen) SDL_DestroyTexture (screen);

  SDL_UpdateTexture (screen, NULL, console->pixels,
                     SCREEN_WIDTH * sizeof (uint32_t));
  SDL_RenderClear (renderer);
  SDL_RenderCopy (renderer, screen, NULL, NULL);
  SDL_RenderPresent (renderer);

  if (console->pixels) free (console->pixels);
}

int
main ()
{
  const char* PROJECT_NAME = "Tomegus";
  bool gameover = false;

  if (SDL_Init (SDL_INIT_VIDEO) != 0)
    {
      SDL_Log ("Unable to initialize SDL: %s", SDL_GetError ());
      return 1;
    }

  SDL_Window *window = SDL_CreateWindow (PROJECT_NAME,
                                         SDL_WINDOWPOS_UNDEFINED,
                                         SDL_WINDOWPOS_UNDEFINED,
                                         SCREEN_WIDTH, SCREEN_HEIGHT,
                                         0);
  SDL_Renderer *renderer = SDL_CreateRenderer (window,
                                               0,
                                               SDL_RENDERER_SOFTWARE);
  SDL_SetHint (SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  SDL_RenderSetLogicalSize (renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
  SDL_Texture *screen = SDL_CreateTexture (renderer,
                                           SDL_PIXELFORMAT_RGBA8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           SCREEN_HEIGHT,
                                           SCREEN_WIDTH);
  PT_Console *console = PT_ConsoleInit (SCREEN_WIDTH,
                                        SCREEN_HEIGHT,
                                        NUM_ROWS,
                                        NUM_COLS);
  PT_ConsoleSetBitmapFont (console, "terminal16x16.png", '0', 16, 16);

  while (!gameover)
    {
      SDL_Event event;
      while (SDL_PollEvent (&event) != 0)
        {
          if (event.type == SDL_QUIT)
            {
              gameover = true;
              break;
            }
        }
      render_screen (renderer, screen, console);
    }

  SDL_DestroyRenderer (renderer);
  SDL_DestroyWindow (window);
  atexit (SDL_Quit);

  return 0;
}
