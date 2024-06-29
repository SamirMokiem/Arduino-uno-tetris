#include <Wire.h>
#include <SPI.h>
#include "GLOBALS.h"
#include "core/Timers/TimerZero.h"
#include "core/ADC/ADC.h"
#include "core/SegmentDisplay/SegmentDisplay.h"
#include "core/SWPush/SWPush.h"
#include "core/Timers/TimerTwo.h"
#include "core/Game/Game.h"

void initialize_I2C();
void initialize_core_components();
void initialize_game();
void initialize_display();

PDQ_ILI9341 display = PDQ_ILI9341();
InfraRed *infraRed = new InfraRed();

volatile bool allowed_to_run_next_game_loop = false;

int main() {
    sei();

    initialize_core_components();
    initialize_display();
    initialize_game();

    // Timer two is responsible for the game loop and thus needs to be enabled after everything game related is done
    initialize_timer_two();

    uint8_t c = 0;

    while (true) {
        if (allowed_to_run_next_game_loop) {
            game_loop();
            allowed_to_run_next_game_loop = false;
            display.fillRect(DISPLAY_PIXEL_WIDTH - 20, 0, 30, 10, ILI9341_BLACK);
            display.setCursor(DISPLAY_PIXEL_WIDTH - 20, 0);
            display.print(c);
            c++;
        }
    }
}

void initialize_display() {
    display.begin();
    display.setRotation(1);
}

void initialize_core_components() {
//    initialize_timer_zero();
//    initialize_ADC();
    initialize_I2C();
    initialize_SWPush();
    write_segment_display(segment_display_modes[0]);
    infraRed->init();
}

void initialize_I2C() {
    Wire.begin();
}
