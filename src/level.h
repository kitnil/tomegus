#ifndef LEVEL_H
#define LEVEL_H

#include <stdbool.h>

#define LEVEL_WIDTH 80
#define LEVEL_HEIGHT 40

bool level_cells[LEVEL_WIDTH][LEVEL_HEIGHT];

void init_level ();
void add_wall (uint8_t x, uint8_t y);

#endif
