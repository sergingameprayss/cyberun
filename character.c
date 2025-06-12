#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>

#include "character.h"
#include "joystick.h"

//MOVIMENTAÇÃO NAO FUNCIONA DIAGONAL

#define MAX_X 1280 // Define maximum X coordinate for boundary checks
#define MAX_Y 720  // Define maximum Y coordinate for boundary checks
#define SQUARE_STEP 1 // Define step size for movement

player* player_create(float x, float y) {
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
    
    new_player->moviment = player_mov; // Assign the movement function
    return new_player;
}

void player_mov(player *self) {
    self->dx = 0; // Reset velocity
    self->dy = 0; // Reset velocity
    // Update velocity based on joystick input

    if (self->control->left) {
        if ((self->x - 1*SQUARE_STEP) - 10 >= 0) self->x = self->x - 1*SQUARE_STEP;} 
    if (self->control->right) {
        if ((self->x + 1*SQUARE_STEP) + 10 <= MAX_X) self->x = self->x + 1*SQUARE_STEP;
    }
    if (self->control->up) {
        if ((self->y - 1*SQUARE_STEP) - 10 >= 0) self->y = self->y - 1*SQUARE_STEP;
    }
    if (self->control->down) {
        if ((self->y + 1*SQUARE_STEP) + 10 <= MAX_Y) self->y = self->y + 1*SQUARE_STEP;
    }

    if (self->x < 0) self->x = 0;
    if (self->y < 0) self->y = 0;
    if (self->x > MAX_X - 10) self->x = MAX_X - 10; // Assuming character width is 10
    if (self->y > MAX_Y - 10) self->y = MAX_Y - 10; // Assuming character height is 10
}


void player_destroy(player *self) {
    if (self) {
        joystick_destroy(self->control);
        free(self);
    }
}
