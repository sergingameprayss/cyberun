
#include <stdlib.h>
#include "npc.h"
#include "collision.h"

npc* npc_create(int x, int y, ALLEGRO_BITMAP *sprite, ALLEGRO_BITMAP *bullet_art) {
    npc *new_npc = (npc*) malloc(sizeof(npc));
    if (!new_npc) return NULL; // Check for memory allocation failure

    new_npc->basics = create_element(x, y, 0, 0, 48, 48, sprite); // Create the basic element with position and size
    if (!new_npc->basics) {
        free(new_npc);
        return NULL; // Check for element creation failure
    }
    new_npc->on_screen = 0;
    new_npc->hp = 10; // Initialize hp
    new_npc->direction = 0; // Initialize facing direction
    new_npc->frame = 0; // Initialize frame for animation
    new_npc->gun = pistol_create(); // Create a new gun for the npc


    if (!new_npc->gun) {
        destroy_element(new_npc->basics);
        free(new_npc);
        return NULL; // Check for gun creation failure
    }

    if(bullet_art) {
        new_npc->gun->bullet_art = bullet_art; // Assign the bullet art if provided
    } else {
        new_npc->gun->bullet_art = NULL;  // No bullet art provided
    }

    new_npc->walk = npc_walk; // Assign the walking function
    new_npc->shoot = npc_shoot; // Assign the shooting function

    return new_npc;
}

void npc_walk(npc *self, char direction, int vr) {
    // Update velocity based on direction
    if (direction == LEFT) {
        self->basics->vx = -5; // Move left
    } else if (direction == RIGHT) {
        self->basics->vx = 5; // Move right
    } else {
        self->basics->vx = 0; // Stop moving if no direction is specified
    }
    self->basics->x += self->basics->vx - vr; // Update the x position based on velocity
}

void npc_shoot(npc *self) {
    if (self->gun && self->gun->timer == 0) { // Check if the gun is available and ready to shoot
            bullet *shot;
        
        //somo com size para que o tiro saia do centro do quadrado (ele escala o quadrado na hora de desenhar)
        if (!self->direction) shot = pistol_shot(self->basics->x + self->basics->width, self->basics->y + self->basics->width, self->direction, self->gun);										//Quadrado atira para a esquerda (!)
        else if (self->direction == 1) shot = pistol_shot(self->basics->x, self->basics->y + self->basics->width, self->direction, self->gun);								//Quadrado atira para a direita (!)
        if (shot) self->gun->shots = shot;
    }
}
void npc_destroy(npc *self) {
    if (self) {
        if (self->basics) {
            destroy_element(self->basics); // Destroy the basic element
        }
        if (self->gun) {
            pistol_destroy(self->gun); // Destroy the gun
        }
        free(self); // Free the npc structure
    }
}

void create_npc_group (npc **npc_group, int group_size, int floor, ALLEGRO_BITMAP *enemy_sprite, ALLEGRO_BITMAP *enemy_bullet_art){
        // Function to create enemies (NPCs) in the game
    // This function can be used to initialize and set up NPCs for the game
    // It can include logic for their positions, behaviors, and interactions with the player

    for (int i = 0; i < group_size; i++) {
        npc_group[i] = npc_create(rand() % (6144 - 48 + 1) + 48, floor - 48, enemy_sprite, enemy_bullet_art); // Create NPCs with initial positions
    }

}

void destroy_npc_group(npc **npc_group, int group_size){
    for(int i = 0; i < group_size; i++) {
        if (npc_group[i]) {
            npc_destroy(npc_group[i]);
            npc_group[i] = NULL;
        }
    }
}

void npc_update(npc *self, int left_limit, int right_limit, int vr){
    if(self->basics->x < left_limit + 48)
            self->direction = RIGHT; // If NPC is too far left, set direction to right
        else if((self->basics->x + self->basics->width)> right_limit - 48)
            self->direction = LEFT; 

        // Update NPC position based on their behavior (e.g., moving towards the player)
        self->walk(self, self->direction, vr); // Call the walk function for each NPC

        if (!self->gun->timer){																																											//Verifica se a arma do primeiro jogador não está em cooldown (!)
			self->shoot(self); 																																											//Se não estiver, faz um disparo (!)
			self->gun->timer = NPC_PISTOL_COOLDOWN;																																							//Inicia o cooldown da arma (!)
		} 

        update_bullets(self->gun, 0, right_limit);
} // Function to update npc state (movement, shooting, etc.)

void npc_collide(npc *self, element *player){

    switch(complex_collide(self->basics->x, self->basics->y, self->basics->x + self->basics->width, self->basics->y + self->basics->height,
                              player->x, player->y, player->x + player->width, player->y + player->height)){
        case 1:
            self->basics->x = player->x - self->basics->width;
            break;
        case 2:
            self->basics->x = player->x + player->width;
            break;
        case 3:
            self->basics->x = player->x - player->width;
            //self->basics->y = player->y - self->basics->height;
            break;
        case 4:
            self->basics->x = player->x + player->width;
            //self->basics->y = player->y + player->height;
            break;
        case 5:
            self->basics->x = player->x - player->width;
            //self->basics->y = player->y + player->height;
            break;
        case 6:
            self->basics->x = player->x + player->width;
            //self->basics->y = player->y + player->height;
            break;
        default:
            return;
        }
    
}

void npc_death(npc *self){
    self->basics->x = 0;
    self->basics->y = 0;
    self->basics->width = 0;
    self->basics->height = 0;

}

void update_npc_group (npc **npc_group, int group_size, int left_limit, int right_limit, int map_adjustment, element *player, int  vr){
    for(int i = 0; i < group_size; i++) { // Loop through each NPC
        if(!npc_group[i]->on_screen){
            if(((npc_group[i]->basics->x - map_adjustment) < right_limit) && (((npc_group[i]->basics->x - map_adjustment) + npc_group[i]->basics->width) > 0)){
                npc_group[i]->on_screen = 1;

                npc_group[i]->basics->x -= map_adjustment;
                npc_update(npc_group[i], 0, right_limit, vr);
                // Check for collisions with the player 
                npc_collide(npc_group[i], player);
            } 
            continue;

    } else if(npc_group[i]->hp <= 0){
            npc_death(npc_group[i]); //vai ficar chamadno toda vez?
            update_bullets(npc_group[i]->gun, left_limit, right_limit);
            continue;
        }

        npc_update(npc_group[i], 0, right_limit, vr);
        // Check for collisions with the player 
        npc_collide(npc_group[i], player);

        }
        
}