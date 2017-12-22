/* https://en.wikipedia.org/wiki/Entity%E2%80%93component%E2%80%93system */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "game.h"

/* Initialize world. */
void
init_world ()
{
  for (uint32_t i = 0; i < MAX_GO; i++)
    {
      game_objects[i].id = UNUSED;
      position_components[i].object_id = UNUSED;
      visibility_components[i].object_id = UNUSED;
      physical_components[i].object_id = UNUSED;
    }
}

game_object *
create_game_object ()
{
 game_object *go = NULL;

  for (uint32_t i = 0; i < MAX_GO; i++)
    if (game_objects[i].id == UNUSED)
      {
        go = &game_objects[i];
        go->id = i;
        break;
      }

  assert (go != NULL);              /* Have we run out of game objects? */

  for (uint32_t i = 0; i < COMPONENT_COUNT; i++)
    go->components[i] = NULL;

  return go;
}

/* Find the next available object space. */
void
add_component_to_game_object (game_object *object,
                              game_component component,
                              void *component_data)
{
  assert (object->id != UNUSED);

  if (component == COMPONENT_POSITION)
    {
      position *position_component = &position_components[object->id];
      position *position_data = (position *) component_data;
      position_component->object_id = object->id;
      position_component->x = position_data->x;
      position_component->y = position_data->y;
      object->components[component] = position_component;
    }
  else if (component == COMPONENT_VISIBILITY)
    {
      visibility *visibility_component = &visibility_components[object->id];
      visibility *visibility_data = (visibility *) component_data;
      visibility_component->object_id = object->id;
      visibility_component->glyph = visibility_data->glyph;
      visibility_component->foreground_color = visibility_data->foreground_color;
      visibility_component->background_color = visibility_data->background_color;
      object->components[component] = visibility_component;
    }
  else if (component == COMPONENT_PHYSICAL)
    {
      physical *physical_component = &physical_components[object->id];
      physical *physical_data = (physical *) component_data;
      physical_component->object_id = object->id;
      physical_component->block_sight = physical_data->block_sight;
      physical_component->block_movement = physical_data->block_movement;
      object->components[component] = physical_component;
    }
  else
    assert (1 == 0);
}

/* TODO: Clean up any components. */
void
destroy_game_object (game_object *object)
{
  position_components[object->id].object_id = UNUSED;
  visibility_components[object->id].object_id = UNUSED;
  physical_components[object->id].object_id = UNUSED;
  object->id = UNUSED;
}

void *
get_component_for_game_object (game_object *object,
                               game_component component)
{
  return object->components[component];
}

game_object *
object_position (uint32_t x, uint32_t y)
{
  for (uint32_t i = 0; i < MAX_GO; i++)
    {
      position new_position = position_components[i];

      if (new_position.object_id != UNUSED
          && new_position.x == x
          && new_position.y == y)
        return &game_objects[i];
    }

  return NULL;
}
