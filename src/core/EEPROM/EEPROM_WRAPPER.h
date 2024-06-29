#ifndef KBS_EEPROM_WRAPPER_H
#define KBS_EEPROM_WRAPPER_H

#include "../../libraries/EEPROM/EEPROM.h"

enum EEPROM_ADDRESS {
    RESERVED_ONE = 0x00,
    RESERVED_TWO = 0x02,
    RESERVED_THREE = 0x04,

    HIGH_SCORE_ONE = 0x03,
    HIGH_SCORE_TWO = 0x05,
    HIGH_SCORE_THREE = 0x07,
    HIGH_SCORE_FOUR = 0x09,
    HIGH_SCORE_FIVE = 0x0B,
};

class EEPROM_WRAPPER {
public:
    void write(EEPROM_ADDRESS, uint16_t);
    uint16_t read(EEPROM_ADDRESS);
    ~EEPROM_WRAPPER();
private:

    EEPROMClass *eepromClass = new EEPROMClass();
};


#endif //KBS_EEPROM_WRAPPER_H
