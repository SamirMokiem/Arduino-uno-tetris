#ifndef KBS_HIGHSCORESCENE_H
#define KBS_HIGHSCORESCENE_H
#include "../../Scene.h"
#include "../../../../../libraries/EEPROM/EEPROM.h"
#include "../../../../EEPROM/EEPROM_WRAPPER.h"

class HighscoreScene : public Scene {
public:
    void setup();
    void on_input(NunchukInput);
    EEPROM_WRAPPER *eepromWrapper = new EEPROM_WRAPPER();
    ~HighscoreScene();
};

#endif //KBS_HIGHSCORESCENE_H
