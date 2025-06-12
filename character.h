#ifndef _characters_
#define _characters_

#include <allegro5/allegro.h>
#include "joystick.h"

typedef struct character{
    float x, y; // Position of the character
    float dx, dy; // Velocity of the character
    joystick *control; // Joystick control for the character

    void (*moviment)(struct character *self); // Function pointer for updating position
} player;

player* player_create(float x, float y); // Function to create a new character
void player_mov(player *self); // Function to update character position based on joystick input
void player_destroy(player *self); // Function to destroy a character and free resources

#endif