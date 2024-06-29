#include <avr/interrupt.h>
#include "../GLOBALS.h"

ISR(TIMER2_COMPA_vect) {
    if (!allowed_to_run_next_game_loop) {
        allowed_to_run_next_game_loop = true;
    }
}