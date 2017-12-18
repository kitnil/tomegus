#include <stdint.h>
#include <libguile.h>
#include "level.h"
#include "fov.h"

void
calc_fov (uint32_t x, uint32_t y, uint32_t level_fov[][LEVEL_HEIGHT])
{
  /* http://www.lonelycactus.com/guilebook/x220.html */
  scm_init_guile ();

  scm_c_primitive_load ("script.scm");

  /* TODO: return (EXIT_SUCCESS); */

  /* Reset FOV to default state (hidden). */
  for (uint32_t x = 0; x < LEVEL_WIDTH; x++)
    for (uint32_t y = 0; y < LEVEL_HEIGHT; y++)
      level_fov[x][y] = 0;

  /* Cast visibility out in four directions.
     Determine visibility rectangle. */
  uint32_t x_1 = scm_to_uint32
    (scm_call_2 (scm_variable_ref (scm_c_lookup ("left")),
                 scm_from_int (x),
                 scm_from_int (FOV_DISTANCE)));

  uint32_t x_2 = scm_to_uint32
    (scm_call_2 (scm_variable_ref (scm_c_lookup ("right")),
                 scm_from_uint32 (x + FOV_DISTANCE),
                 scm_from_int (LEVEL_WIDTH)));

  uint32_t y_1 = scm_to_uint32
    (scm_call_2 (scm_variable_ref (scm_c_lookup ("left")),
                 scm_from_uint32 (y),
                 scm_from_int (FOV_DISTANCE)));

  uint32_t y_2 = scm_to_uint32
    (scm_call_2 (scm_variable_ref (scm_c_lookup ("right")),
                 scm_from_uint32 (y + FOV_DISTANCE),
                 scm_from_int (LEVEL_HEIGHT)));

  /* TODO: Move to function: Apply visibility to level_fov. */
  for (uint32_t f_x = x_1; f_x <= x_2; f_x++)
    for (uint32_t f_y = y_1; f_y <= y_2; f_y++)
      level_fov[f_x][f_y] = 10;
}
