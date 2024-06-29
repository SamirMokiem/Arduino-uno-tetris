/**
 *
 * Physical button on the game-shield
 *
 * Used by:
 * Nothing
 *
 **/

#include <avr/io.h>

void initialize_SWPush() {
    // Enable port C interrupt zone
    PCICR |= (1 << PCIE1);

    // Enable pin change interrupt
    PCMSK1 |= (1 << PCINT9);

    // Enable pull up
    PORTC |= (1 << PORTC1);
}