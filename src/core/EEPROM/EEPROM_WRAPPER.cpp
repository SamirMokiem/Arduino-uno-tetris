#include "EEPROM_WRAPPER.h"

void EEPROM_WRAPPER::write(EEPROM_ADDRESS address, uint16_t data) {
    // Write right part
    this->eepromClass->begin();
    this->eepromClass->write(address, data);
    this->eepromClass->end();

    // Write left part
    data = data >> 8;
    uint8_t next_addr = address + 1;
    this->eepromClass->begin();
    this->eepromClass->write(next_addr, data);
    this->eepromClass->end();
}

uint16_t EEPROM_WRAPPER::read(EEPROM_ADDRESS address) {
    this->eepromClass->begin();
    uint16_t result = this->eepromClass->read(address);
    this->eepromClass->end();

    uint8_t next_addr = address + 1;
    this->eepromClass->begin();
    result |= this->eepromClass->read(next_addr) << 8;
    this->eepromClass->end();

    return result;
}

EEPROM_WRAPPER::~EEPROM_WRAPPER() {
    delete this->eepromClass;
}
