#include <avr/interrupt.h>
#include "../GLOBALS.h"

// generate constant signal
ISR(TIMER0_COMPA_vect) {
    static unsigned int timer = 0;

    if (timer >= infraRed->ir_count) {
        PORTD ^= (1 << PORTD6);
        timer = 0;
    } else
        timer++;
}