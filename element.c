// This file is part of Cyberun, a 2D platformer game.
//this file contains the implementation of the basic_element 
#include <stdlib.h>
#include "element.h"



element *create_element(int x, int y, int vx, int vy, int width, int height, ALLEGRO_BITMAP *sprite) {
    element *new_element = (element *)malloc(sizeof(element));
    if(!new_element) {
        return NULL; // Check for memory allocation failure
    }

    // Initialize the element's properties

    new_element->x = x; // Initial horizontal position
    new_element->y = y; // Initial vertical position
    new_element->vx = vx; // Initial horizontal velocity
    new_element->vy = vy; // Initial vertical velocity
    new_element->width = width;
    new_element->height = height;
    
    if(sprite) {
        new_element->sprite = sprite; // Assign the sprite if provided
    } else {
        new_element->sprite = NULL; // No sprite assigned
    }


    return new_element;
}

void destroy_element(element *elem) {
    if (elem) {
        free(elem);
    }
}