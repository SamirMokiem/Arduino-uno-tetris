#include "HighscoreScene.h"
#include "../../../../../GLOBALS.h"
#include "../StartScene/StartScene.h"
#include "../../../Game.h"


void HighscoreScene::setup() {
    display.fillScreen(ILI9341_BLACK);
    display.setTextSize(3);
    display.setTextColor(0xFF9D);
    display.setCursor(70, 30);
    display.println("HIGH SCORES");
    display.setTextSize(2);
    display.setCursor(90, 70);
    display.println(eepromWrapper->read(EEPROM_ADDRESS::HIGH_SCORE_ONE));
    display.setCursor(90, 95);
    display.println(eepromWrapper->read(EEPROM_ADDRESS::HIGH_SCORE_TWO));
    display.setCursor(90, 120);
    display.println(eepromWrapper->read(EEPROM_ADDRESS::HIGH_SCORE_THREE));
    display.setCursor(90, 145);
    display.println(eepromWrapper->read(EEPROM_ADDRESS::HIGH_SCORE_FOUR));
    display.setCursor(90, 170);
    display.println(eepromWrapper->read(EEPROM_ADDRESS::HIGH_SCORE_FIVE));
    display.setTextSize(1);
    display.setCursor(110, 195);
    display.println("Press Z to go back");
}

void HighscoreScene::on_input(NunchukInput input) {
    if (input == NunchukInput::Z) {
        switch_scene(new StartScene());
    }
}

HighscoreScene::~HighscoreScene() {
    delete this->eepromWrapper;
}
