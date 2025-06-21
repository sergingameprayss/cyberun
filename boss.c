

#include "boss.h"

#define gravity 5
#define PARADO 5

boss* boss_create(int x, int y, ALLEGRO_BITMAP *sprite, ALLEGRO_BITMAP *bullet_art){
    boss *new_boss = (boss*)malloc(sizeof(boss));
    if(!new_boss){
        return NULL;
    }

    if(!(new_boss->enemy = npc_create(x, y, sprite, bullet_art))){
        free(new_boss);
        return NULL;
    }

    new_boss->enemy->on_screen = 1;
    new_boss->enemy->basics->height = 96;
    new_boss->enemy->basics->width = 96;
    new_boss->enemy->hp = 100; // Initialize hp
    new_boss->enemy->gun->damage = 3;
    new_boss->jumping = 0;
    new_boss->falling = 0;

    new_boss->sandevistan = 0;

    new_boss->jump = boss_mov_y;
    new_boss->fall = boss_mov_y;
    new_boss->agro = agro;
    new_boss->melee_attack = melee_attack;

    return new_boss;
}

void destroy_boss(boss * self){
    if(!self){
        return;
    }
    if(self->enemy){
        npc_destroy(self->enemy);
    }
    free(self);
}

void boss_mov_y(boss *self, char direction){

    if(direction == DOWN){
        self->enemy->basics->vy += gravity; // Apply gravity effect
        self->enemy->basics->y += self->enemy->basics->vy; // Update vertical position based on velocity
    }
    
    if(direction == UP) {
        self->enemy->basics->vy = -25; // Set vertical velocity for jumping
        self->enemy->basics->y += self->enemy->basics->vy; // Update vertical position based on velocity
    }
}

char agro (boss *self, element *player){
    if(player->x > self->enemy->basics->x){
        return RIGHT;
    } else if(player->x < self->enemy->basics->x){
        return LEFT;
    } else if (player->y < self->enemy->basics->y) {
        return UP;
    }
    return RIGHT;
}

char melee_attack (boss *self){
    if(self->enemy->hp < 25){
        return 3;
    } 
    return 1;
}

char update_boss_position(boss *self, int relative_floor, element *player){
    char retorno = 1;


    self->enemy->direction = self->agro(self, player);
    if(!self->sandevistan){
        self->enemy->walk(self->enemy, self->enemy->direction, 0, 3);
    } else self->enemy->walk(self->enemy, self->enemy->direction, 0, 5);

	if (self->enemy->direction == UP) {
        // controle para cima e o player não está pulando nem caindo
        if((self->jumping == 0) && (self->enemy->basics->vy == 0) && (self->enemy->basics->y == relative_floor - self->enemy->basics->height)) { // Check if the player is on the floor and not jumping or falling
            self->jumping = 1;																																									//Se o botão de movimentação para cima do controle do primeiro jogador está ativado e o jogador não está pulando... (!)
        	self->jump(self, UP);																																								//Define a velocidade vertical do jogador para -10, iniciando o pulo (!)
            self->falling = 1; //ele vai cair depois do pulo		
            retorno = 2;																																		//Move o quadrado do primeiro jogador para cima (!)																										//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)
	    }
    }
//da pra juntar no control up se PPAAAAAAAAAAH
    if (self->falling || self->jumping) { // If the player is falling or jumping
        retorno = 2; // Indicate that the player is in the air
    }

    if(npc_collide(self->enemy, player) == 0){
        if (!self->enemy->gun->timer){																																											//Verifica se a arma do primeiro jogador não está em cooldown (!)
            self->enemy->shoot(self->enemy); 																																											//Se não estiver, faz um disparo (!)
            self->enemy->gun->timer = NPC_PISTOL_COOLDOWN;																																							//Inicia o cooldown da arma (!)
        } 		    																																								//Retorna o valor de retorno da função (!)      
    } else if(retorno != 2) retorno = 0;

    return retorno;
}
