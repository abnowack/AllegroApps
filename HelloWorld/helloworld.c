#include <stdio.h>
// This header MUST be included in file containing main()
#include <allegro5/allegro.h>

const float FPS = 60;

int main(int argc, char **argv) {
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    bool redraw = true;

    // Initialize Allegro Library
    if(!al_init()) {
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
    display = al_create_display(640, 480);
    if(!display) {
        fprintf(stderr, "failed to create display!\n");
        al_destroy_timer(timer);
        return -1;
    }

    // Create an event queue, a FIFO container where events are stored and removed as they are handled by program
    // Can direct event sources (Timers, Inputs, and Displays) to be handled by different queues
    // Could instead poll continuously, but this is less processor intensive
    event_queue = al_create_event_queue();
    if (!event_queue) {
        fprintf(stderr, "failed to create event_queue!\n");
        al_destroy_timer(timer);
        al_destroy_display(display);
        return -1;
    }

    // Tie display events to our created event queue, such as the buttons in the window bar (minimize, close)
    al_register_event_source(event_queue, al_get_display_event_source(display));
    // Add timer events to event queue
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    // Create RGB color and and clear display to RGB color
    al_clear_to_color(al_map_rgb(0,0,0));

    // Allegro creates a displayed and background display, commands draw to background display
    // Now we flip the displays so the drawn display is visible
    al_flip_display();

    al_start_timer(timer);

    while (1) {
        ALLEGRO_EVENT ev;
        // Since the timer adds regular events coming in a timeout is not needed
        al_wait_for_event(event_queue, &ev);

        // redraw the screen, game logic would be placed here
        if (ev.type == ALLEGRO_EVENT_TIMER) {
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
            al_flip_display();
        }
    }

    // Free memory associated with timer, display, event_queue
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}
