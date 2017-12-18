#ifndef FOV_H
#define FOV_H

#define FOV_DISTANCE 5

void calc_fov (uint32_t x, uint32_t y, uint32_t level_fov[][LEVEL_HEIGHT]);

typedef struct
{
  uint32_t x, y;
} fov_cell;

typedef struct
{
  float start;
  float end;
} shadow;

#endif
