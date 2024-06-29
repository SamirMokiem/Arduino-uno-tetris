#include "Tetromino.h"

TETR_I::TETR_I() : Tetromino(4) {
    uint8_t **matrix = new uint8_t *[4];
    matrix[0] = new uint8_t[4]{0, 0, 0, 0};
    matrix[1] = new uint8_t[4]{1, 1, 1, 1};
    matrix[2] = new uint8_t[4]{0, 0, 0, 0};
    matrix[3] = new uint8_t[4]{0, 0, 0, 0};


    this->matrix = matrix;
    this->id = 'I';
    this->setCol(4);
}

TETR_J::TETR_J() : Tetromino() {
    uint8_t **matrix = new uint8_t *[3];
    matrix[0] = new uint8_t[3]{1, 0, 0};
    matrix[1] = new uint8_t[3]{1, 1, 1};
    matrix[2] = new uint8_t[3]{0, 0, 0};

    this->matrix = matrix;
    this->id = 'J';
}

TETR_L::TETR_L() : Tetromino() {
    uint8_t **matrix = new uint8_t *[3];
    matrix[0] = new uint8_t[3]{0, 0, 1};
    matrix[1] = new uint8_t[3]{1, 1, 1};
    matrix[2] = new uint8_t[3]{0, 0, 0};

    this->matrix = matrix;
    this->id = 'L';
}

TETR_O::TETR_O() : Tetromino(2) {
    uint8_t **matrix = new uint8_t *[2];
    matrix[0] = new uint8_t[2]{1, 1};
    matrix[1] = new uint8_t[2]{1, 1};

    this->matrix = matrix;
    this->id = 'O';
    this->setCol(4);
}

TETR_S::TETR_S() : Tetromino() {
    uint8_t **matrix = new uint8_t *[3];
    matrix[0] = new uint8_t[3]{0, 1, 1};
    matrix[1] = new uint8_t[3]{1, 1, 0};
    matrix[2] = new uint8_t[3]{0, 0, 0};

    this->matrix = matrix;
    this->id = 'S';
}

TETR_T::TETR_T() : Tetromino() {
    uint8_t **matrix = new uint8_t *[3];
    matrix[0] = new uint8_t[3]{0, 1, 0};
    matrix[1] = new uint8_t[3]{1, 1, 1};
    matrix[2] = new uint8_t[3]{0, 0, 0};

    this->matrix = matrix;
    this->id = 'T';
}

TETR_Z::TETR_Z() : Tetromino() {
    uint8_t **matrix = new uint8_t *[3];
    matrix[0] = new uint8_t[3]{1, 1, 0};
    matrix[1] = new uint8_t[3]{0, 1, 1};
    matrix[2] = new uint8_t[3]{0, 0, 0};

    this->matrix = matrix;
    this->id = 'Z';
}

Tetromino::Tetromino(uint8_t matrix_size) {
    this->matrix_size = matrix_size;

    this->previous_location_coords = new uint8_t *[this->matrix_size * this->matrix_size];

    for (uint8_t r = 0; r < this->matrix_size * this->matrix_size; r++) {
        this->previous_location_coords[r] = new uint8_t[2]{0, 0};
    }
}

Tetromino::~Tetromino() {
    for (uint8_t r = 0; r < this->matrix_size * this->matrix_size; r++) {
        delete[] this->previous_location_coords[r];
    }

    delete[] this->previous_location_coords;

    for (uint8_t x = 0; x < this->matrix_size; x++) {
        delete[] this->matrix[x];
    }

    delete[] this->matrix;
}


void Tetromino::setRow(int8_t row) {
    this->row = row;
}

void Tetromino::setCol(int8_t col) {
    this->col = col;
}

int8_t Tetromino::getRow() {
    return this->row;
}

int8_t Tetromino::getCol() {
    return this->col;
}

// TODO:: Make this clockwise, not counter clockwise
void Tetromino::rotate() {
    for (int i = 0; i < this->matrix_size; i = i + 1) {
        for (int j = i; j < this->matrix_size; j = j + 1) {
            uint8_t hold = this->matrix[i][j];
            this->matrix[i][j] = this->matrix[j][i];
            this->matrix[j][i] = hold;
        }
    }

    int i = 0, j = 0, column = 0;
    while (column < this->matrix_size) {
        i = 0, j = this->matrix_size - 1;
        while (i < j) {
            uint8_t hold = this->matrix[i][column];
            this->matrix[i][column] = this->matrix[j][column];
            this->matrix[j][column] = hold;
            i = i + 1;
            j = j - 1;
        }
        column = column + 1;
    }
}
