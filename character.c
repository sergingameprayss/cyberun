#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>

#include "character.h"
#include "joystick.h"

//MOVIMENTAÇÃO NAO FUNCIONA DIAGONAL

#define MAX_X 1280 // Define maximum X coordinate for boundary checks
#define MAX_Y 720  // Define maximum Y coordinate for boundary checks
#define STEP 5 // Define step size for movement

player* player_create(float x, float y, ALLEGRO_BITMAP *sprite) {
    player *new_player = (player*) malloc(sizeof(player));
    if (!new_player) return NULL; // Check for memory allocation failure

    new_player->x = x;
    new_player->y = y;
    new_player->dx = 0;
    new_player->dy = 0;
    new_player->control = joystick_create();
    if (!new_player->control) {
        free(new_player);
        return NULL; // Check for joystick creation failure
    }
    
    if(sprite) {
        new_player->sprite = sprite; // Assign the sprite if provided
    } else {
        new_player->sprite = NULL; // No sprite assigned
    }

    new_player->moviment = player_mov; // Assign the movement function
    return new_player;
}

void player_mov(player *self, char direction) {
    self->dx = 0; // Reset velocity
    self->dy = 0; // Reset velocity
    // Update velocity based on joystick input

    if (direction == LEFT) {
        if ((self->x - 1*STEP) - 48 >= 0) self->x = self->x - 1*STEP;
    } 
    if (direction == RIGHT) {
        if ((self->x + 1*STEP) + 48 <= MAX_X) self->x = self->x + 1*STEP;
    }
    //if (self->control->up) {
    //    if ((self->y - 1*SQUARE_STEP) - 10 >= 0) self->y = self->y - 1*SQUARE_STEP;
    //}
    //if (self->control->down) {
     //   if ((self->y + 1*SQUARE_STEP) + 10 <= MAX_Y) self->y = self->y + 1*SQUARE_STEP;
   // }
}


void player_destroy(player *self) {
    if (self) {
        joystick_destroy(self->control);
        free(self);
    }
}
