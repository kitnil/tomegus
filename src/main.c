/* Some code from: http://www.willusher.io/sdl2%20tutorials/2013/08/17/lesson-2-dont-put-everything-in-main */

#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>

#include "pt_console.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define NUM_COLS 80
#define NUM_ROWS 45

void
render_screen (SDL_Renderer *renderer, SDL_Texture *screen, PT_Console *console)
{
  PT_ConsoleClear (console);
  PT_ConsolePutCharAt (console, '@', 10, 10, 0xffffffff, 0x000000ff);

  SDL_UpdateTexture (screen, NULL, console->pixels,
                     SCREEN_WIDTH * sizeof (uint32_t));
  SDL_RenderClear (renderer);
  SDL_RenderCopy (renderer, screen, NULL, NULL);
  SDL_RenderPresent (renderer);
}

int
main ()
{
  const char* PROJECT_NAME = "Tomegus";
  bool gameover = false;

  if (SDL_Init (SDL_INIT_VIDEO) < 0)
    {
      SDL_LogError (SDL_LOG_CATEGORY_APPLICATION,
                    "Couldn't initialize SDL: %s",
                    SDL_GetError());
      return 3;
    }

  /* TODO: SDL_WINDOW_RESIZABLE */
  SDL_Window *window = SDL_CreateWindow (PROJECT_NAME,
                                         SDL_WINDOWPOS_UNDEFINED,
                                         SDL_WINDOWPOS_UNDEFINED,
                                         SCREEN_WIDTH, SCREEN_HEIGHT,
                                         0);
  if (window == NULL) SDL_Log ("SDL_SetHint failed: %s\n", SDL_GetError ());

  SDL_Renderer *renderer = SDL_CreateRenderer (window, -1, SDL_RENDERER_SOFTWARE);
  if (renderer == NULL) SDL_Log ("SDL_Renderer failed: %s\n", SDL_GetError ());

  if (SDL_SetHint (SDL_HINT_RENDER_SCALE_QUALITY, "linear") == SDL_FALSE)
    SDL_Log ("SDL_SetHint failed: %s\n", SDL_GetError ());

  if (SDL_RenderSetLogicalSize (renderer, SCREEN_WIDTH, SCREEN_HEIGHT) < 0)
    SDL_Log ("SDL_RenderSetLogicalSize failed: %s\n", SDL_GetError ());

  /* TODO: SIGSEGV: SDL_TEXTUREACCESS_TARGET */
  SDL_Texture *screen = SDL_CreateTexture (renderer, SDL_PIXELFORMAT_RGBA8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           SCREEN_WIDTH, SCREEN_HEIGHT);
  if (screen == NULL) SDL_Log ("SDL_CreateTexture failed: %s\n", SDL_GetError ());

  PT_Console *console = PT_ConsoleInit (SCREEN_WIDTH, SCREEN_HEIGHT,
                                        NUM_ROWS, NUM_COLS);
  if (console == NULL) printf ("Initialize console failed. \n");

  PT_ConsoleSetBitmapFont (console, "terminal16x16.png", 0, 16, 16);

  /* Main loop. */
  while (!gameover)
    {
      /* Event handling. */
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

  /* Clean up. */
  SDL_DestroyRenderer (renderer);
  SDL_DestroyWindow (window);
  atexit (SDL_Quit);

  return 0;
}
