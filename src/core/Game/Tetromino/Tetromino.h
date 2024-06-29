#ifndef KBS_TETROMINO_H
#define KBS_TETROMINO_H

#include <stdint.h>

class Tetromino {
public:
    Tetromino(uint8_t matrix_size = 3);

    uint8_t **matrix;
    uint8_t **previous_location_coords;
    uint8_t matrix_size;

    char id;

    void rotate();
    void setRow(int8_t row);
    void setCol(int8_t col);

    int8_t getRow();
    int8_t getCol();

    ~Tetromino();
private:
    int8_t row = -1;
    int8_t col = 3;
};

class TETR_I : public Tetromino {
public:
    TETR_I();
};

class TETR_J : public Tetromino {
public:
    TETR_J();
};

class TETR_L : public Tetromino {
public:
    TETR_L();
};

class TETR_O : public Tetromino {
public:
    TETR_O();
};

class TETR_S : public Tetromino {
public:
    TETR_S();
};

class TETR_T : public Tetromino {
public:
    TETR_T();
};

class TETR_Z : public Tetromino {
public:
    TETR_Z();
};

#endif //KBS_TETROMINO_H
