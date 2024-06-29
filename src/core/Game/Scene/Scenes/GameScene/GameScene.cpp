#include "../../../../../GLOBALS.h"
#include "GameSceneHelper.h"
#include "../../../../../bitmaps/tetromino_bitmap.h"
#include "../../../../../libraries/PDQ/PDQ_GFX/Fonts/FreeMonoBold9pt7b.h"
#include "../../../../../libraries/PDQ/PDQ_GFX/Fonts/FreeMonoBold12pt7b.h"
#include "GameScene.h"
#include "../../../../SegmentDisplay/SegmentDisplay.h"
#include "../../../../../libraries/ArduinoSort.h"

void GameScene::setup() {
    // Populate 2d play_field with 0's
    for (uint8_t row = 0; row < PLAYFIELD_HEIGHT; row++) {
        this->play_field[row] = new uint8_t[PLAYFIELD_WIDTH];
        for (uint8_t col = 0; col < PLAYFIELD_WIDTH; col++) {
            this->play_field[row][col] = 0;
        }
    }

    this->tetromino = GameSceneHelper::get_next_tetromino();

    display.fillScreen(ILI9341_BLACK);
    display.setTextColor(0xC618);

    display.setFont(&FreeMonoBold9pt7b);

    display.setCursor(4, 100);
    display.println("HOLD");

    display.setCursor(230, 50);
    display.println("SCORE");

    display.setCursor(230, 100);
    display.println("LVL");

    display.setCursor(230, 150);
    display.println("P2 SCORE");

    display.setCursor(230, 200);
    display.println("P2 LVL");

    display.setFont(NULL);


    display.drawRect(97, 3, 126, 234, 0x5AA9);
    display.drawRect(98, 4, 124, 232, 0xC618);
    display.drawRect(99, 5, 122, 230, 0xFF9D);
}

GameScene::~GameScene() {
    for (uint8_t height = 0; height < PLAYFIELD_HEIGHT; height++) {
        delete[] this->play_field[height];
    }

    delete this->tetromino;
    delete this->eepromWrapper;
    delete[] this->play_field;
}

/**
 * Game tick in this scene is responsible for lowering the active tetromino
 * The faster the game_tick runs, the faster it drops
 * **/
void GameScene::game_tick() {
    if (!this->tetromino || this->game_over) {
        return;
    }

    this->tetromino->setRow(this->tetromino->getRow() + 1);
    this->validate_and_place();
}

void GameScene::validate_and_place() {
    // If not valid move, it hit the play field
    if (!GameSceneHelper::is_valid_move(this->play_field, this->tetromino)) {
        // Restore original row and place tetromino
        this->tetromino->setRow(this->tetromino->getRow() - 1);
        this->place_tetromino(this->tetromino);
        this->process_garbage();

        this->check_and_clear_lines();

        if (this->used_hold) {
            this->used_hold = false;
        }

        delete this->tetromino;

        this->tetromino = GameSceneHelper::get_next_tetromino();
        this->render_play_field();
    }
    this->render_tetromino();
}

void GameScene::on_input(NunchukInput input) {
    if (this->game_over) {
        return;
    }

    if (this->tetromino) {
        if (this->tetromino->getRow() < 0) {
            return;
        }
    }

    if (input == NunchukInput::C) {
        this->hold_tetromino();
        return;
    }
    if (input == NunchukInput::Z) {
        this->tetromino->rotate();
        if (!GameSceneHelper::is_valid_move(this->play_field, this->tetromino)) {
            // Rotate back
            this->tetromino->rotate();
            this->tetromino->rotate();
            this->tetromino->rotate();
        }
    }
    if (input == NunchukInput::UP) {
        this->tetromino->setRow(this->tetromino->getRow() + 1);
        while (GameSceneHelper::is_valid_move(this->play_field, this->tetromino)) {
            this->tetromino->setRow(this->tetromino->getRow() + 1);
        }
    }
    if (input == NunchukInput::DOWN) {
        this->tetromino->setRow(
                this->tetromino->getRow() + 1
        );
    }
    if (input == NunchukInput::LEFT) {
        this->tetromino->setCol(this->tetromino->getCol() - 1);
        if (!GameSceneHelper::is_valid_move(this->play_field, this->tetromino)) {
            this->tetromino->setCol(this->tetromino->getCol() + 1);
        }
    }
    if (input == NunchukInput::RIGHT) {
        this->tetromino->setCol(this->tetromino->getCol() + 1);
        if (!GameSceneHelper::is_valid_move(this->play_field, this->tetromino)) {
            this->tetromino->setCol(this->tetromino->getCol() - 1);
        };
    }
    this->validate_and_place();
}

void GameScene::render() {
    if (this->game_over) {
        if (!this->rendered_game_over) {
            this->handle_game_over();
        }
        return;
    }

    if (!this->rendered_new_score) {
        this->rendered_new_score = true;
        display.setTextSize(2);
        display.setTextColor(0xFF9D);

        display.setCursor(230, 55);
        display.fillRect(230, 55, 48, 14, ILI9341_BLACK);
        display.print(this->current_score);

        display.setCursor(230, 105);
        display.fillRect(230, 105, 48, 14, ILI9341_BLACK);
        display.print(this->current_level);

        display.setCursor(230, 155);
        display.fillRect(230, 155, 48, 14, ILI9341_BLACK);
        display.print(this->player_two_current_score);

        display.setCursor(230, 205);
        display.fillRect(230, 205, 48, 14, ILI9341_BLACK);
        display.print(this->player_two_current_level);

        display.setTextSize(1);
        display.setTextColor(ILI9341_WHITE);

        write_segment_display(segment_display_modes[this->received_garbage]);
    }
}

void GameScene::place_tetromino(Tetromino *tetromino) {
    for (uint8_t row = 0; row < tetromino->matrix_size; row++) {
        for (uint8_t col = 0; col < tetromino->matrix_size; col++) {
            if (tetromino->matrix[row][col]) {

                // game over if piece has any part offscreen
                if (tetromino->getRow() + row < 0) {
                    this->game_over = true;
                    return;
                }

                this->play_field[tetromino->getRow() + row][tetromino->getCol() + col] = tetromino->id;
            }
        }
    }
}

void GameScene::render_play_field() {
    for (uint8_t row = 0; row < PLAYFIELD_HEIGHT; row++) {
        for (uint8_t col = 0; col < PLAYFIELD_WIDTH; col++) {
            if (this->play_field[row][col]) {
                uint32_t full_color = GameSceneHelper::get_color_from_tetromino_id(this->play_field[row][col]);
                uint16_t foreground_color = (full_color >> 16);
                uint16_t background_color = full_color;

                display.drawBitmap(PLAY_FIELD_START_PIXEL_X + (col * 12), PLAY_FIELD_START_PIXEL_Y + (row * 12),
                                   TETROMINO_BITMAP, 12, 12, foreground_color, background_color);
            } else {
                // TODO:: THIS IS SLOW, BAD DEVELOPER >:(
                display.fillRect(PLAY_FIELD_START_PIXEL_X + (col * 12), PLAY_FIELD_START_PIXEL_Y + (row * 12), 12, 12,
                                 ILI9341_BLACK);
            }
        }
    }
}

void GameScene::render_tetromino() {
    for (uint8_t r = 0; r < this->tetromino->matrix_size * this->tetromino->matrix_size; r++) {
        if (this->tetromino->previous_location_coords[r][0] != 0 &&
            this->tetromino->previous_location_coords[r][1] != 0) {
            display.fillRect(this->tetromino->previous_location_coords[r][0],
                             this->tetromino->previous_location_coords[r][1], 12, 12, ILI9341_BLACK);
        }
    }

    uint8_t c = 0;

    for (uint8_t row = 0; row < this->tetromino->matrix_size; row++) {
        for (uint8_t col = 0; col < this->tetromino->matrix_size; col++) {
            if (this->tetromino->matrix[row][col]) {
                uint32_t full_color = GameSceneHelper::get_color_from_tetromino_id(this->tetromino->id);
                uint16_t foreground_color = (full_color >> 16);
                uint16_t background_color = full_color;

                uint8_t x = PLAY_FIELD_START_PIXEL_X + ((this->tetromino->getCol() + col) * 12);
                uint8_t y = PLAY_FIELD_START_PIXEL_Y + ((this->tetromino->getRow() + row) * 12);
                display.drawBitmap(x, y, TETROMINO_BITMAP, 12, 12, foreground_color, background_color);

                this->tetromino->previous_location_coords[c][0] = x;
                this->tetromino->previous_location_coords[c][1] = y;

                c++;
            }
        }
    }
}

void GameScene::check_and_clear_lines() {
    uint8_t line_clears = 0;
    for (int8_t row = PLAYFIELD_HEIGHT - 1; row >= 0;) {
        bool allCells = true;

        for (uint8_t col = 0; col < PLAYFIELD_WIDTH; col++) {
            if (this->play_field[row][col] == 0) {
                allCells = false;
                break;
            }
        }

        if (allCells) {
            line_clears++;

            // drop every row above this one
            for (int8_t r = row; r > 0; r--) {
                for (uint8_t c = 0; c < PLAYFIELD_WIDTH; c++) {
                    this->play_field[r][c] = this->play_field[r - 1][c];
                }
            }
        } else {
            row--;
        }
    }

    this->current_line_clears += line_clears;
    this->total_line_clears += line_clears;

    this->rendered_new_score = false;

    if (line_clears < 4) {
        this->back_to_back_tetris = 0;
    }

    if (line_clears == 1) {
        this->current_score += 4 * (line_clears + 1);
    } else if (line_clears == 2) {
        this->current_score += 10 * (line_clears + 1);
    } else if (line_clears == 3) {
        this->current_score += 30 * (line_clears + 1);
    } else if (line_clears == 4) {
        this->back_to_back_tetris++;
        this->current_score += 120 * (line_clears + 1);
    }

    if (this->current_line_clears > 10) {
        this->current_line_clears = 0;
        this->level_up();
    }

    if (line_clears > 0) {
        infraRed->send_message(0x3, line_clears);
    }
}


/**
 * Level	Frames/drop	Period (sec/drop)	Speed (drops/sec)
 * 0    	48	        .799	            1.25
 * 1    	43	        .715	            1.40
 * 2    	38	        .632	            1.58
 * 3    	33	        .549	            1.82
 * 4    	28	        .466	            2.15
 * 5    	23	        .383	            2.61
 * 6    	18	        .300	            3.34
 * 7    	13	        .216	            4.62
 * 8    	8	        .133	            7.51
 * 9    	6	        .100	            10.02
 * 10–12	5	        .083	            12.02
 * 13–15	4	        .067	            15.05
 * 16–18	3	        .050	            20.03
 * 19–28	2	        .033	            30.05
 * 29+	    1	        .017	            60.10
 * **/
void GameScene::level_up() {
    this->current_level++;

    // Below lvl 8, frames per drop -5 per lvl
    if (this->current_level < 8) {
        this->frames_per_drop -= 5;
        return;
    }

    switch (this->current_level) {
        case 9:
            this->frames_per_drop = 6;
            break;
        case 10:
            this->frames_per_drop = 5;
            break;
        case 13:
            this->frames_per_drop = 4;
            break;
        case 16:
            this->frames_per_drop = 3;
            break;
        case 19:
            this->frames_per_drop = 2;
            break;
        case 29:
            this->frames_per_drop = 1;
            break;
    }
}

void GameScene::hold_tetromino() {
    if (this->used_hold) {
        return;
    }

    this->used_hold = true;

    Tetromino *next_tetromino;
    if (!this->hold_id) {
        next_tetromino = GameSceneHelper::get_next_tetromino();
    } else {
        next_tetromino = GameSceneHelper::get_next_tetromino_by_id(this->hold_id);
    }

    this->hold_id = this->tetromino->id;
    delete this->tetromino;
    this->tetromino = next_tetromino;
    this->render_tetromino();
    this->render_play_field();
    display.fillRect(4, 110, 48, 48, ILI9341_BLACK);

    Tetromino *holding_tetromino = GameSceneHelper::get_next_tetromino_by_id(this->hold_id);

    // Todo:: replace with dry code
    for (uint8_t row = 0; row < holding_tetromino->matrix_size; row++) {
        for (uint8_t col = 0; col < holding_tetromino->matrix_size; col++) {
            if (holding_tetromino->matrix[row][col]) {
                uint32_t full_color = GameSceneHelper::get_color_from_tetromino_id(holding_tetromino->id);
                uint16_t foreground_color = (full_color >> 16);
                uint16_t background_color = full_color;

                uint8_t x = 4 + (col * 12);
                uint8_t y = 110 + (row * 12);
                display.drawBitmap(x, y, TETROMINO_BITMAP, 12, 12, foreground_color, background_color);
            }
        }
    }

    delete holding_tetromino;
}


void GameScene::process_garbage() {
    if (this->received_garbage <= 0) {
        return;
    }

    // Garbage always has 1 line in them to allow the player to clear the garbage
    // TODO:: rand() crashes for some reason!
    uint8_t col_to_spare = 3;

    for (uint8_t to_increase = this->received_garbage; to_increase > 0; to_increase--) {
        for (uint8_t row = 0; row < PLAYFIELD_HEIGHT; row++) {
            for (uint8_t col = 0; col < PLAYFIELD_WIDTH; col++) {
                if (row - 1 <= 0 && this->play_field[row][col]) {
                    this->game_over = true;
                    return;
                }
                this->play_field[row - 1][col] = this->play_field[row][col];
            }
        }
    }

    for (uint8_t row = PLAYFIELD_HEIGHT; row >= PLAYFIELD_HEIGHT - this->received_garbage; row--) {
        for (uint8_t col = 0; col < PLAYFIELD_WIDTH; col++) {
            if (col_to_spare == col) {
                this->play_field[row][col] = 0;
            } else {
                this->play_field[row][col] = 'G';
            }
        }
    }

    this->received_garbage = 0;
    write_segment_display(segment_display_modes[0]);
}

void GameScene::on_message_received(uint8_t adress, uint8_t data) {
    switch (adress) {
        case 0x3:
            this->player_two_current_line_clears += data;

            if (data == 1) {
                this->player_two_current_score += 4 * (data + 1);
            } else if (data == 2) {
                this->player_two_current_score += 10 * (data + 1);
            } else if (data == 3) {
                this->player_two_current_score += 30 * (data + 1);
            } else if (data == 4) {
                this->player_two_current_score += 120 * (data + 1);
            }

            if (this->player_two_current_line_clears > 10) {
                this->player_two_current_line_clears = 0;
                this->player_two_current_level++;
            }

            this->received_garbage = 0;

            if (data == 2) {
                this->received_garbage = 1;
            }

            if (data == 3) {
                this->received_garbage = 2;
            }

            if (data == 4) {
                this->received_garbage = 4;
            }

            this->rendered_new_score = false;

            break;
    }
}

void GameScene::handle_game_over() {
    this->rendered_game_over = true;
    display.fillRect(PLAY_FIELD_START_PIXEL_X, (PLAYFIELD_HEIGHT * 12) / 2 - 3, PLAYFIELD_WIDTH * 12, 17, 0xFF9D);
    display.setCursor(PLAY_FIELD_START_PIXEL_X + 10, (PLAYFIELD_HEIGHT * 12) / 2 + 4);
    display.setTextColor(ILI9341_RED);
    display.setFont(&FreeMonoBold9pt7b);
    display.print("GAME OVER");
    display.setTextColor(ILI9341_WHITE);
    display.setFont(NULL);
    display.setTextSize(1);

    // Create array with all highscores + current scores
    uint16_t highscores[7] = {
            this->eepromWrapper->read(HIGH_SCORE_ONE),
            this->eepromWrapper->read(HIGH_SCORE_TWO),
            this->eepromWrapper->read(HIGH_SCORE_THREE),
            this->eepromWrapper->read(HIGH_SCORE_FOUR),
            this->eepromWrapper->read(HIGH_SCORE_FIVE),
            this->current_score,
            this->player_two_current_score
    };

    // Sort the array
    sortArrayReverse(highscores, 7);

    // Replace highscores with sorted highscores
    this->eepromWrapper->write(HIGH_SCORE_ONE, highscores[0]);
    this->eepromWrapper->write(HIGH_SCORE_TWO, highscores[1]);
    this->eepromWrapper->write(HIGH_SCORE_THREE, highscores[2]);
    this->eepromWrapper->write(HIGH_SCORE_FOUR, highscores[3]);
    this->eepromWrapper->write(HIGH_SCORE_FIVE, highscores[4]);
}
