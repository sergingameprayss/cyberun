#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "character.h"

#define MAX_X 1280
#define MAX_Y 720


void must_init(bool test, const char *description)
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

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

    ALLEGRO_DISPLAY* disp = al_create_display(MAX_X, MAX_Y);
    must_init(disp, "display");


    ALLEGRO_FONT* font = al_create_builtin_font();
    must_init(font, "font");

    must_init(al_init_image_addon(), "image addon");
    ALLEGRO_BITMAP *background = al_load_bitmap("background.png");
    must_init(background, "background");

    must_init(al_init_primitives_addon(), "primitives");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool done = false;

    ALLEGRO_EVENT event;

    struct character *player;
    player = player_create(100, 100); // Create player character



    al_start_timer(timer);
    while(1)
    {
        al_wait_for_event(queue, &event);

        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                player->moviment(player);
                al_draw_scaled_bitmap(background, 0, 0, al_get_bitmap_width(background), al_get_bitmap_height(background), 0, 0, MAX_X, MAX_Y, 0);
                al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 0, 0, "X: %.1f Y: %.1f", player->x, player->y);
                al_draw_filled_rectangle(player->x, player->y, player->x + 10, player->y + 10, al_map_rgb(255, 0, 0));

                al_flip_display();
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                if(event.keyboard.keycode == ALLEGRO_KEY_UP)
                    joystick_up(player->control);
                if(event.keyboard.keycode == ALLEGRO_KEY_DOWN)
                    joystick_down(player->control);
                if(event.keyboard.keycode == ALLEGRO_KEY_LEFT)
                    joystick_left(player->control);
                if(event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
                    joystick_right(player->control);

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

                if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                    done = true;

                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if(done)
            break;

        
    }

    player_destroy(player); // Clean up player character
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}