#include <stdlib.h>
#include <stdint.h>

#include "pt_console.h"
#include "level.h"
#include "game.h"

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
