#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>

#include "character.h"

//MOVIMENTAÇÃO NAO FUNCIONA DIAGONAL

#define MAX_X 1280 // Define maximum X coordinate for boundary checks
#define MAX_Y 720  // Define maximum Y coordinate for boundary checks


player* player_create(float x, float y, ALLEGRO_BITMAP *sprite, ALLEGRO_BITMAP *bullet_art) {
    player *new_player = (player*) malloc(sizeof(player));
    if (!new_player) return NULL; // Check for memory allocation failure

    //flags for character state
    new_player->sandevistan = 0; // Initialize sandevistan mode (0 = off)
    new_player->fix_camera_right = 0; // Initialize camera fix state
    new_player->fix_camera_left = 0; // Initialize camera fix state
    new_player->squat = 0; 
    new_player->jumping = 0; // Initialize jumping state
    new_player->falling = 1; // Initialize falling state


    new_player->face = RIGHT; // Initialize character facing direction (0 = right, 1 = left, 2 = up, 3 = down)
    new_player->size = 48; // Set size of the player character
    new_player->frame = 0; // Initialize frame for animation
    new_player->vx = 5; // Initialize velocity in X direction
    new_player->vy = 0; // Initialize velocity in Y direction
    new_player->x = x;
    new_player->y = y;
    new_player->control = joystick_create();
    if (!new_player->control) {
        free(new_player);
        return NULL; // Check for joystick creation failure
    }

    new_player->gun = pistol_create(); // Create a new gun for the player
    if (!new_player->gun) {
        joystick_destroy(new_player->control);
        free(new_player);
        return NULL; // Check for gun creation failure
    }
    
    if(bullet_art) {
        new_player->gun->bullet_art = bullet_art; // Assign the bullet art if provided
    } else {
        new_player->gun->bullet_art = NULL;  // No bullet art provided
    }

    if(sprite) {
        new_player->sprite = sprite; // Assign the sprite if provided
    } else {
        new_player->sprite = NULL; // No sprite assigned
    }

    new_player->walk = player_mov_x; // Assign the movement function
    new_player->fall = player_mov_y; // Assign the jump function
    new_player->jump = player_mov_y; // Assign the fall function
    new_player->shoot = character_shot; // Assign the shoot function

    return new_player;
}

void player_mov_x(player *self, char direction) {
    // Update velocity based on joystick input

    if (direction == LEFT) {
        if ((self->x - 1*self->vx) >= self->size) { // Ensure the player does not move out of bounds
            self->x = self->x - 1*self->vx;
        } else { 
            self->fix_camera_left = 1;
            //self->x = self->x - 1*self->vx;
        }
    } 

    if (direction == RIGHT) {
        if ((self->x + 1*self->vx) <= MAX_X - 2 * self->size) { // Ensure the player does not move out of bounds
            self->x = self->x + 1*self->vx;
        } else {
            self->fix_camera_right = 1;
            //self->x = self->x + 1*self->vx;
        }
    }
}

void player_mov_y(player *self, char direction){
    
    if(direction == DOWN){
        self->vy += gravity; // Apply gravity effect
        self->y += self->vy; // Update vertical position based on velocity
    }
    
    if(direction == UP) {
        self->vy = -25; // Set vertical velocity for jumping
        self->y += self->vy; // Update vertical position based on velocity
    }
}

void character_shot(player *self){																														//Implementação da função "square_shot" (!)
	bullet *shot;
    
    //somo com size para que o tiro saia do centro do quadrado (ele escala o quadrado na hora de desenhar)
	if (!self->face) shot = pistol_shot(self->x + self->size, self->y + self->size, self->face, self->gun);										//Quadrado atira para a esquerda (!)
	else if (self->face == 1) shot = pistol_shot(self->x, self->y + self->size, self->face, self->gun);								//Quadrado atira para a direita (!)
	if (shot) self->gun->shots = shot;
}



void player_destroy(player *self) {
    if (self) {
        joystick_destroy(self->control);
        al_destroy_bitmap(self->sprite); // Destroy the sprite if it exists
        free(self);
    }
}
