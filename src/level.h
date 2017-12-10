#ifndef LEVEL_H
#define LEVEL_H

#include <stdbool.h>
#include <game.h>

#define LEVEL_WIDTH 80
#define LEVEL_HEIGHT 40
#define VISIBLE 0

uint32_t level_fov[LEVEL_WIDTH][LEVEL_HEIGHT];

bool level_cells[LEVEL_WIDTH][LEVEL_HEIGHT];

void clean_level (game_object *player);
void init_level ();
void init_player ();
void add_wall (uint8_t x, uint8_t y);

#endif
