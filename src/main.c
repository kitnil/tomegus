#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

void render_screen (SDL_Renderer *renderer, SDL_Texture *screen)
{
  uint32_t *pixels = calloc (SCREEN_WIDTH * SCREEN_HEIGHT, sizeof (uint32_t));

  if (screen) SDL_DestroyTexture (screen);

  SDL_UpdateTexture (screen, NULL, pixels, SCREEN_WIDTH * sizeof (uint32_t));
  SDL_RenderClear (renderer);
  SDL_RenderCopy (renderer, screen, NULL, NULL);
  SDL_RenderPresent (renderer);

  if (pixels) free (pixels);
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
      render_screen (renderer, screen);
    }

  SDL_DestroyRenderer (renderer);
  SDL_DestroyWindow (window);
  atexit(SDL_Quit);

  return 0;
}
