#include <stdint.h>
#include "level.h"
#include "fov.h"

float
pifagor (uint32_t x_1, uint32_t x_2, uint32_t y_1, uint32_t y_2)
{
  return sqrt (pow (x_2 - x_1, 2) + pow (y_2 - y_1, 2));
}

void
calc_fov (uint32_t x, uint32_t y, uint32_t level_fov[][LEVEL_HEIGHT])
{
  bool previous_block = false;
  float start = 0.0;
  float end = 0.0;
  shadow know_shadows[10];
  uint32_t count = 0;

  for (int cell_y = 1; cell_y < FOV_DISTANCE; cell_y++)
    {
      previous_block = false;

      for (int cell_x; cell_x <= distance + 1; cell_x++)
        {
          /* Cell within view distance. */
          if (pifagor (x, y, cell_x, cell_y) <= FOV_DISTANCE)
            if (! shadow (cell_x, cell_y))
              {
                mark_visible (cell_x, cell_y);
                if (block (cell_x, cell_y))
                  if (! previous_block)
                    start = slope (x, y, cell_x - 0.5, cell_y);
                  else
                    if (previous_block)
                      {
                        end = slope (x, y, cell_x + 0.5, cell_y);
                        shadow new_shadow = {start, end};
                        add_shadow (new_shadow);
                      }
              }
          /* Cell within known shadow. */
        }

      if (previous_block)
        {
          end = slope (x, y, cell_x + 0.5, cell_y);
          shadow new_shadow = {start, end};
          add_shadow (new_shadow);
        }
    }

  /* TODO: Move to function: Apply visibility to level_fov. */
  for (uint32_t f_x = x_1; f_x <= x_2; f_x++)
    for (uint32_t f_y = y_1; f_y <= y_2; f_y++)
      level_fov[f_x][f_y] = 10;
}
