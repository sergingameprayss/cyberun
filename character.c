#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>

#include "character.h"

//MOVIMENTAÇÃO NAO FUNCIONA DIAGONAL

#define MAX_X 1280 // Define maximum X coordinate for boundary checks
#define MAX_Y 720  // Define maximum Y coordinate for boundary checks

#define SIZE 48 // Define the SIZE of the character sprite

player* player_create(int x, int y, ALLEGRO_BITMAP *sprite, ALLEGRO_BITMAP *bullet_art) {
    player *new_player = (player*) malloc(sizeof(player));
    if (!new_player) return NULL; // Check for memory allocation failure

    //flags for character state
    new_player->sandevistan = 0; // Initialize sandevistan mode (0 = off)
    new_player->fix_camera= 0; // Initialize camera fix state
    new_player->squat = 0; 
    new_player->jumping = 0; // Initialize jumping state
    new_player->falling = 1; // Initialize falling state

    new_player->basics = create_element(x, y, 5, 0, SIZE, SIZE, sprite);// Create the basic element with position and size
    new_player->hp = 50;
    new_player->direction = RIGHT; // Initialize character facing direction (0 = right, 1 = left, 2 = up, 3 = down)
    new_player->frame = 0; // Initialize frame for animation
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
    new_player->gun->damage = 5;
    
    if(bullet_art) {
        new_player->gun->bullet_art = bullet_art; // Assign the bullet art if provided
    } else {
        new_player->gun->bullet_art = NULL;  // No bullet art provided
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
        if(self->sandevistan){
            self->basics->vx = -7;
        } else self->basics->vx = -5;
        if (!((self->basics->x - 1*self->basics->vx) >=  2 * self->basics->width)) { 
            self->fix_camera = 1;
            return;
        }

    } 

    if (direction == RIGHT) {
        if(self->sandevistan){
            self->basics->vx = 7;
        } else self->basics->vx = 5;
        if (!((self->basics->x + 1*self->basics->vx) <= MAX_X - 4 * self->basics->width)) { // Ensure the player does not move out of bounds
            self->fix_camera = 1;
            return;
        }
    }

    self->fix_camera = 0; // Reset camera fix state when moving 
    self->basics->x += self->basics->vx;
}

void player_mov_y(player *self, char direction){
    
    if(direction == DOWN){
        self->basics->vy += gravity; // Apply gravity effect
        self->basics->y += self->basics->vy; // Update vertical position based on velocity
    }
    
    if(direction == UP) {
        self->basics->vy = -25; // Set vertical velocity for jumping
        self->basics->y += self->basics->vy; // Update vertical position based on velocity
    }
}

void character_shot(player *self){																														//Implementação da função "square_shot" (!)
	bullet *shot;
    
    //somo com size para que o tiro saia do centro do quadrado (ele escala o quadrado na hora de desenhar)
	if (!self->direction) shot = pistol_shot(self->basics->x + self->basics->width, self->basics->y + self->basics->height - 16, self->direction, self->gun);										//Quadrado atira para a esquerda (!)
	else if (self->direction == 1) shot = pistol_shot(self->basics->x, self->basics->y + self->basics->height - 16, self->direction, self->gun);								//Quadrado atira para a direita (!)
	if (shot) self->gun->shots = shot;
}

void player_destroy(player *self) {
    if (self) {

        destroy_element(self->basics); // Destroy the basic element
        joystick_destroy(self->control);
        pistol_destroy(self->gun); // Destroy the gun if it exists
        
        free(self);
    }
}


char update_character_position(player *self, int relative_floor_y){
    char retorno = 0;

        if (self->control->left){
            if(!self->squat)																																										//Se o botão de movimentação para esquerda do controle do primeiro jogador está ativado... (!)
                self->walk(self, LEFT);	

            self->direction = LEFT;

            retorno = 1;																																										//Move o quadrado do primeiro jogador para a esquerda (!)
                                                                                                                                                //Move o quadrado do primeiro jogador para a esquerda (!)
            //if (collision_2D(self,) player_move(self, -1, 0, SCREEN_X, SCREEN_Y);																											//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)
        } 
        
        if (self->control->right){	
            if(!self->squat)																																									//Se o botão de movimentação para direita do controle do primeir ojogador está ativado... (!)
                self->walk(self, RIGHT);	

            self->direction = RIGHT;
            retorno = 1;																																										//Move o quadrado do primeiro jogador para a direita (!)
                                                                                                                                                            //Move o quadrado do primeiro jogador para a direta (!)
            //if (collision_2D(self, player_2)) player_move(self, -1, 1, SCREEN_X, SCREEN_Y);																											//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)
        } 			
    

    if(retorno != 1){
        self->fix_camera = 0; 
    }
	if (self->control->up) {
        // controle para cima e o player não está pulando nem caindo
        if((self->jumping == 0) && (self->basics->vy == 0) && (self->basics->y == relative_floor_y - self->basics->height)) { // Check if the player is on the floor and not jumping or falling
            self->jumping = 1;																																									//Se o botão de movimentação para cima do controle do primeiro jogador está ativado e o jogador não está pulando... (!)
        	self->jump(self, UP);																																								//Define a velocidade vertical do jogador para -10, iniciando o pulo (!)
            self->falling = 1; //ele vai cair depois do pulo		
            retorno = 2;																																		//Move o quadrado do primeiro jogador para cima (!)
		//if (collision_2D(self, player_2)) player_move(self, -1, 2, SCREEN_X, SCREEN_Y);																											//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)
	    }
    }
    if (self->control->down){																																										//Se o botão de movimentação para baixo do controle do primeiro jogador está ativado... (!)
		self->squat = 1;
        
        retorno = 3;
		//if (collision_2D(self, player_2)) player_move(self, -1, 3, SCREEN_X, SCREEN_Y);																											//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)
	} else self->squat = 0; // Reset squat state if down control is not pressed

//da pra juntar no control up se PPAAAAAAAAAAH
    if (self->falling || self->jumping) { // If the player is falling or jumping
        retorno = 2; // Indicate that the player is in the air
    }


    if (self->control->fire){																																											//Verifica se o primeiro jogador está atirando (!)
		if (!self->gun->timer){																																											//Verifica se a arma do primeiro jogador não está em cooldown (!)
			self->shoot(self); 																																											//Se não estiver, faz um disparo (!)
			self->gun->timer = PISTOL_COOLDOWN;																																							//Inicia o cooldown da arma (!)
		} 
        if(retorno == 0){
            retorno = 4;
        } else if(!self->squat) {
            retorno = 5;		    																																								//Retorna o valor de retorno da função (!)
        } else  retorno = 6;       
    }
    return retorno;
}

char character_collide(player *self, element *thing){

    switch(complex_collide(self->basics->x, self->basics->y, self->basics->x + self->basics->width, self->basics->y + self->basics->height,
                              thing->x, thing->y, thing->x + thing->width, thing->y + thing->height)){
        case 1:
            self->basics->x = thing->x - self->basics->width;
            break;
        case 2:
            self->basics->x = thing->x + thing->width;
            break;
        case 3:
            self->basics->y = thing->y - self->basics->height;
            break;
        case 4:
            self->basics->y = thing->y - thing->height;
            break;
        case 5:

            self->basics->y = thing->y + thing->height;
            break;
        case 6:

            self->basics->y = thing->y + thing->height;
            break;
        default:
            return 0;
        }
        return 1;

}