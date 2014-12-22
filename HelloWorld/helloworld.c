#include <stdio.h>
// This header MUST be included in file containing main()
#include <allegro5/allegro.h>

int main(int argc, char **argv) {
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;

    // Initialize Allegro Library
    if(!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }

    // Create display with specified width and height
    display = al_create_display(640, 480);
    if(!display) {
        fprintf(stderr, "failed to create display!\n");
        return -1;
    }

    // Create an event queue, a FIFO container where events are stored and removed as they are handled by program
    // Can direct event sources (Timers, Inputs, and Displays) to be handled by different queues
    // Could instead poll continuously, but this is less processor intensive
    event_queue = al_create_event_queue();
    if (!event_queue) {
        fprintf(stderr, "failed to create event_queue!\n");
        al_destroy_display(display);
        return -1;
    }

    // Tie display events to our created event queue, such as the buttons in the window bar (minimize, close)
    al_register_event_source(event_queue, al_get_display_event_source(display));

    // Create RGB color and and clear display to RGB color
    al_clear_to_color(al_map_rgb(0,0,0));

    // Allegro creates a displayed and background display, commands draw to background display
    // Now we flip the displays so the drawn display is visible
    al_flip_display();

    while (1) {
        ALLEGRO_EVENT ev;
        // create timeout of 60 ms
        ALLEGRO_TIMEOUT timeout;
        al_init_timeout(&timeout, 0.06);

        // wait until event arrives or timeout has elapsed
        bool get_event = al_wait_for_event_until(event_queue, &ev, &timeout);

        // if we have an event, and it corresponds to a close event, break the loop and exit
        if (get_event && ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }

        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_flip_display();
    }

    // Free memory associated with display
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}
