#include <stdio.h>
// This header MUST be included in file containing main()
#include <allegro5/allegro.h>

const float FPS = 60;
const int SCREEN_W = 640;
const int SCREEN_H = 480;
const int BOUNCER_SIZE = 32;

int main(int argc, char **argv) {
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_BITMAP *bouncer = NULL;
    float bouncer_x = SCREEN_W / (float) 2.0 - BOUNCER_SIZE / (float) 2.0;
    float bouncer_y = SCREEN_H / (float) 2.0 - BOUNCER_SIZE / (float) 2.0;
    float bouncer_dx = -(float) 4.0, bouncer_dy = (float) 4.0;
    bool redraw = true;

    // Initialize Allegro Library
    if (!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }

    // set timer tick rate in seconds
    timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        fprintf(stderr, "failed to create timer!\n");
        return -1;
    }

    // Create display with specified width and height
    display = al_create_display(SCREEN_W, SCREEN_H);
    if (!display) {
        fprintf(stderr, "failed to create display!\n");
        al_destroy_timer(timer);
        return -1;
    }

    // create bitmap with size
    bouncer = al_create_bitmap(BOUNCER_SIZE, BOUNCER_SIZE);
    if (!bouncer) {
        fprintf(stderr, "failed to create bouncer bitmap!\n");
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }

    // target bitmap is bitmap in which all drawing functions act on
    al_set_target_bitmap(bouncer);
    // create magenta square
    al_clear_to_color(al_map_rgb(255, 0, 255));
    // return to drawing target to display
    al_set_target_bitmap(al_get_backbuffer(display));

    // Create an event queue, a FIFO container where events are stored and removed as they are handled by program
    // Can direct event sources (Timers, Inputs, and Displays) to be handled by different queues
    // Could instead poll continuously, but this is less processor intensive
    event_queue = al_create_event_queue();
    if (!event_queue) {
        fprintf(stderr, "failed to create event_queue!\n");
        al_destroy_bitmap(bouncer);
        al_destroy_timer(timer);
        al_destroy_display(display);
        return -1;
    }

    // Tie display events to our created event queue, such as the buttons in the window bar (minimize, close)
    al_register_event_source(event_queue, al_get_display_event_source(display));
    // Add timer events to event queue
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    // Create RGB color and and clear display to RGB color
    al_clear_to_color(al_map_rgb(0, 0, 0));

    // Allegro creates a displayed and background display, commands draw to background display
    // Now we flip the displays so the drawn display is visible
    al_flip_display();

    al_start_timer(timer);

    while (1) {
        ALLEGRO_EVENT ev;
        // Since the timer adds regular events coming in a timeout is not needed
        al_wait_for_event(event_queue, &ev);

        // redraw the screen, bouncer window collision logic
        if (ev.type == ALLEGRO_EVENT_TIMER) {
            if (bouncer_x < 0 || bouncer_x > SCREEN_W - BOUNCER_SIZE) {
                bouncer_dx = -bouncer_dx;
            }

            if (bouncer_y < 0 || bouncer_y > SCREEN_H - BOUNCER_SIZE) {
                bouncer_dy = -bouncer_dy;
            }

            bouncer_x += bouncer_dx;
            bouncer_y += bouncer_dy;

            redraw = true;
        }
            // if we have an event, and it corresponds to a close event, break the loop and exit
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }

        // A test that the event queue is empty is needed in case the queue falls behind (lag)
        // Lagging events are skipped over to empty the queue
        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(bouncer, bouncer_x, bouncer_y, 0);
            al_flip_display();
        }
    }

    // Free memory associated with timer, display, event_queue
    al_destroy_bitmap(bouncer);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}
