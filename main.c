#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>


#include "character.h"
#include "npc.h"
#include "collision.h"
#include "map.h"

#include "boss.h"

#define SCREEN_X 1280
#define SCREEN_Y 720

#define JUMP_HEIGHT 400 // Define the height of the jump
#define NUM_NPCS 10 // Define the number of NPCs in the game

#define NUM_PLATFORMS 7 // Define the movement speed of bullets

#define MENU  0
#define IN_GAME 1
#define PAUSE 2
#define GAMEOVER 3
#define WIN 4

//global variables
char tela = 0;
int rolling_velocity = 0;
element *vector_platform[NUM_PLATFORMS]; // Array of platforms in the game

npc *enemies[NUM_NPCS]; // Array of NPCs (used for enemies or other characters)                 

struct character *player_1; // Global player character variable
struct boss *adam_smasher = NULL;
element relative_floor;
element relative_boss_floor;
int map_ajustment = 0;
char enemies_gone = 0;
element *life;


//poderiam ser variáveis globais,
char num_frames;
char num_walk_frames;
int frame_width;
int frame_height;

ALLEGRO_TIMER* timer;
ALLEGRO_EVENT_QUEUE* queue;
ALLEGRO_DISPLAY* disp;
ALLEGRO_FONT* font;
ALLEGRO_BITMAP *all_background;
ALLEGRO_BITMAP *menu_screen;
ALLEGRO_BITMAP *win_screen;
ALLEGRO_BITMAP *lose_screen;
ALLEGRO_BITMAP *heart;
ALLEGRO_BITMAP *main_character;
ALLEGRO_BITMAP *bullet_art;
ALLEGRO_BITMAP *background;
ALLEGRO_BITMAP *sprite_floor;
ALLEGRO_BITMAP *enemy1;
ALLEGRO_BITMAP *bullet_enemy1;
ALLEGRO_BITMAP *adam_sprite;
ALLEGRO_BITMAP *adam_bullets;


//functions
void must_init(bool test, const char *description);
void colision_floor();
void colision_floor_boss();
char update_position();
char update_boss();
void init_stuff();
void destroy_stuff();

unsigned char check_damage_enemies(player *killer, npc *victim){																																					//Implementação da função que verifica se um projétil acertou um jogador

	bullet *previous = NULL;
	for (bullet *index = killer->gun->shots; index != NULL; index = (bullet*) index->next){																													//Para todos os projéteis do atirador
		if ((index->x >= victim->basics->x - victim->basics->width) && (index->x <= victim->basics->x + victim->basics->width) && //																										//Verique se houve colisão com a vítima no eixo X
		   (index->y >= victim->basics->y - victim->basics->width) && (index->y <= victim->basics->y + victim->basics->width)){																											//Verifique se houve colisão com a vítima no eixo Y
			victim->hp -= killer->gun->damage;																																													//Reduz o HP da vítima em uma unidade (!)
			if (victim->hp){																																												//Verifica se a vítima ainda tem HP (!)
				if (previous){																																												//Verifica se não é o primeiro elemento da lista de projéteis (!)
					previous->next = index->next;																																							//Se não for, salva o próximo projétil (!)
					bullet_destroy(index);																																									//Chama o destrutor para o projétil atual (!)
					index = (bullet*) previous->next;																																						//Atualiza para o próximo projétil (!)
				}
				else {																																														//Se for o primeiro projétil da lista (!)
					killer->gun->shots = (bullet*) index->next;																																				//Atualiza o projétil no início da lista (!)
					bullet_destroy(index);																																									//Chama o destrutor para o projétil atual (!)
					index = killer->gun->shots;																																								//Atualiza para o próximo projétil (!)
				}
				return 0;																																													//A vítima sofreu dano, mas ainda não morre (!)
			}
			else return 1;																																													//A vítima sofreu dano e morreu (!)
		}
		previous = index;																																													//Atualiza a variável de controle do projétil anterior (!)
	}
	return 0;																																																//Se não houver nenhum projétil que acertou a vítima, retorne falso
}
unsigned char check_damage_player(npc *killer, player *victim){																																					//Implementação da função que verifica se um projétil acertou um jogador

	bullet *previous = NULL;

    if(victim->squat){
        return 0;
    }

	for (bullet *index = killer->gun->shots; index != NULL; index = (bullet*) index->next){																													//Para todos os projéteis do atirador
		if ((index->x >= victim->basics->x - victim->basics->width) && (index->x <= victim->basics->x + victim->basics->width) && //																										//Verique se houve colisão com a vítima no eixo X
		   (index->y >= victim->basics->y - victim->basics->width) && (index->y <= victim->basics->y + victim->basics->width)){																											//Verifique se houve colisão com a vítima no eixo Y
			victim->hp -= killer->gun->damage;																																													//Reduz o HP da vítima em uma unidade (!)
			if (victim->hp){																																												//Verifica se a vítima ainda tem HP (!)
				if (previous){																																												//Verifica se não é o primeiro elemento da lista de projéteis (!)
					previous->next = index->next;																																							//Se não for, salva o próximo projétil (!)
					bullet_destroy(index);																																									//Chama o destrutor para o projétil atual (!)
					index = (bullet*) previous->next;																																						//Atualiza para o próximo projétil (!)
				}
				else {																																														//Se for o primeiro projétil da lista (!)
					killer->gun->shots = (bullet*) index->next;																																				//Atualiza o projétil no início da lista (!)
					bullet_destroy(index);																																									//Chama o destrutor para o projétil atual (!)
					index = killer->gun->shots;																																								//Atualiza para o próximo projétil (!)
				}
				return 0;																																													//A vítima sofreu dano, mas ainda não morre (!)
			}
			else return 1;																																													//A vítima sofreu dano e morreu (!)
		}
		previous = index;																																													//Atualiza a variável de controle do projétil anterior (!)
	}
	return 0;																																																//Se não houver nenhum projétil que acertou a vítima, retorne falso
}


int main(){
    
    init_stuff();
    
    bool done = false;

    //poderiam ser variáveis globais,
    num_frames = 4; // Number of frames in the sprite sheet
    num_walk_frames = 6; // Number of walking frames in the sprite sheet
    frame_width = player_1->basics->width;
    frame_height = player_1->basics->height;
    
    srand(0); // Seed the random number generator for consistent NPC positions

    default_relative_floor(SCREEN_X, &relative_floor);   
    default_relative_floor(SCREEN_X, &relative_boss_floor);

    create_map(vector_platform, SCREEN_Y, all_background, sprite_floor); // Create the map
    create_npc_group(enemies, NUM_NPCS, FLOOR_LEVEL, enemy1, bullet_enemy1);

    life = create_element(2100, 360, 0, 0, al_get_bitmap_width(heart),al_get_bitmap_height(heart), heart);

    ALLEGRO_EVENT event;
    al_set_target_backbuffer(disp);

    al_start_timer(timer);
    while(1) {
        al_wait_for_event(queue, &event);

        switch(event.type)
        {

            case ALLEGRO_EVENT_KEY_DOWN:
                if(event.keyboard.keycode == ALLEGRO_KEY_UP)
                    joystick_up(player_1->control);

                if(event.keyboard.keycode == ALLEGRO_KEY_DOWN)
                    joystick_down(player_1->control);

                if(event.keyboard.keycode == ALLEGRO_KEY_LEFT)
                    joystick_left(player_1->control);
                if(event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
                    joystick_right(player_1->control);
                if(event.keyboard.keycode == ALLEGRO_KEY_X){ // key for sandevistan mode
                    joystick_x(player_1->control); // Toggle sandevistan mode
                    player_1->sandevistan = !player_1->sandevistan; // Toggle sandevistan state
                }
                if(event.keyboard.keycode == ALLEGRO_KEY_C) {
                    joystick_fire(player_1->control); // Toggle fire button
                }
                if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                    if((tela == PAUSE) || tela == MENU || tela == GAMEOVER){
                        done = true;
                    } else tela = PAUSE;

                }
                    
                if(event.keyboard.keycode == ALLEGRO_KEY_ENTER){
                    if(tela != GAMEOVER){
                        tela = IN_GAME;
                    }
                }

                break;

            case ALLEGRO_EVENT_KEY_UP:
                if(event.keyboard.keycode == ALLEGRO_KEY_UP)
                    joystick_up(player_1->control);
                if(event.keyboard.keycode == ALLEGRO_KEY_DOWN)
                    joystick_down(player_1->control);
                if(event.keyboard.keycode == ALLEGRO_KEY_LEFT)
                    joystick_left(player_1->control);
                if(event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
                    joystick_right(player_1->control);

                if(event.keyboard.keycode == ALLEGRO_KEY_C) {
                    joystick_fire(player_1->control); // Toggle fire button
                }

                break;

            case ALLEGRO_EVENT_TIMER:
                if(tela == MENU){
                    al_draw_scaled_bitmap(menu_screen, 0, 0,  1920, 960, 0, 0, SCREEN_X, SCREEN_Y, 0);
                } else if(tela == IN_GAME) {

                    if(!adam_smasher && !player_1->sandevistan){
                        if(player_1->fix_camera) {
                            rolling_velocity = player_1->basics->vx;
                        } else rolling_velocity = 0;

                        map_ajustment += rolling_velocity;
                    }

                    if(map_ajustment < 0) {
                        map_ajustment = 0; // Prevent scrolling beyond the left edge
                    } else if(map_ajustment > 6144 - 1536) {
                        map_ajustment = 6144 - 1536; // Prevent scrolling beyond the right edge
                    }
                    enemies_gone = 0;
                    for(int i = 0; i < NUM_NPCS; i++){
                        check_damage_enemies(player_1, enemies[i]);

                        check_damage_player(enemies[i], player_1);
                        if (enemies[i]->hp <= 0) {
                            enemies_gone += 1;
                        }
                    }

                    
                    if(!player_1->sandevistan) { // If sandevistan mode is not active
                        al_reparent_bitmap(background, all_background, map_ajustment, 0, 1536, 1024); // Reparent the background bitmap to create a scrolling effect
                        //cuidar final mapa rolling background deve ser feito.
                        al_draw_scaled_bitmap(background, 0, 0,  1536, 1024, 0, 0, SCREEN_X, SCREEN_Y, 0);

                        update_npc_group(enemies, NUM_NPCS, 0, SCREEN_X, map_ajustment, player_1->basics, rolling_velocity); // Update NPC positions based on their movement logic


                        for(int i = 0; i < NUM_NPCS; i++){
                            for (bullet *index = enemies[i]->gun->shots; index != NULL; index = (bullet*) index->next) {
                                al_draw_scaled_bitmap(enemies[i]->gun->bullet_art, 0, 0, 6, 4, index->x + pow(-1, enemies[i]->direction) * 24, index->y, 12, 8, 0);								//Insere as balas existentes disparadas pelo primeiro jogador na tela (!)
                            }   
                        if(enemies[i]->gun->timer) enemies[i]->gun->timer--;                     
                    }
                        
                    } else if (((al_get_timer_count(timer)) % (num_walk_frames + 2)) == 0) { // If sandevistan mode is active and the timer count is a multiple of the number of frames
                        al_draw_tinted_scaled_rotated_bitmap(background, al_map_rgba(128, 128, 128, 255), 0, 0, 0, 0, 0.8334, 0.7032, 0, 0);
                    }

                    al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 0, 0, "X: %d Y: %d", player_1->basics->x, player_1->basics->y);
                    
                    if(life != NULL){
                        if(life->x - map_ajustment < SCREEN_X && life->x - map_ajustment + life->width > 0) {
                             al_draw_tinted_bitmap(life->sprite,  al_map_rgba(255, 0, 0, 255), life->x - map_ajustment, life->y, 0);
                        }
                        if(simple_collide(player_1->basics->x, player_1->basics->y, player_1->basics->x + player_1->basics->width, player_1->basics->y + player_1->basics->height,
                                            life->x - map_ajustment, life->y, life->x + life->width - map_ajustment, life->y + life->height)){
                                                player_1->hp += 4;
                                                destroy_element(life);
                                                life = NULL;
                                            }
                    }

                    //draw the objectis of the map that appear in the screen

                    //draw a long platform
                    if(vector_platform[0]->x - map_ajustment < SCREEN_X && vector_platform[0]->x - map_ajustment + vector_platform[0]->width > 0) {
                        al_draw_tinted_scaled_rotated_bitmap_region(sprite_floor, 0, 32, 192, 32, al_map_rgb(255, 255, 255),
                                                                    0, 0, vector_platform[0]->x - map_ajustment, vector_platform[0]->y + 48,
                                                                    2.0, 2.0, 0.0, 0);
                    }

                    if(vector_platform[1]->x - map_ajustment < SCREEN_X && vector_platform[1]->x - map_ajustment + vector_platform[1]->width > 0) {
                        //draw a short platform
                        al_draw_tinted_scaled_rotated_bitmap_region(sprite_floor, 0, 64, 64, 32, al_map_rgb(255, 255, 255),
                                                                    0, 0, vector_platform[1]->x - map_ajustment, vector_platform[1]->y + 48,
                                                                    2.0, 2.0, 0.0, 0);
                    }
                    if(vector_platform[2]->x - map_ajustment < SCREEN_X && vector_platform[2]->x - map_ajustment + vector_platform[2]->width > 0) {
                        //draw a long platform
                        al_draw_tinted_scaled_rotated_bitmap_region(sprite_floor, 0, 32, 192, 32, al_map_rgb(255, 255, 255),
                                                                    0, 0, vector_platform[2]->x - map_ajustment, vector_platform[2]->y + 48,
                                                                    2.0, 2.0, 0.0, 0);
                    }
                    if(vector_platform[3]->x - map_ajustment < SCREEN_X && vector_platform[3]->x - map_ajustment + vector_platform[3]->width > 0) {
                        //draw a short platform
                            for(int i = 0; i < 8; i++) { // Draw a platform
                                al_draw_tinted_scaled_rotated_bitmap_region(sprite_floor, 0, 64, 64, 32, al_map_rgb(255, 255, 255),
                                                                    0, 0, i*64 + vector_platform[3]->x - map_ajustment, vector_platform[3]->y + 48, 1.0, 2.0, 0.0, 0);
                            }
                    }
                    if(vector_platform[4]->x - map_ajustment < SCREEN_X && vector_platform[4]->x - map_ajustment + vector_platform[3]->width > 0) {
                        //draw a short platform
                            for(int i = 0; i < 4; i++) { // Draw a platform
                                al_draw_tinted_scaled_rotated_bitmap_region(sprite_floor, 0, 64, 64, 32, al_map_rgb(255, 255, 255),
                                                                    0, 0, i*64 + vector_platform[4]->x - map_ajustment, vector_platform[4]->y + 48, 1.0, 2.0, 0.0, 0);
                            }
                    }
                    if(vector_platform[5]->x - map_ajustment < SCREEN_X && vector_platform[5]->x - map_ajustment + vector_platform[5]->width > 0) {
                        //draw a short platform
    
                        al_draw_tinted_scaled_rotated_bitmap_region(sprite_floor, 0, 32, 192, 32, al_map_rgb(128, 128, 255),
                                                            0, 0, vector_platform[5]->x - map_ajustment, vector_platform[5]->y + 48, 1.0, 2.0, 0.0, 0);
                            
                    }
                    if(vector_platform[6]->x - map_ajustment < SCREEN_X && vector_platform[6]->x - map_ajustment + vector_platform[6]->width > 0) {
                        //draw a short platform
    
                        for(int i = 0; i < 9; i++) { // Draw a platform
                                al_draw_tinted_scaled_rotated_bitmap_region(sprite_floor, 0, 64, 64, 32, al_map_rgb(0, 0, 255),
                                                                    0, 0, i*64 + vector_platform[6]->x - map_ajustment, vector_platform[6]->y + 48, 1.0, 2.0, 0.0, 0);
                            }
                    }
                    
                    //update_npc_group(enemies, NUM_NPCS, 0, SCREEN_X, map_ajustment, player_1->basics); // Update NPC positions based on their movement logic
                    for (int i = 0; i < NUM_NPCS; i++) { // Draw all NPCs
                        enemies[i]->frame = ( al_get_timer_count(timer) / 3 ) % num_frames; // Update NPC frame for animation
                        if ((enemies[i]->hp && ((enemies[i]->basics->x) < SCREEN_X) && (((enemies[i]->basics->x) + enemies[i]->basics->width) > 0))) {
                            al_draw_tinted_scaled_rotated_bitmap_region(enemies[i]->basics->sprite, enemies[i]->frame * frame_width, 0,
                                                                        48, 48, al_map_rgba(255, 255, 255, 255), 0, 0,
                                                                        enemies[i]->basics->x + (1 - pow((double)49, (double)enemies[i]->direction)),
                                                                        enemies[i]->basics->y, 2.0, 2.0, 0.0, enemies[i]->direction);
                        }
                    }

                    for(int i = 0; i < player_1->hp; i += 2){
                        al_draw_tinted_bitmap(heart,  al_map_rgba(255, 0, 0, 255), i * 16, 0, 0);
                    }

                    switch (update_position()) { // Update player_1 position based on joystick input
                        case 1: // walk movement
                            player_1->frame = ( al_get_timer_count(timer) / 3 ) % num_walk_frames;
                            al_draw_tinted_scaled_rotated_bitmap_region(player_1->basics->sprite, player_1->frame * frame_width, frame_height,
                                                                48, 48, al_map_rgba(255, 255, 255, 255), 0, 0, player_1->basics->x + (1 - pow((double)49, (double)player_1->direction)),
                                                                player_1->basics->y, 2.0, 2.0, 0.0, player_1->direction);
                            break;
                        case 2: // Up movement
                            player_1->frame = ( al_get_timer_count(timer) / 3 ) % num_frames;
                            al_draw_tinted_scaled_rotated_bitmap_region(player_1->basics->sprite, player_1->frame * frame_width, 2 * frame_height,
                                                                48, 48, al_map_rgba(255, 255, 255, 255), 0, 0, player_1->basics->x + (1 - pow((double)49, (double)player_1->direction)),
                                                                player_1->basics->y + (player_1->basics->height / 2), 2.0, 2.0, 0.0, player_1->direction);
                            break;
                        case 3: // squat movement
                            player_1->frame = (( al_get_timer_count(timer) / 3 ) % (num_frames - 1)) + 1;
                            al_draw_tinted_scaled_rotated_bitmap_region(player_1->basics->sprite, player_1->frame * frame_width, 3 * frame_height,
                                                                48, 48, al_map_rgba(255, 255, 255, 255), 0, 0, player_1->basics->x + (1 - pow((double)49, (double)player_1->direction)),
                                                                player_1->basics->y, 2.0, 2.0, 0.0, player_1->direction);
                            break;

                        case 4: // fire movement
                            player_1->frame = (( al_get_timer_count(timer) / 3 )) % num_frames;
                            al_draw_tinted_scaled_rotated_bitmap_region(player_1->basics->sprite, player_1->frame * frame_width, 4 * frame_height,
                                                                48, 48, al_map_rgba(255, 255, 255, 255), 0, 0, player_1->basics->x + (1 - pow((double)49, (double)player_1->direction)),
                                                                player_1->basics->y, 2.0, 2.0, 0.0, player_1->direction);
                            break; 
                            
                        case 5:
                        player_1->frame = ( al_get_timer_count(timer) / 3 ) % num_walk_frames;
                            al_draw_tinted_scaled_rotated_bitmap_region(player_1->basics->sprite, player_1->frame * frame_width, 5 * frame_height,
                                                                48, 48, al_map_rgba(255, 255, 255, 255), 0, 0, player_1->basics->x + (1 - pow((double)49, (double)player_1->direction)),
                                                                player_1->basics->y, 2.0, 2.0, 0.0, player_1->direction);
                            break;

                        case 6:
                            player_1->frame = (( al_get_timer_count(timer) / 3 ) % (num_frames - 1)) + 1;
                            al_draw_tinted_scaled_rotated_bitmap_region(player_1->basics->sprite, player_1->frame * frame_width, 6 * frame_height,
                                                                48, 48, al_map_rgba(255, 255, 255, 255), 0, 0, player_1->basics->x + (1 - pow((double)49, (double)player_1->direction)),
                                                                player_1->basics->y, 2.0, 2.0, 0.0, player_1->direction);
                            break;
                        case -1: //personagem morreu
                            tela = GAMEOVER;
                            break;                                 
                        default: // No movement
                            player_1->frame = ( al_get_timer_count(timer) / 3 ) % num_frames;
                            al_draw_tinted_scaled_rotated_bitmap_region(player_1->basics->sprite, player_1->frame * frame_width, 0,
                                                                48, 48, al_map_rgba(255, 255, 255, 255), 0, 0, player_1->basics->x + (1 - pow((double)49, (double)player_1->direction)),
                                                                player_1->basics->y, 2.0, 2.0, 0.0, player_1->direction);
                            break;
                    }
                    
                    for (bullet *index = player_1->gun->shots; index != NULL; index = (bullet*) index->next) {
                        al_draw_scaled_bitmap(player_1->gun->bullet_art, 0, 0, 6, 4, index->x + pow(-1, player_1->direction) * 24, index->y, 12, 8, 0);								//Insere as balas existentes disparadas pelo primeiro jogador na tela (!)
                                                
                    }
                    if(enemies_gone >= 6){
                        if(adam_smasher){
                            check_damage_enemies(player_1, adam_smasher->enemy);
                            check_damage_player(adam_smasher->enemy, player_1);
                            adam_smasher->sandevistan = player_1->sandevistan;
                        }
 
                        switch (update_boss()){
                            case 1:
                                adam_smasher->enemy->frame = (( al_get_timer_count(timer) / 3 ) % num_frames) + 1;
                                al_draw_tinted_scaled_rotated_bitmap_region(adam_smasher->enemy->basics->sprite, adam_smasher->enemy->frame * 2 * frame_width, 0,
                                                                adam_smasher->enemy->basics->width, adam_smasher->enemy->basics->height, al_map_rgba(255, 255, 255, 255), 0, 0, 
                                                                adam_smasher->enemy->basics->x + (1 - pow((double)97, (double)(adam_smasher->enemy->direction % 2))),
                                                                adam_smasher->enemy->basics->y - frame_height, 2.0, 2.0, 0.0, adam_smasher->enemy->direction % 2);
                                
                                break;
                            case 2:
                                adam_smasher->enemy->frame = 2;
                                al_draw_tinted_scaled_rotated_bitmap_region(adam_smasher->enemy->basics->sprite, adam_smasher->enemy->frame * 2 * frame_width, 2 * 96,
                                                                adam_smasher->enemy->basics->width, adam_smasher->enemy->basics->height, al_map_rgba(255, 255, 255, 255), 0, 0, 
                                                                adam_smasher->enemy->basics->x + (1 - pow((double)97, (double)(adam_smasher->enemy->direction % 2))),
                                                                adam_smasher->enemy->basics->y - frame_height, 2.0, 2.0, 0.0, adam_smasher->enemy->direction % 2);
                                
                                break;
                            case -2:
                                tela = WIN;
                                
                                break;

                            default:
                                adam_smasher->enemy->frame = (( al_get_timer_count(timer) / 3 ) % num_walk_frames);
                                if(adam_smasher->enemy->frame == 4){
                                    player_1->hp -= adam_smasher->melee_attack(adam_smasher);
                                }
                                al_draw_tinted_scaled_rotated_bitmap_region(adam_smasher->enemy->basics->sprite, adam_smasher->enemy->frame * 96, 96,
                                                                adam_smasher->enemy->basics->width, adam_smasher->enemy->basics->height, al_map_rgba(255, 255, 255, 255), 0, 0, adam_smasher->enemy->basics->x + (1 - pow((double)97, (double)(adam_smasher->enemy->direction % 2))),
                                                                adam_smasher->enemy->basics->y - frame_height, 2.0, 2.0, 0.0, adam_smasher->enemy->direction % 2);
                                
                                break;
                            }

                        for (bullet *index = adam_smasher->enemy->gun->shots; index != NULL; index = (bullet*) index->next) {
                            al_draw_scaled_bitmap(adam_smasher->enemy->gun->bullet_art, 0, 0, 6, 4, index->x + pow(-1, adam_smasher->enemy->direction) * 24, index->y, 12, 8, 0);	
                            						//Insere as balas existentes disparadas pelo primeiro jogador na tela (!)
                        }
                        if(adam_smasher->enemy->gun->timer) adam_smasher->enemy->gun->timer--;                    

                    }

                if (player_1->gun->timer) player_1->gun->timer--;


                } else if (tela == GAMEOVER) {
                    al_draw_scaled_bitmap(lose_screen, 0, 0,  1920, 1072, 0, 0, SCREEN_X, SCREEN_Y, 0);
                } else if (tela == PAUSE) {
                    al_draw_scaled_bitmap(menu_screen, 0, 0,  1920, 960, 0, 0, SCREEN_X, SCREEN_Y, 0);
                } else if(tela == WIN){
                    al_draw_scaled_bitmap(win_screen, 0, 0,  912, 513, 0, 0, SCREEN_X, SCREEN_Y, 0);
                }

                al_flip_display();
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if(done)
            break;

    }

    destroy_stuff();

    return 0;
}

void init_stuff() {
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    timer = al_create_timer(1.0 / 30.0);
    must_init(timer, "timer");

    queue = al_create_event_queue();
    must_init(queue, "queue");

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    disp = al_create_display(SCREEN_X, SCREEN_Y);
    must_init(disp, "display");


    font = al_create_builtin_font();
    must_init(font, "font");

    must_init(al_init_image_addon(), "image addon");
    all_background = al_load_bitmap("images/background_cyber.png");
    must_init(all_background, "background");

    must_init(al_init_primitives_addon(), "primitives");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    main_character = al_load_bitmap("images/sprite_wgun3.png");
    must_init(main_character, "main character sprite");
       
    bullet_art = al_load_bitmap("images/bullet.png");
    must_init(bullet_art, "bullet bitmap");

    player_1 = player_create(SCREEN_X / 2, SCREEN_Y / 2 , main_character, bullet_art); // Create player character

    background = al_create_sub_bitmap(all_background, 0, 0, 1536, 1024);
    must_init(background, "background sub bitmap");

    menu_screen = al_load_bitmap("images/menu.png");
    must_init(menu_screen, "tela de menu iniciar");

    win_screen = al_load_bitmap("images/win_screen.png");
    must_init(win_screen, "tela de vitoria");

    lose_screen = al_load_bitmap("images/gameover_screen.png");
    must_init(lose_screen, "tela de derrota"); 

    heart = al_load_bitmap("images/heart.png");
    must_init(heart, "heart_bar");

    sprite_floor = al_load_bitmap("images/sprite_tile.png");
    must_init(sprite_floor, "floor bitmap");

    enemy1 = al_load_bitmap("images/enemy1.png");
    must_init(enemy1, "enemy1 sprite");

    bullet_enemy1 = al_load_bitmap("images/Ball1.png");
    must_init(enemy1, "enemy1 sprite");

    adam_sprite = al_load_bitmap("images/boss.png");
    must_init(adam_sprite, "adam sprite");

    adam_bullets = al_load_bitmap("images/Projectile.png");
    must_init(adam_bullets, "bullets adao");
}

void destroy_stuff(){
    destroy_structures(vector_platform, NUM_PLATFORMS); // Clean up platforms
    player_destroy(player_1); // Clean up player_1 character
    destroy_boss(adam_smasher);
    destroy_npc_group(enemies, NUM_NPCS);

    if(life){
        destroy_element(life);
    }

    al_destroy_bitmap(adam_bullets);
    al_destroy_bitmap(adam_sprite);
    al_destroy_bitmap(heart);
    al_destroy_bitmap(win_screen); // Clean up enemy sprite
    al_destroy_bitmap(lose_screen); // Clean up enemy sprite
    al_destroy_bitmap(menu_screen); // Clean up enemy sprite
    al_destroy_bitmap(enemy1); // Clean up enemy sprite
    al_destroy_bitmap(bullet_enemy1); // Clean up enemy bullet sprite
    al_destroy_bitmap(sprite_floor); // Clean up floor sprite
    al_destroy_bitmap(bullet_art);
    al_destroy_bitmap(background);
    al_destroy_bitmap(all_background);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
}

char update_boss(){
    //se morreu só atualiza os tiros que ja tinham sido feitos
    if(!adam_smasher){
        adam_smasher = boss_create(SCREEN_X / 2, SCREEN_Y / 2, adam_sprite, adam_bullets);
    }

    if(adam_smasher->enemy->hp == 0){
        update_bullets(adam_smasher->enemy->gun, 0, SCREEN_X, rolling_velocity);
        return -2;
    }

    update_relative_floor(&relative_boss_floor, map_ajustment, &relative_boss_floor); // Update the relative floor level based on the player's position

    if(simple_collide(adam_smasher->enemy->basics->x, adam_smasher->enemy->basics->y, adam_smasher->enemy->basics->x + adam_smasher->enemy->basics->width, 
                        adam_smasher->enemy->basics->y + adam_smasher->enemy->basics->height,
                        relative_boss_floor.x, relative_boss_floor.y, relative_boss_floor.x + relative_boss_floor.width, relative_boss_floor.y + relative_boss_floor.height)) {
        adam_smasher->enemy->basics->y = relative_boss_floor.y - adam_smasher->enemy->basics->height; // Set player position to the top of the floor
        adam_smasher->enemy->basics->vy = 0; // Reset vertical velocity
        adam_smasher->jumping = 0; // Reset jumping state
        adam_smasher->falling = 0; // Reset falling state
    } else {
        adam_smasher->falling = 1; // Set falling state if not colliding with the floor
    }
    
    if((adam_smasher->jumping) && (adam_smasher->enemy->basics->y + adam_smasher->enemy->basics->height >= (relative_boss_floor.y - 240))) {
        adam_smasher->jump(adam_smasher, UP); // Call the jump function if the player is jumping
    } else if(adam_smasher->falling) {
        adam_smasher->jumping = 0; // Reset jumping state if the player is falling
        adam_smasher->fall(adam_smasher, DOWN); // Call the fall function if the player is falling
        int i;
        for(i = 0; i < NUM_PLATFORMS; i++) {																																										//Verifica se o primeiro jogador está colidindo com alguma plataforma (!)
            if(simple_collide(adam_smasher->enemy->basics->x, adam_smasher->enemy->basics->y, adam_smasher->enemy->basics->x + adam_smasher->enemy->basics->width,
                                adam_smasher->enemy->basics->y + adam_smasher->enemy->basics->height,
                                vector_platform[i]->x - map_ajustment, vector_platform[i]->y, (vector_platform[i]->x - map_ajustment) + vector_platform[i]->width, 
                                vector_platform[i]->y + vector_platform[i]->height)) {
                
                update_relative_floor(vector_platform[i], map_ajustment, &relative_boss_floor); // Update the floor level based on the platform's position
                adam_smasher->enemy->basics->y = vector_platform[i]->y - adam_smasher->enemy->basics->height; // Set player position to the top of the platform
                adam_smasher->enemy->basics->vy = 0; // Reset vertical velocity
                adam_smasher->falling = 0; // Stop falling                 
                
                break;
            } 

        }
        if(i == NUM_PLATFORMS) { // If no collision with platforms
            default_relative_floor(SCREEN_X, &relative_boss_floor);// Reset relative floor level to the default floor level
            colision_floor_boss();
        }
    } else {
        adam_smasher->enemy->basics->vy = 0; // Reset vertical velocity if not jumping or falling
        adam_smasher->jumping = 0; // Reset jumping state
        adam_smasher->falling = 0; // Reset falling state
    }

	char retorno = update_boss_position(adam_smasher, relative_boss_floor.y, player_1->basics);
    
	update_bullets(adam_smasher->enemy->gun, 0, SCREEN_X, rolling_velocity);

    return retorno;					
}

void must_init(bool test, const char *description)
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}
void colision_floor_boss() { //REVER NECESSIDADE DESTA FUNÇÃO
    // Check if the player_1 is falling and has reached the floor level
    if (adam_smasher->falling && (adam_smasher->enemy->basics->y + adam_smasher->enemy->basics->height >= FLOOR_LEVEL)) {
        adam_smasher->enemy->basics->y = FLOOR_LEVEL - adam_smasher->enemy->basics->height; // Set adam_smasher position to floor level
        adam_smasher->enemy->basics->vy = 0; // Reset vertical velocity
        adam_smasher->falling = 0; // Stop falling
        //adam_smasher->jumping = 0; // Reset jumping state
    }
}


void colision_floor() { //REVER NECESSIDADE DESTA FUNÇÃO
    // Check if the player_1 is falling and has reached the floor level
    if (player_1->falling && (player_1->basics->y + player_1->basics->height >= FLOOR_LEVEL)) {
        player_1->basics->y = FLOOR_LEVEL - player_1->basics->height; // Set player_1 position to floor level
        player_1->basics->vy = 0; // Reset vertical velocity
        player_1->falling = 0; // Stop falling
        //player_1->jumping = 0; // Reset jumping state
    }
}

char update_position(){			
    //se morreu só atualiza os tiros que ja tinham sido feitos
    if(player_1->hp <= 0){
        update_bullets(player_1->gun, 0, SCREEN_X, rolling_velocity);
        return -1;
    }

    update_relative_floor(&relative_floor, map_ajustment, &relative_floor); // Update the relative floor level based on the player's position

    if(simple_collide(player_1->basics->x, player_1->basics->y, player_1->basics->x + player_1->basics->width, player_1->basics->y + player_1->basics->height,
                        relative_floor.x, relative_floor.y, relative_floor.x + relative_floor.width, relative_floor.y + relative_floor.height)) {
        player_1->basics->y = relative_floor.y - player_1->basics->height; // Set player position to the top of the floor
        player_1->basics->vy = 0; // Reset vertical velocity
        player_1->jumping = 0; // Reset jumping state
        player_1->falling = 0; // Reset falling state
    } else {
        player_1->falling = 1; // Set falling state if not colliding with the floor
    }
    
    if((player_1->jumping) && (player_1->basics->y + player_1->basics->height >= (relative_floor.y - 240))) {
        player_1->jump(player_1, UP); // Call the jump function if the player is jumping
    } else if(player_1->falling) {
        player_1->jumping = 0; // Reset jumping state if the player is falling
        player_1->fall(player_1, DOWN); // Call the fall function if the player is falling
        int i;
        for(i = 0; i < NUM_PLATFORMS; i++) {																																										//Verifica se o primeiro jogador está colidindo com alguma plataforma (!)
            if(simple_collide(player_1->basics->x, player_1->basics->y, player_1->basics->x + player_1->basics->width, player_1->basics->y + player_1->basics->height,
                        vector_platform[i]->x - map_ajustment, vector_platform[i]->y, (vector_platform[i]->x - map_ajustment) + vector_platform[i]->width, 
                        vector_platform[i]->y + vector_platform[i]->height)) {
                update_relative_floor(vector_platform[i], map_ajustment, &relative_floor); // Update the floor level based on the platform's position
                player_1->basics->y = vector_platform[i]->y - player_1->basics->height; // Set player position to the top of the platform
                player_1->basics->vy = 0; // Reset vertical velocity
                player_1->falling = 0; // Stop falling                 
                
                break;
            } 

        }
        if(i == NUM_PLATFORMS) { // If no collision with platforms
            colision_floor(); // Check for collision with the floor
            default_relative_floor(SCREEN_X, &relative_floor);// Reset relative floor level to the default floor level
        }
    } else {
        player_1->basics->vy = 0; // Reset vertical velocity if not jumping or falling
        player_1->jumping = 0; // Reset jumping state
        player_1->falling = 0; // Reset falling state
    }

	char retorno = update_character_position(player_1, relative_floor.y);
    
    for(int i = 0; i < NUM_NPCS; i++){
        if(character_collide(player_1, enemies[i]->basics))
            break;
    }
	update_bullets(player_1->gun, 0, SCREEN_X, rolling_velocity);
    
    return retorno;																																										//Retorna o valor de retorno da função (!)
}

