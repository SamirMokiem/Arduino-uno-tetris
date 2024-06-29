#include "../../../../../GLOBALS.h"
#include "InfraTestScene.h"

static bool toggle = true;
static bool fault = false;

void INFRATESTSCENE::setup() {
    display.println("Setup!");
}

INFRATESTSCENE::~INFRATESTSCENE() {
    display.println("Deconstructing scene");
}

void INFRATESTSCENE::game_tick() {
    // RUNS EVERY 1 SECOND!
    infraRed->send_message(0x6, 0xAA); // for testing

    if (fault){
        display.fillRect(200, 200, 50, 50, ILI9341_BLUE);
        return;
    }

    if (toggle) {
        display.fillRect(200, 200, 50, 50, ILI9341_RED);
    } else{
        display.fillRect(200, 200, 50, 50, ILI9341_GREEN);
    }
}

void INFRATESTSCENE::on_input(NunchukInput input) {
    display.fillRect(0, 0, 50, 50, ILI9341_BLACK);
    display.print("INPUT :");
    display.print(input);
    display.println(' ');
}

void INFRATESTSCENE::render() {
    /// RUNS EVERY 16MS!
}

void INFRATESTSCENE::on_message_received(uint8_t adress, uint8_t data) {
    /// CALLED ONES ON MESSAGE RECEIVED
    if (adress == 0x6 && data == 0xAA){
        toggle = false;
        fault = false;
    } else if (adress == 0x6){
        toggle = true;
        fault = false;
    } else{
        fault = true;
    }
}