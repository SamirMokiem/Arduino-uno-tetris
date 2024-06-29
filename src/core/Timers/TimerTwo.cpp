/**
 *
 * Used by:
 * Game loop
 *
 **/

#include <avr/io.h>

void initialize_timer_two() {
    // Set CTC mode
    TCCR2A |= (1 << WGM21);

    // clk/1024
    TCCR2B |= (1 << CS20) | (1 << CS21) | (1 << CS22);

    // CTC value
    OCR2A = 249;

    // Enable ISR
    TIMSK2 |= (1 << OCIE2A);
}