#ifndef _ENEMY_
#define _ENEMY_

#include "element.h"

#define RIGHT 0
#define LEFT 1
#define UP 2
#define DOWN 3

typedef struct npc {
    element *basics; // Basic properties of the enemy
    int hp; // Health of the enemy
    int direction; // Direction the enemy is facing (0 = right, 1 = left)
    int frame; // Current frame for animation
    pistol *gun; // Gun for the enemy (optional, can be NULL)
    ALLEGRO_BITMAP *sprite; // Sprite for the enemy
    char on_screen;
    void (*walk)(struct npc *self, char direction, int velocidade_relativa); // Function pointer for walking
    void (*shoot)(struct npc *self); // Function pointer for attacking

} npc;

npc* npc_create(int x, int y, ALLEGRO_BITMAP *sprite, ALLEGRO_BITMAP *bullet_art); // Function to create a new npc
void npc_walk(npc *self, char direction, int velocidade_relativa); // Function to update npc position based on direction
void npc_shoot(npc *self); // Function to handle npc shooting
void npc_destroy(npc *self); // Function to destroy an npc and free resources

//create a group of npcs with the same sprites, in random positions x and the same y.
void create_npc_group (npc **npc_group, int group_size, int floor, ALLEGRO_BITMAP *enemy_sprite, ALLEGRO_BITMAP *enemy_bullet_art);
void update_npc_group (npc **npc_group, int group_size, int left_limit, int right_limit, int map_adjustment, element *player, int velocidade_relativa);
void destroy_npc_group(npc **npc_group, int group_size);
void npc_update(npc *self, int left_limit, int right_limit, int velocidade_relativa);

//void npc_draw(npc *self); // Function to draw the npc on the screen
void npc_collide(npc *self, element *player); // Function to handle collision with the player
void npc_death(npc *self);



#endif