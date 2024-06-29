#include <avr/interrupt.h>
#include "../GLOBALS.h"

// manage data income and outgoing
ISR(TIMER1_COMPA_vect) {
    static unsigned int timer = 0;
    if (timer >= 8) {
        infraRed->send_message();
        infraRed->read_messsage();
        timer = 0;
    } else
        timer++;
}