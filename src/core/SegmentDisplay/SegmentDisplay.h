#ifndef KBS_SEGMENTDISPLAY_H
#define KBS_SEGMENTDISPLAY_H

#include <stdint.h>

// Port expander, used by segment display
#define PCF8574A_PORT_EXPANDER_ADDRESS 0x21

const uint8_t segment_display_modes[] = {
        0xC0,
        0xF9,
        0xA4,
        0xB0,
        0x99,
        0x92,
        0x82,
        0xF8,
        0x80,
        0x90,
        0x88,
        0x83,
        0xC6,
        0xA1,
        0x86,
        0x8E,
        0xFF,
};

void write_segment_display(uint8_t);

#endif //KBS_SEGMENTDISPLAY_H
