#ifndef __MAP__ 																																//Guardas de inclusão (!)
#define __MAP__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>

#include "element.h"

#define FLOOR_LEVEL 640 // Define the floor level for the game


void create_map(element **structures_vector, int max_y, ALLEGRO_BITMAP *all_background, ALLEGRO_BITMAP *floor);
void destroy_structures(element **structures_vector, int num_structures);
void default_relative_floor(int width, element *relative_floor);
void update_relative_floor(element *new_floor, int map_ajustment, element *relative_floor);

#endif