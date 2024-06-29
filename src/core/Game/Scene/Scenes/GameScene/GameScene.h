#ifndef KBS_GAMESCENE_H
#define KBS_GAMESCENE_H

#include "../../Scene.h"
#include "../../../Tetromino/Tetromino.h"
#include "../../../../EEPROM/EEPROM_WRAPPER.h"

#define PLAYFIELD_WIDTH 10
#define PLAYFIELD_HEIGHT 19

class GameScene : public Scene {
public:
    uint8_t get_game_ticks() {
        return this->frames_per_drop;
    }

    void game_tick();
    void setup();
    void render();

    void on_input(NunchukInput);

    void level_up();

    void validate_and_place();
    void check_and_clear_lines();

    void place_tetromino(Tetromino *tetromino);
    void render_tetromino();
    void render_play_field();
    void hold_tetromino();
    void process_garbage();

    void on_message_received(uint8_t adress, uint8_t data);

    ~GameScene();
private:
    uint8_t **play_field = new uint8_t *[PLAYFIELD_HEIGHT];

    EEPROM_WRAPPER *eepromWrapper = new EEPROM_WRAPPER();

    bool used_hold = false;
    char hold_id = 0;

    // Default drop speed is 48 on lvl 0
    uint8_t frames_per_drop = 48;
    uint8_t current_level = 0;

    // Todo:: Increase score limit!
    uint8_t current_line_clears = 0;
    uint16_t total_line_clears = 0;
    uint16_t current_score = 0;
    uint8_t back_to_back_tetris = 0;

    // MAX 17!
    uint8_t received_garbage = 0;

    bool game_over = false;
    bool rendered_game_over = false;
    bool rendered_new_score = false;

    Tetromino *tetromino;

    // Player 2
    uint8_t player_two_current_line_clears = 0;
    uint8_t player_two_current_level = 0;
    uint16_t player_two_current_score = 0;

    void handle_game_over();
};


#endif //KBS_GAMESCENE_H
