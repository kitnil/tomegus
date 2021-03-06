#ifndef GAME_H
#define GAME_H

#define MAX_COMPONENT_COUNT 100
#define MAX_GO 10000
#define UNUSED 100000

typedef enum
{
  COMPONENT_POSITION = 0,
  COMPONENT_VISIBILITY,
  COMPONENT_PHYSICAL,
  COMPONENT_HEALTH,
  COMPONENT_MOVEMENT,
  COMPONENT_COUNT
} game_component;

/* Entity. */
typedef struct
{
  uint32_t id;
  void *components[COMPONENT_COUNT];
} game_object;

/* Component definitions. */
typedef struct
{
  uint32_t object_id;
  uint8_t x, y;
} position;

typedef struct
{
  uint32_t object_id;
  unsigned char glyph;
  uint32_t foreground_color;
  uint32_t background_color;
  uint32_t seen;
} visibility;

typedef struct
{
  uint32_t object_id;
  bool block_movement;
  bool block_sight;
} physical;

typedef struct
{
  uint32_t x, y;
} point;

game_object *create_game_object ();

void add_component_to_game_object (game_object *object,
                                   game_component component,
                                   void *component_data);

void *get_component_for_game_object (game_object *object,
                                     game_component component);

visibility visibility_components[MAX_GO];
position position_components[MAX_GO];
game_object game_objects[MAX_GO];
physical physical_components[MAX_GO];

void init_world ();

void destroy_game_object (game_object *object);

game_object *object_position (uint32_t x, uint32_t y);

#endif  /* GAME_H */
