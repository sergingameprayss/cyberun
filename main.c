#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>


#include "character.h"

#define SCREEN_X 1280
#define SCREEN_Y 720

#define FLOOR_LEVEL 640 // Define the floor level for the game
#define JUMP_HEIGHT 400 // Define the height of the jump


void must_init(bool test, const char *description);
char collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2);
void colision_floor(player *self);
void update_bullets(player *player);
char update_position(player *player_1);



int main()
{
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    ALLEGRO_DISPLAY* disp = al_create_display(SCREEN_X, SCREEN_Y);
    must_init(disp, "display");


    ALLEGRO_FONT* font = al_create_builtin_font();
    must_init(font, "font");

    must_init(al_init_image_addon(), "image addon");
    ALLEGRO_BITMAP *all_background = al_load_bitmap("background_cyber.png");
    must_init(all_background, "background");

    must_init(al_init_primitives_addon(), "primitives");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool done = false;

    ALLEGRO_EVENT event;

    struct character *player;


    ALLEGRO_BITMAP *main_character = al_load_bitmap("sprite3.0.png");
    must_init(main_character, "main character sprite");
       
    ALLEGRO_BITMAP *bullet_art = al_load_bitmap("bullet.png");
    must_init(bullet_art, "bullet bitmap");

    player = player_create(SCREEN_X / 2, SCREEN_Y / 2 , main_character, bullet_art); // Create player character

    ALLEGRO_BITMAP *background = al_create_sub_bitmap(all_background, 0, 0, 1536, 1024);
    must_init(background, "background sub bitmap");

    ALLEGRO_BITMAP *floor = al_load_bitmap("sprite_tile.png");
    must_init(floor, "floor bitmap");



    //poderiam ser variáveis globais,
    char num_frames = 4; // Number of frames in the sprite sheet
    char num_walk_frames = 6; // Number of walking frames in the sprite sheet
    int frame_width = 48;
    int frame_height = 48;
    
    //int plimit = FLOOR_LEVEL; // Player limit on the Y-axis (floor level)
    int x_ysis = 0; // X-axis offset for the background
    

    al_set_target_bitmap(all_background);

    al_draw_bitmap_region(floor, 0, 0, 32, 32, 0, 1024 - 48, 0);
    al_draw_bitmap_region(floor, 0, 0, 32, 16, 0, 1024 - 16, 0);
    int i;
    for(i = 1; i < 200; i++) { // Draw the floor
        al_draw_bitmap_region(floor, 32, 0, 32, 32, i * 32, 1024 - 48, 0);
        al_draw_bitmap_region(floor, 32, 0, 32, 16, i * 32, 1024 - 16, 0); //trocar essa sprite 
    }
    al_draw_bitmap_region(floor, 64, 0, 32, 32, i * 32, 1024 - 48, 0);
    al_draw_bitmap_region(floor, 64, 0, 32, 16, i * 32, 1024 - 16, 0);

    //draw a long platform
    al_draw_tinted_scaled_rotated_bitmap_region(floor, 0, 32, 192, 32, al_map_rgb(255, 255, 255),
                                                0, 0, 1500, SCREEN_Y, 2.0, 2.0, 0.0, 0);

    //draw a short platform
    al_draw_tinted_scaled_rotated_bitmap_region(floor, 0, 64, 64, 32, al_map_rgb(255, 255, 255),
                                                0, 0, 2400, SCREEN_Y, 2.0, 2.0, 0.0, 0);
    //draw a long platform
    al_draw_tinted_scaled_rotated_bitmap_region(floor, 0, 32, 192, 32, al_map_rgb(255, 255, 255),
                                                0, 0, 2464, SCREEN_Y - 400, 2.0, 2.0, 0.0, 0);

    for(i = 1; i < 5; i++) { // Draw a platform
        al_draw_tinted_scaled_rotated_bitmap_region(floor, 0, 64, 64, 32, al_map_rgb(255, 255, 255),
                                                    0, 0, i*64 + 4000, SCREEN_Y, 2.0, 2.0, 0.0, 0);
    }

    al_set_target_backbuffer(disp);

    al_start_timer(timer);
    while(1)
    {
        al_wait_for_event(queue, &event);

        switch(event.type)
        {

            case ALLEGRO_EVENT_KEY_DOWN:
                if(event.keyboard.keycode == ALLEGRO_KEY_UP)
                    joystick_up(player->control);

                if(event.keyboard.keycode == ALLEGRO_KEY_DOWN)
                    joystick_down(player->control);

                if(event.keyboard.keycode == ALLEGRO_KEY_LEFT)
                    joystick_left(player->control);
                if(event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
                    joystick_right(player->control);
                if(event.keyboard.keycode == ALLEGRO_KEY_X){ // key for sandevistan mode
                    joystick_x(player->control); // Toggle sandevistan mode
                    player->sandevistan = !player->sandevistan; // Toggle sandevistan state
                }
                if(event.keyboard.keycode == ALLEGRO_KEY_C) {
                    joystick_fire(player->control); // Toggle fire button
                }
                if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                    done = true;

                break;

            case ALLEGRO_EVENT_KEY_UP:
                if(event.keyboard.keycode == ALLEGRO_KEY_UP)
                    joystick_up(player->control);
                if(event.keyboard.keycode == ALLEGRO_KEY_DOWN)
                    joystick_down(player->control);
                if(event.keyboard.keycode == ALLEGRO_KEY_LEFT)
                    joystick_left(player->control);
                if(event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
                    joystick_right(player->control);

                if(event.keyboard.keycode == ALLEGRO_KEY_C) {
                    joystick_fire(player->control); // Toggle fire button
                }

                if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                    done = true;

                break;

            case ALLEGRO_EVENT_TIMER:
                if(player->fix_camera_left) {
                    x_ysis -= player->vx;
                } else if(player->fix_camera_right) {
                    x_ysis += player->vx;
                }
                if(x_ysis < 0) {
                    x_ysis = 0; // Prevent scrolling beyond the left edge
                } else if(x_ysis > 6144 - 1536) {
                    x_ysis = 6144 - 1536; // Prevent scrolling beyond the right edge
                }

                
                //melhorar isso aqui, talvez com uma função de scroll
                if(!player->sandevistan) { // If sandevistan mode is not active
                    al_reparent_bitmap(background, all_background, x_ysis, 0, 1536, 1024); // Reparent the background bitmap to create a scrolling effect
                    //cuidar final mapa rolling background deve ser feito.
                    al_draw_scaled_bitmap(background, 0, 0,  1536, 1024, 0, 0, SCREEN_X, SCREEN_Y, 0);
                    
                } else if (((al_get_timer_count(timer)) % (num_walk_frames + 2)) == 0) { // If sandevistan mode is active and the timer count is a multiple of the number of frames
                    al_draw_tinted_scaled_rotated_bitmap(background, al_map_rgba(128, 128, 128, 255), 0, 0, 0, 0, 0.8334, 0.7032, 0, 0);
                } //FAZER REPINTAR DE INICIO NA PRIMEIRA VEZ

                al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 0, 0, "X: %.1f Y: %.1f", player->x, player->y);
                
               

                switch (update_position(player)) { // Update player position based on joystick input
                    case 1: // walk movement
                        player->frame = ( al_get_timer_count(timer) / 3 ) % num_walk_frames;
                        al_draw_tinted_scaled_rotated_bitmap_region(player->sprite, player->frame * frame_width, frame_height,
                                                            48, 48, al_map_rgba(255, 255, 255, 255), 0, 0, player->x + (1 - pow((double)49, (double)player->face)),
                                                             player->y, 2.0, 2.0, 0.0, player->face);
                        break;
                    case 2: // Up movement
                        player->frame = ( al_get_timer_count(timer) / 3 ) % num_frames;
                        al_draw_tinted_scaled_rotated_bitmap_region(player->sprite, player->frame * frame_width, 2 * frame_height,
                                                            48, 48, al_map_rgba(255, 255, 255, 255), 0, 0, player->x + (1 - pow((double)49, (double)player->face)),
                                                             player->y + (player->size / 2), 2.0, 2.0, 0.0, player->face);
                        break;
                    case 3: // squat movement
                        player->frame = ( al_get_timer_count(timer) / 3 ) % num_frames;
                        al_draw_tinted_scaled_rotated_bitmap_region(player->sprite, player->frame * frame_width, 3 * frame_height,
                                                            48, 48, al_map_rgba(255, 255, 255, 255), 0, 0, player->x + (1 - pow((double)49, (double)player->face)),
                                                             player->y, 2.0, 2.0, 0.0, player->face);
                        break;

                    case 4: // fire movement
                        player->frame = (player->frame = ( al_get_timer_count(timer) / 3 ) % 2) + 4;
                        al_draw_tinted_scaled_rotated_bitmap_region(player->sprite, player->frame * frame_width, 4 * frame_height,
                                                             48, 48, al_map_rgba(255, 255, 255, 255), 0, 0, player->x + (1 - pow((double)49, (double)player->face)),
                                                             player->y, 2.0, 2.0, 0.0, player->face);
                        break;                                   
                    default: // No movement
                        player->frame = ( al_get_timer_count(timer) / 3 ) % num_frames;
                        al_draw_tinted_scaled_rotated_bitmap_region(player->sprite, player->frame * frame_width, 0,
                                                            48, 48, al_map_rgba(255, 255, 255, 255), 0, 0, player->x + (1 - pow((double)49, (double)player->face)),
                                                             player->y, 2.0, 2.0, 0.0, player->face);
                        break;
                }
                
                for (bullet *index = player->gun->shots; index != NULL; index = (bullet*) index->next) {
                    al_draw_scaled_bitmap(player->gun->bullet_art, 0, 0, 6, 4, index->x + pow(-1, player->face) * 24, index->y, 12, 8, 0);								//Insere as balas existentes disparadas pelo primeiro jogador na tela (!)
                                              
                }
                
                    if (player->gun->timer) player->gun->timer--;

                al_flip_display();
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if(done)
            break;

        
    }

    al_destroy_bitmap(background);
    al_destroy_bitmap(all_background);
    player_destroy(player); // Clean up player character
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}



char collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2)
{
    if(ax1 > bx2) return false;
    if(ax2 < bx1) return false;
    if(ay1 > by2) return false;
    if(ay2 < by1) return false;

    return true;
}

void colision_floor(player *self) {
    // Check if the player is falling and has reached the floor level
    if (self->falling && self->y + self->size >= FLOOR_LEVEL) {
        self->y = FLOOR_LEVEL - self->size; // Set player position to floor level
        self->vy = 0; // Reset vertical velocity
        self->falling = 0; // Stop falling
        //self->jumping = 0; // Reset jumping state
    }
}

void must_init(bool test, const char *description)
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}


void update_bullets(player *player){																																										//Implementação da função que atualiza o posicionamento de projéteis conforme o movimento dos mesmos (!)
	
	bullet *previous = NULL;																																												//Variável auxiliar para salvar a posição imediatamente anterior na fila (!)
	for (bullet *index = player->gun->shots; index != NULL;){																																				//Para cada projétil presente na lista de projéteis disparados (!)
		if (!index->trajectory) index->x += BULLET_MOVE;																																					//Se a trajetória for para a esquerda, atualiza a posição para a esquerda (!)
		else if (index->trajectory == 1) index->x -= BULLET_MOVE;																																			//Se a trajetória for para a direita, atualiza a posição para a esquerda (!)
		
		if ((index->x < 0) || (index->x > SCREEN_X)){																																						//Verifica se o projétil saiu das bordas da janela (!)
			if (previous){																																													//Verifica se não é o primeiro elemento da lista de projéteis (!)
				previous->next = index->next;																																								//Se não for, salva o próximo projétil (!)
				bullet_destroy(index);																																										//Chama o destrutor para o projétil atual (!)
				index = (bullet*) previous->next;																																							//Atualiza para o próximo projétil (!)
			}
			else {																																															//Se for o primeiro projétil da lista (!)
				player->gun->shots = (bullet*) index->next;																																					//Atualiza o projétil no início da lista (!)
				bullet_destroy(index);																																										//Chama o destrutor para o projétil atual (!)
				index = player->gun->shots;																																									//Atualiza para o próximo projétil (!)
			}
		}
		else{																																																//Se não saiu da tela (!)
			previous = index;																																												//Atualiza o projétil anterior (para a próxima iteração) (!)
			index = (bullet*) index->next;																																									//Atualiza para o próximo projétil (!)
		}
	}
}


char update_position(player *player_1){		
    char retorno = 0;	
    
    if((player_1->jumping) && (player_1->y + player_1->size >= JUMP_HEIGHT)) {
        player_1->jump(player_1, UP); // Call the jump function if the player is jumping
    } else if(player_1->falling) {
        player_1->jumping = 0; // Reset jumping state if the player is falling
        player_1->fall(player_1, DOWN); // Call the fall function if the player is falling
        colision_floor(player_1); // Check for collision with the floor
    } else {
        player_1->vy = 0; // Reset vertical velocity if not jumping or falling
        player_1->jumping = 0; // Reset jumping state
        player_1->falling = 0; // Reset falling state
    }


	if (player_1->control->left){																																										//Se o botão de movimentação para esquerda do controle do primeiro jogador está ativado... (!)
		player_1->walk(player_1, LEFT);	
        player_1->face = LEFT;
        retorno = 1;																																										//Move o quadrado do primeiro jogador para a esquerda (!)
        																																	//Move o quadrado do primeiro jogador para a esquerda (!)
		//if (collision_2D(player_1,) player_move(player_1, -1, 0, SCREEN_X, SCREEN_Y);																											//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)
	} else player_1->fix_camera_left = 0;
    
	if (player_1->control->right){																																										//Se o botão de movimentação para direita do controle do primeir ojogador está ativado... (!)
		player_1->walk(player_1, RIGHT);	
        player_1->face = RIGHT;
        retorno = 1;																																										//Move o quadrado do primeiro jogador para a direita (!)
        																																				//Move o quadrado do primeiro jogador para a direta (!)
		//if (collision_2D(player_1, player_2)) player_move(player_1, -1, 1, SCREEN_X, SCREEN_Y);																											//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)
	} else player_1->fix_camera_right = 0;																																										//Se o botão de movimentação para direita do controle do primeiro jogador não está ativado, desative a câmera fixa à direita (!) 
    
	if (player_1->control->up) {
        // controle para cima e o player não está pulando nem caindo
        if((player_1->jumping == 0) && (player_1->vy == 0) && (player_1->y == FLOOR_LEVEL - player_1->size)) { // Check if the player is on the floor and not jumping or falling
            player_1->jumping = 1;																																									//Se o botão de movimentação para cima do controle do primeiro jogador está ativado e o jogador não está pulando... (!)
        	player_1->jump(player_1, UP);																																								//Define a velocidade vertical do jogador para -10, iniciando o pulo (!)
            player_1->falling = 1; //ele vai cair depois do pulo		
        	
        }	
        retorno = 2;																																		//Move o quadrado do primeiro jogador para cima (!)
		//if (collision_2D(player_1, player_2)) player_move(player_1, -1, 2, SCREEN_X, SCREEN_Y);																											//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)
	}
    if (player_1->control->down){																																										//Se o botão de movimentação para baixo do controle do primeiro jogador está ativado... (!)
		player_1->squat = 1;
        
        retorno = 3;
		//if (collision_2D(player_1, player_2)) player_move(player_1, -1, 3, SCREEN_X, SCREEN_Y);																											//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)
	} else player_1->squat = 0; // Reset squat state if down control is not pressed

    // Check if the player is in the air (jumping or falling)
    // If the player is jumping or falling, we can return a specific value
    // to indicate that the player is not on the ground.
    // This can be used to prevent certain actions when the player is in the air.

//da pra juntar no control up se PPAAAAAAAAAAH
    if (player_1->falling || player_1->jumping) { // If the player is falling or jumping
        retorno = 2; // Indicate that the player is in the air
    }

    //if(player_1->control->left && (retorno == 3))
    //    retorno = 5; // Indicate that both left and up controls are pressed
    //if(player_1->control->left && (retorno == 4))
    //    retorno = 6; // Indicate that both left and down controls are pressed


    if (player_1->control->fire){																																											//Verifica se o primeiro jogador está atirando (!)
		if (!player_1->gun->timer){																																											//Verifica se a arma do primeiro jogador não está em cooldown (!)
			player_1->shoot(player_1); 																																											//Se não estiver, faz um disparo (!)
			player_1->gun->timer = PISTOL_COOLDOWN;																																							//Inicia o cooldown da arma (!)
		} 
        retorno = 4;																																										//Retorna o valor de retorno da função (!)
	}

	update_bullets(player_1);																																												//Atualiza os disparos do primeiro jogador (!)

    
    return retorno;																																										//Retorna o valor de retorno da função (!)
}
