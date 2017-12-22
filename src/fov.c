#include <stdint.h>
#include "level.h"
#include "fov.h"
#include <assert.h>

shadow known_shadows[10];
uint32_t shadow_count = 0;

uint32_t
first (uint32_t source, uint32_t distance)
{
  if (source >= distance)
    return source - distance;
  else
    return 0;
}

uint32_t
second (uint32_t source, uint32_t distance)
{
  if (source >= distance)
    return distance - 1;
  else
    return source;
}

fov_cell
local_cell (uint32_t sector, fov_cell creature, fov_cell translate)
{
  switch (sector)
    {
    case 1:
      {
        fov_cell level_cell = {creature.x + translate.x,
                               creature.y - translate.y};
        return level_cell;
      }
      break;
    case 2:
      {
        fov_cell level_cell = {creature.x + translate.y,
                               creature.y - translate.x};
        return level_cell;
      }
      break;
    case 3:
      {
        fov_cell level_cell = {creature.x + translate.y,
                               creature.y + translate.x};
        return level_cell;
      }
      break;
    case 4:
      {
        fov_cell level_cell = {creature.x + translate.x,
                               creature.y + translate.y};
        return level_cell;
      }
      break;
    case 5:
      {
        fov_cell level_cell = {creature.x - translate.x,
                               creature.y + translate.y};
        return level_cell;
      }
      break;
    case 6:
      {
        fov_cell level_cell = {creature.x - translate.y,
                               creature.y + translate.x};
        return level_cell;
      }
      break;
    case 7:
      {
        fov_cell level_cell = {creature.x - translate.y,
                               creature.y - translate.x};
        return level_cell;
      }
      break;
    case 8:
      {
        fov_cell level_cell = {creature.x - translate.x,
                               creature.y - translate.y};
        return level_cell;
      }
      break;
    default:
      assert(false);
      fov_cell c = {0,0};
      return c;
    }
}

void
calc_fov (uint32_t x, uint32_t y, uint32_t level_fov[][LEVEL_HEIGHT])
{
  /* Reset FOV to default state (hidden). */
  for (uint32_t x = 0; x < LEVEL_WIDTH; x++)
    for (uint32_t y = 0; y < LEVEL_HEIGHT; y++)
      level_fov[x][y] = 0;

  level_fov[x][y] = 1;

  for (uint32_t sector = 1; sector <= 8; sector++)
    {
      bool previous_block = false;
      float start = 0.0;
      float end = 0.0;

      for (int cell_y = 1; cell_y < FOV_DISTANCE; cell_y++)
        {
          previous_block = false;

          for (int cell_x = 0; cell_x <= cell_y + 1; cell_x++)
            {
              fov_cell player_cell = {x, y};
              fov_cell translate_cell = {cell_x, cell_y};
              fov_cell level_cells = local_cell (sector,
                                                 player_cell, translate_cell);
            }
        }
    }

  /* TODO: Move to function: Apply visibility to level_fov. */
  /* for (uint32_t f_x = x_1; f_x <= x_2; f_x++) */
  /*   for (uint32_t f_y = y_1; f_y <= y_2; f_y++) */
  /*     level_fov[f_x][f_y] = 10; */

}
