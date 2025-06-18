#ifndef _characters_
#define _characters_

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "joystick.h"
#include "gun.h"

#define RIGHT 0
#define LEFT 1
#define UP 2
#define DOWN 3

#define gravity 5 // Define gravity effect on the player

typedef struct character{
    float x, y; // Position of the character
    float vx, vy; // Velocity of the character
    int frame; // Current frame for animation
    int size;
    char face; // Direction the character is facing (0 = right, 1 = left, 2 = up, 3 = down)

    char sandevistan; // Flag for sandevistan mode (0 = off, 1 = on)

    char fix_camera_right; // Flag to fix camera on character (0 = not fixed, 1 = fixed)
    char fix_camera_left; // Flag to fix camera on character (0 = not fixed, 1 = fixed)
    //flags for character state
    char jumping; // Jump state (0 = not jumping, 1 = jumping)
    char falling; // Fall state (0 = not falling, 1 = falling)
    char squat; // Squat state (0 = not squatting, 1 = squatting)

    joystick *control; // Joystick control for the character
    pistol *gun; // Gun for the character (optional, can be NULL)
    ALLEGRO_BITMAP *sprite; // Sprite for the character (optional)

    void (*walk)(struct character *self, char direction); // Function pointer for updating position
    void (*jump)(struct character *self, char direction); // Function pointer for jumping
    void (*fall)(struct character *self, char direction); // Function pointer for falling
    void (*shoot)(struct character *self); // Function pointer for shooting

} player;

player* player_create(float x, float y, ALLEGRO_BITMAP *sprite, ALLEGRO_BITMAP *bullet_art); // Function to create a new character
void player_mov_x(player *self, char direction); // Function to update character position based on joystick input
void player_mov_y(player *self, char direction);
void character_shot(player *self); // Function to handle character shooting
void player_destroy(player *self); // Function to destroy a character and free resources

#endif