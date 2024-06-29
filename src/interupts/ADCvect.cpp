#include <avr/interrupt.h>

ISR(ADC_vect) {
    OCR0B = ADCH;
}