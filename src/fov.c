#include <stdint.h>
#include "level.h"
#include "fov.h"
#include <assert.h>
#include <stddef.h>

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

void
add_shadow (shadow new_shadow)
{
  known_shadows[shadow_count] = new_shadow;
  shadow_count += 1;
}

float
slope_between (float x_1, float y_1, float x_2, float y_2)
{
  return (y_2 - y_1) / (x_2 - x_1);
}

bool
shadow_cell (float slope)
{
  for (uint8_t i = 0; i < shadow_count; i++)
    {
      shadow new_shadow = known_shadows[i];

      if (new_shadow.start <= slope && new_shadow.end >= slope)
        return true;
    }

  return false;
}

bool
cell_block (uint32_t x, uint32_t y)
{
  game_object *new_game_object = object_position (x, y);

  if (new_game_object != NULL)
    {
      physical *new_physical = (physical *)
        get_component_for_game_object (new_game_object, COMPONENT_PHYSICAL);

      if (new_physical->block_sight)
        return true;
    }

  return false;
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
              fov_cell level_cell = local_cell (sector,
                                                player_cell, translate_cell);

              if ((level_cell.x >= 0)
                  && (level_cell.x < LEVEL_WIDTH)
                  && (level_cell.y < LEVEL_HEIGHT))
                {
                  float cell_slope = slope_between (0, 0, cell_x, cell_y);

                  if (! shadow_cell (cell_slope))
                    {
                      level_fov[level_cell.x][level_cell.y] = 1;

                      if (cell_block (level_cell.x, level_cell.y))
                        if (previous_block == false)
                          start = slope_between (0, 0, cell_x - 0.5, cell_y);
                    }
                  else if (previous_block)
                    {
                      end = slope_between (0, 0, cell_x + 0.5, cell_y);

                      shadow new_shadow = {start, end};

                      add_shadow (new_shadow);
                    }
                }
            }
          if (previous_block)
            {
              end = slope_between (0, 0, cell_y + 1 + 0.5, cell_y);

              shadow new_shadow = {start, end};

              add_shadow (new_shadow);
            }
        }
    }
}
