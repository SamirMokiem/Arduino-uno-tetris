#include "SegmentDisplay.h"
#include <Wire.h>

void write_segment_display(uint8_t value) {
    Wire.beginTransmission(PCF8574A_PORT_EXPANDER_ADDRESS);
    Wire.write(value);
    Wire.endTransmission();
}