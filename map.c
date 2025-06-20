
#include "map.h"


void create_map(element **structures_vector, int max_x, int max_y, int num_structures, ALLEGRO_BITMAP *all_background, ALLEGRO_BITMAP *floor){

    al_set_target_bitmap(all_background);

    al_draw_bitmap_region(floor, 0, 0, 32, 32, 0, 1024 - 48, 0);
    al_draw_bitmap_region(floor, 0, 0, 32, 16, 0, 1024 - 16, 0);
    int i;
    for(i = 1; i < 200; i++) { // Draw the floor
        al_draw_bitmap_region(floor, 32, 0, 32, 32, i * 32, 1024 - 48, 0);
        al_draw_bitmap_region(floor, 32, 0, 32, 16, i * 32, 1024 - 16, 0); //trocar essa sprite 
    }
    al_draw_bitmap_region(floor, 64, 0, 32, 32, i * 32, 1024 - 48, 0);
    al_draw_bitmap_region(floor, 64, 0, 32, 16, i * 32, 1024 - 16, 0);

    // Draw the platforms

    structures_vector[0] = create_element(1380, 460, 0, 0, 2 * 192, 0, floor); // Create the first platform

    structures_vector[1] = create_element(2048, 460, 0, 0, 2 * 64, 0, floor); // Create the second platform

    structures_vector[2] = create_element(2264, max_y - 500, 0, 0, 2 * 192, 0, floor); // Create the third platform


    structures_vector[3] = create_element(4000, max_y - 200, 0, 0, 2 * 256, 0, floor); // Create the fourth platform


    
}

void destroy_structures(element **structures_vector, int num_structures) {
    if (structures_vector) {
        for (int i = 0; i < num_structures; i++) { // Loop through each platform
            destroy_element(structures_vector[i]); // Destroy each platform element
        }
    }
}

void default_relative_floor(int width, element *relative_floor){
    relative_floor->x = 0;
    relative_floor->y = FLOOR_LEVEL; // Set the relative floor level
    relative_floor->vx = 0;
    relative_floor->vy = 0;
    relative_floor->width = width; // Set the width of the relative floor
    relative_floor->height = 0; // Set the height of the relative floor
}

void update_relative_floor(element *new_floor, int map_ajustment, element *relative_floor) {
    relative_floor->x = new_floor->x - map_ajustment; // Update the relative floor position
    relative_floor->y = new_floor->y; // Update the relative floor position
    relative_floor->vx = new_floor->vx; // Update the horizontal velocity
    relative_floor->vy = new_floor->vy; // Update the vertical velocity
    relative_floor->width = new_floor->width; // Update the width of the relative floor; // Update the height of the relative floor
}