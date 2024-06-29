#include "StartScene.h"
#include "../../../../../GLOBALS.h"
#include "../../../../../bitmaps/logo_chars.h"
#include "../../../Game.h"
#include "../HighscoreScene/HighscoreScene.h"

void StartScene::setup() {
    display.fillScreen(ILI9341_BLACK);
    uint8_t start_x = 80;
    uint8_t start_y = 35;
    display.drawXBitmap(start_x, start_y, LOGO_CHARS::T, T_WIDTH, T_HEIGHT, 0xF809);
    display.drawXBitmap(start_x + 30, start_y, LOGO_CHARS::E, E_WIDTH, E_HEIGHT, 0xFD00);
    display.drawXBitmap(start_x + 60, start_y, LOGO_CHARS::T, T_WIDTH, T_HEIGHT, 0xFF64);
    display.drawXBitmap(start_x + 90, start_y, LOGO_CHARS::R, R_WIDTH, R_HEIGHT, 0x0726);
    display.drawXBitmap(start_x + 125, start_y, LOGO_CHARS::I, I_WIDTH, I_HEIGHT, 0x2D7F);
    display.drawXBitmap(start_x + 135, start_y, LOGO_CHARS::S, S_WIDTH, S_HEIGHT, 0xFBB5);

    display.setTextSize(3);
    display.setTextColor(0xFF9D);

    display.setCursor(120, 110);
    display.print("START!");

    display.setCursor(75, 145);
    display.print("HIGH SCORES");

    display.setFont(NULL);
    display.setTextSize(1);
    this->render_active_item();
}

void StartScene::render_active_item() {
    display.setTextSize(3);
    display.setTextColor(0xFF9D);

    display.setTextColor(this->selected_item ? ILI9341_BLACK : 0xF809);
    display.setCursor(100, 110);
    display.print(">");

    display.setTextColor(this->selected_item ? 0xF809 : ILI9341_BLACK);
    display.setCursor(55, 145);
    display.print(">");
    display.setTextColor(ILI9341_WHITE);
    display.setTextSize(0);
}

void StartScene::on_input(NunchukInput input) {
    if (input == NunchukInput::Z) {
        if (this->selected_item) {
            switch_scene(new HighscoreScene());
        } else {
            infraRed->send_message(0xE, 0x00);
            switch_scene(new GameScene());
        }
    }
    if (input == NunchukInput::UP) {
        this->selected_item = 0;
    }
    if (input == NunchukInput::DOWN) {
        this->selected_item = 1;
    }

    this->render_active_item();
}

void StartScene::on_message_received(uint8_t adress, uint8_t data){
    switch (adress){
        case 0xE:
            switch_scene(new GameScene());
            break;
    }
}