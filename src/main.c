#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>

#include "game.h"
#include "pt_console.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define NUM_COLS 80
#define NUM_ROWS 45

void
render_screen (SDL_Renderer *renderer, SDL_Texture *screen, PT_Console *console)
{
  PT_ConsoleClear (console);

  for (uint32_t i = 1; i < MAX_GO; i++)
    if (visibility_components[i].object_id > 0)
      {
        position *object_position = (position *)
          get_component_for_game_object (&game_objects[i], COMPONENT_POSITION);

        PT_ConsolePutCharAt (console, visibility_components[i].glyph,
                             object_position->x, object_position->y,
                             visibility_components[i].foreground_color,
                             visibility_components[i].background_color);
      }

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

  game_object *player = create_game_object ();
  position player_position = {player->id, 25, 25};
  add_component_to_game_object (player, COMPONENT_POSITION, &player_position);
  visibility player_visibility = {player->id, '@', 0x00ff00ff, 0x000000ff};
  add_component_to_game_object (player, COMPONENT_VISIBILITY, &player_visibility);

  game_object *wall = create_game_object ();
  position wall_position = {wall->id, 30, 25};
  add_component_to_game_object (wall, COMPONENT_POSITION, &wall_position);
  visibility wall_visibility = {wall->id, '#', 0xffffffff, 0x000000ff};
  add_component_to_game_object (wall, COMPONENT_VISIBILITY, &wall_visibility);

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
          if (event.type == SDL_KEYDOWN)
            {
              SDL_Keycode key = event.key.keysym.sym;
              position *player_position = (position *)
                get_component_for_game_object (player, COMPONENT_POSITION);
              switch (key)
                {
                case SDLK_UP:
                  if (player_position->y > 0)
                    player_position->y -= 1;
                  break;
                case SDLK_DOWN:
                  if (player_position->y < NUM_ROWS - 1)
                    player_position->y += 1;
                  break;
                case SDLK_LEFT:
                  if (player_position->x > 0)
                    player_position->x -= 1;
                  break;
                case SDLK_RIGHT:
                  if (player_position->x < NUM_COLS - 1)
                    player_position->x += 1;
                  break;
                case SDLK_ESCAPE:
                  gameover = true;
                  break;
                default:
                  break;
                }
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
