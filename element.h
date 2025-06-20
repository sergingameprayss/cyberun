#ifndef _ELEMENTO_
#define _ELEMENTO_

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "gun.h"


typedef struct {
    int x, y; // Position of the element
    int vx, vy; // Velocity of the element
    int width, height; // Size of the element
    pistol *gun; // Pointer to the gun associated with the element (optional, can be NULL)
    ALLEGRO_BITMAP *sprite; // Bitmap for the element's sprite
} element;

element *create_element(int x, int y, int vx, int vy, int width, int height, ALLEGRO_BITMAP *sprite);

void destroy_element(element *elem);

#endif