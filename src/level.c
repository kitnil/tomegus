#include <stdlib.h>
#include <stdint.h>

#include "pt_console.h"
#include "level.h"
#include "game.h"

point
rectangle_random_point (PT_Rect rectangle)
{
  uint32_t point_x = (rand () % rectangle.w) + rectangle.x;
  uint32_t point_y = (rand () % rectangle.h) + rectangle.y;
  point pair = {point_x, point_y};
  return pair;
}

/* Determine if all cells within the given rectangle are filled. */
bool
carve_level_room (uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
  for (uint8_t i = x - 1; i < x + (width + 1); i++)
    for (uint8_t j = y - 1; j < y + (height + 1); j++)
      if (level_cells[i][j] == false)
        return false;

  /* TODO: Move to function: Carve out the room. */
  for (uint8_t i = x; i < x + width; i++)
    for (uint8_t j = y; j < y + height; j++)
      level_cells[i][j] = false;

  return true;
}

void
carve_level_room_hallway_horizontal (point from, point to)
{
  uint32_t first, last;

  if (from.x < to.x)
    {
      first = from.x;
      last = to.x;
    }
  else
    {
      first = to.x;
      last = from.x;
    }

  for (uint32_t x = first; x <= last; x++)
    level_cells[x][from.y] = false;
}

void
carve_level_room_hallway_vertical (point from, point to)
{
  uint32_t first, last;

  if (from.y < to.y)
    {
      first = from.y;
      last = to.y;
    }
  else
    {
      first = to.y;
      last = from.y;
    }

  for (uint32_t y = first; y <= last; y++)
    level_cells[from.x][y] = false;
}

/* Create a player in the level. */
void
init_player (game_object *player)
{
  for (;;)
    {
      uint32_t x = rand () % LEVEL_WIDTH;
      uint32_t y = rand () % LEVEL_HEIGHT;

      if (level_cells[x][y] == false)
        {
          position player_position = {player->id, 25, 25};
          add_component_to_game_object (player, COMPONENT_POSITION,
                                        &player_position);
          break;
        }
    }
}

/* Clean all except the player. */
void
clean_level (game_object *player)
{
  for (uint32_t i = 0; i < MAX_GO; i++)
    if ((game_objects[i].id != player->id)
        && (game_objects[i].id != UNUSED))
      destroy_game_object (&game_objects[i]);
}

/* Mark all level cells as `filled'. */
void
init_level ()
{
  /* TODO: Move to function: Carve out non-overlapping rooms that are
     randomly placed, and of random size. */

  PT_Rect level_rooms[100];
  bool done_rooms = false;
  uint32_t used_cells = 0;
  uint32_t room_count = 0;

  /* TODO: Move to function: Mark all the map cells as "filled". */
  for (uint32_t x = 0; x < LEVEL_WIDTH; x++)
    for (uint32_t y = 0; y < LEVEL_HEIGHT; y++)
      level_cells[x][y] = true;

  while (! done_rooms)
    {
      /* TODO: Move to function: Generate a random width/height for a
         room. */
      uint32_t width = (rand () % 17) + 3;
      uint32_t height = (rand () % 17) + 3;

      uint32_t x = rand () % (LEVEL_WIDTH - width - 1);
      uint32_t y = rand () % (LEVEL_HEIGHT - height - 1);

      if (x == 0) x = 1;
      if (y == 0) y = 1;

      bool success = carve_level_room (x, y, width, height);
      if (success)
        {
          PT_Rect r = {x, y, width, height};
          level_rooms[room_count] = r;
          room_count += 1;
          used_cells += (width * height);
        }

      carve_level_room (x, y, width, height);

      /* TODO: Move to function: Exit condition - more that desired %
         of cells in use. */
      if (((float) used_cells / (float) (LEVEL_HEIGHT * LEVEL_WIDTH)) > 0.45)
        done_rooms = true;
    }

  /* TODO: Join all rooms with corridors, so that all rooms are
     reachable. */
  for (uint32_t r = 1; r < room_count; r++)
    {
      PT_Rect from = level_rooms[r - 1];
      PT_Rect to = level_rooms[r];

      /* Join two rooms via random points. */
      point from_point = rectangle_random_point (from);
      point to_point = rectangle_random_point (to);
      point middle_point_horizontal = {to_point.x, from_point.y};
      point middle_point_vertical = {from_point.x, to_point.y};

      /* TODO: Make even number function. */
      /* Move horizontal then vertical else vertical then horizontal. */
      if (rand () % 2 == 0)
        {
          carve_level_room_hallway_horizontal (from_point,
                                               middle_point_horizontal);
          carve_level_room_hallway_vertical (middle_point_horizontal,
                                             to_point);
        }
      else
        {
          carve_level_room_hallway_vertical (from_point, middle_point_vertical);
          carve_level_room_hallway_horizontal (middle_point_vertical, to_point);
        }
    }

  /* TODO: Move to function: Generate a level. */
  for (uint32_t x = 0; x < LEVEL_WIDTH; x++)
    for (uint32_t y = 0; y < LEVEL_HEIGHT; y++)
      if (level_cells[x][y] == true)
        add_wall (x, y);

  /* TODO: Move to function: Place a player. */
}

void
add_wall (uint8_t x, uint8_t y)
{
  game_object *wall = create_game_object ();
  position wall_position = {wall->id, x, y};
  add_component_to_game_object (wall, COMPONENT_POSITION, &wall_position);
  visibility wall_visibility = {wall->id, '#', 0xffffffff, 0x000000ff};
  add_component_to_game_object (wall, COMPONENT_VISIBILITY, &wall_visibility);
  physical wall_physic = {wall->id, true, true};
  add_component_to_game_object (wall, COMPONENT_PHYSICAL, &wall_physic);
}
