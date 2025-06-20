#ifndef _BOSS_
#define _BOSS_

#include "npc.h"


typedef struct boss {
    npc *enemy;
    
    char jumping;
    char falling;

    char sandevistan;

    void (*jump)(struct boss *self, char direction); // Function pointer for jumping
    void (*fall)(struct boss *self, char direction); // Function pointer for falling
    char (*agro)(struct boss *self, element *player); //Function pointer for agro player
    
    char (*melee_attack)(struct boss *self);

} boss;

boss* boss_create(int x, int y, ALLEGRO_BITMAP *sprite, ALLEGRO_BITMAP *bullet_art); // Function to create a new npc

void boss_mov_y(boss *self, char direction);

char agro (boss *self, element *player);

char melee_attack (boss *self);

char update_boss_position(boss *self, int relative_floor, element *player);

void destroy_boss(boss * self);


#endif