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

bool
can_move (position next_position)
{
  bool allow_move = true;

  if ((next_position.x >= 0)
      && (next_position.x < NUM_COLS)
      && (next_position.y < NUM_ROWS))
    for (uint32_t i = 1; i < MAX_GO; i++)
      {
        position new_position = position_components[i];
        if ((new_position.object_id > 0)
            && (new_position.x == next_position.x)
            && (new_position.y == next_position.y))
          if (physical_components[i].block_movement == true)
            allow_move = false;
      }
  else
    allow_move = false;

  return allow_move;
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

  SDL_Texture *screen = SDL_CreateTexture (renderer, SDL_PIXELFORMAT_RGBA8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           SCREEN_WIDTH, SCREEN_HEIGHT);
  if (screen == NULL) SDL_Log ("SDL_CreateTexture failed: %s\n", SDL_GetError ());

  PT_Console *console = PT_ConsoleInit (SCREEN_WIDTH, SCREEN_HEIGHT,
                                        NUM_ROWS, NUM_COLS);
  if (console == NULL) printf ("Initialize console failed. \n");

  PT_ConsoleSetBitmapFont (console, "terminal16x16.png", 0, 16, 16);

  game_object *wall = create_game_object ();
  position wall_position = {wall->id, 30, 25};
  add_component_to_game_object (wall, COMPONENT_POSITION, &wall_position);
  visibility wall_visibility = {wall->id, '#', 0xffffffff, 0x000000ff};
  add_component_to_game_object (wall, COMPONENT_VISIBILITY, &wall_visibility);
  physical wall_physic = {wall->id, true, true};
  add_component_to_game_object (wall, COMPONENT_PHYSICAL, &wall_physic);

  game_object *player = create_game_object ();
  position player_position = {player->id, 25, 25};
  add_component_to_game_object (player, COMPONENT_POSITION, &player_position);
  visibility player_visibility = {player->id, '@', 0xffff00ff, 0x000000ff};
  add_component_to_game_object (player, COMPONENT_VISIBILITY, &player_visibility);
  physical player_physic = {player->id, true, true};
  add_component_to_game_object (player, COMPONENT_PHYSICAL, &player_physic);

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
                case SDLK_UP:;
                  position up = {player_position->object_id,
                                 player_position->x,
                                 player_position->y - 1};
                  if (can_move (up))
                    add_component_to_game_object (player,
                                                  COMPONENT_POSITION,
                                                  &up);
                  break;
                case SDLK_DOWN:;
                  position down = {player_position->object_id,
                                   player_position->x,
                                   player_position->y + 1};
                  if (can_move (down))
                    add_component_to_game_object (player,
                                                  COMPONENT_POSITION,
                                                  &down);
                  break;
                case SDLK_LEFT:;
                  position left = {player_position->object_id,
                                   player_position->x - 1,
                                   player_position->y};
                  if (can_move (left))
                    add_component_to_game_object (player,
                                                  COMPONENT_POSITION,
                                                  &left);
                  break;
                case SDLK_RIGHT:;
                  position right = {player_position->object_id,
                                    player_position->x + 1,
                                    player_position->y};
                  if (can_move (right))
                    add_component_to_game_object (player,
                                                  COMPONENT_POSITION,
                                                  &right);
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
