#include <stdint.h>
#include "level.h"
#include "fov.h"

void
calc_fov (uint32_t x, uint32_t y, uint32_t level_fov[][LEVEL_HEIGHT])
{
  /* Reset FOV to default state (hidden). */
  for (uint32_t x = 0; x < LEVEL_WIDTH; x++)
    for (uint32_t y = 0; y < LEVEL_HEIGHT; y++)
      level_fov[x][y] = 0;

  /* Cast visibility out in four directions.
     Determine visibility rectangle. */
  uint32_t x_1 = 0; 
  if (x >= FOV_DISTANCE)
    x_1 = x - FOV_DISTANCE; 

  uint32_t x_2 = x + FOV_DISTANCE;
  if (x_2 >= LEVEL_WIDTH)
    x_2 = LEVEL_WIDTH - 1;

  uint32_t y_1 = 0;
  if (y >= FOV_DISTANCE)
    y_1 = y - FOV_DISTANCE;

  uint32_t y_2 = y + FOV_DISTANCE;
  if (y_2 >= LEVEL_HEIGHT)
    y_2 = LEVEL_HEIGHT - 1;

  /* TODO: Move to function: Apply visibility to level_fov. */
  for (uint32_t f_x = x_1; f_x <= x_2; f_x++)
    for (uint32_t f_y = y_1; f_y <= y_2; f_y++)
      level_fov[f_x][f_y] = 10;
}
