#ifndef _characters_
#define _characters_

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "joystick.h"

#define RIGHT 0
#define LEFT 1
#define UP 2
#define DOWN 3

typedef struct character{
    float x, y; // Position of the character
    float dx, dy; // Velocity of the character
    joystick *control; // Joystick control for the character
    ALLEGRO_BITMAP *sprite; // Sprite for the character (optional)

    void (*moviment)(struct character *self, char direction); // Function pointer for updating position
} player;

player* player_create(float x, float y, ALLEGRO_BITMAP *sprite); // Function to create a new character
void player_mov(player *self, char direction); // Function to update character position based on joystick input
void player_destroy(player *self); // Function to destroy a character and free resources

#endif