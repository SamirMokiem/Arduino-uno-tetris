/**
 *
 * Used by:
 * LCD Brightness control combined with ADC using a ADC_vect interupt
 *
 **/

#include <avr/io.h>

void initialize_ADC() {
    // set pin A0
    ADMUX &= ~((1 << MUX0) | (1 << MUX1) | (1 << MUX2) | (1 << MUX3));

    // Left adjust ADC result (Result will be in ADCH)
    // Bit    15   14   13   12   11   10   9    8
    // (0x79) ADC9 ADC8 ADC7 ADC6 ADC5 ADC4 ADC3 ADC2 ADCH
    // (0x78) ADC1 ADC0    –    –    –    –    –    – ADCL
    ADMUX |= (1 << ADLAR);

    // VCC as reference voltage
    ADMUX |= (1 << REFS0);

    // ADC Enable
    ADCSRA |= (1 << ADEN);

    // ADC Interrupt enable
    ADCSRA |= (1 << ADIE);

    // ADC Auto Trigger Enable
    ADCSRA |= (1 << ADATE);

    // ADC Pre scaler on slowest - 128
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    // ADC Start Conversion (FREE RUNNING!)
    ADCSRA |= (1 << ADSC);
}