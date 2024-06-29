#ifndef KBS_GAMESCENEHELPER_H
#define KBS_GAMESCENEHELPER_H

namespace GameSceneHelper {

    // Foreground color is in the last 16 bits
    // Background color is in the first 16 bits
    // 00000 000000 00000 - 00000 000000 00000
    // |    Foreground    |    Background    |
    uint32_t get_color_from_tetromino_id(uint8_t id) {
        switch (id) {
            case 'I':
                // Foreground: #C2C3C7 -> 0xC618
                // Background: #FFF2E8 -> 0xFF9Dre
                return 0xC618FF9D;
            case 'J':
                // Foreground: #1D2B53 -> 0x194A
                // Background: #2AACFF -> 0x2D7F
                return 0x194A2D7F;
            case 'L':
                // Foreground: #AB5236 -> 0xAA86
                // Background: #FFA301 -> 0xFD00
                return 0xAA86FD00;
            case 'O':
                // Foreground: #FFA301 -> 0xFD00
                // Background: #FFEC26 -> 0xFF64
                return 0xFD00FF64;
            case 'S':
                // Foreground: #038751 -> 0x042A
                // Background: #01E435 -> 0x0726
                return 0x042A0726;
            case 'T':
                // Foreground: #FF004D -> 0xF809
                // Background: #FF77A8 -> 0xFBB5
                return 0xF809FBB5;
            case 'Z':
                // Foreground: #7E2553 -> 0x792A
                // Background: #FF004D -> 0xF809
                return 0x792AF809;
            case 'G':
                // Foreground: #C2C3C7 -> 0xC618
                // Background: #5F574F -> 0x5AA9
                return 0xC6185AA9;
        }
    }

    Tetromino *get_next_tetromino() {
        switch (rand() % 7) {
            case 0:
                return new TETR_I();
            case 1:
                return new TETR_Z();
            case 2:
                return new TETR_S();
            case 3:
                return new TETR_T();
            case 4:
                return new TETR_L();
            case 5:
                return new TETR_J();
            case 6:
            default:
                return new TETR_O();
        }
    }

    Tetromino *get_next_tetromino_by_id(char id) {
        switch (id) {
            case 'I':
                return new TETR_I();
            case 'Z':
                return new TETR_Z();
            case 'S':
                return new TETR_S();
            case 'T':
                return new TETR_T();
            case 'L':
                return new TETR_L();
            case 'J':
                return new TETR_J();
            case 'O':
            default:
                return new TETR_O();
        }
    }

    bool is_valid_move(uint8_t **play_field, Tetromino *tetromino) {
        for (uint8_t row = 0; row < tetromino->matrix_size; row++) {
            for (uint8_t col = 0; col < tetromino->matrix_size; col++) {
                if (tetromino->matrix[row][col] && (
                        // outside the game bounds
                        tetromino->getCol() + col < 0 ||
                        tetromino->getCol() + col >= PLAYFIELD_WIDTH ||
                        tetromino->getRow() + row >= PLAYFIELD_HEIGHT ||
                        // collides with another piece
                        play_field[tetromino->getRow() + row][tetromino->getCol() + col])
                        ) {
                    return false;
                }
            }
        }

        return true;
    }

}

#endif //KBS_GAMESCENEHELPER_H
