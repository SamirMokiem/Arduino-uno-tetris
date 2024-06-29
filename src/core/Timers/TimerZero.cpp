/**
 *
 * Used by:
 * LCD Brightness control combined with ADC using a ADC_vect interupt
 *
 **/

#include <avr/io.h>

void initialize_timer_zero() {
    // Set digital 5 as output
    DDRD |= (1 << DDD5);

    // Clear OC0B on Compare Match, set OC0B at BOTTOM, (non-inverting mode)
    TCCR0A |= (1 << COM0B1);

    // Fast PWM 0xFF BOTTOM MAX
    TCCR0A |= (1 << WGM01);
    TCCR0A |= (1 << WGM00);

    // clkI/O/1024 (From prescaler)
    TCCR0B |= (1 << CS02);
    TCCR0B |= (1 << CS00);

    // Set initial timer value on 0
    TCNT0 = 0;

    // Output compare register to toggle COM0B0 on
    OCR0B = 100;
}