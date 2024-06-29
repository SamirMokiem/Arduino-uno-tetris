#ifndef KBS_INFRATESTSCENE_H
#define KBS_INFRATESTSCENE_H

#include "../../Scene.h"

class INFRATESTSCENE : public Scene {
public:
    uint8_t get_game_ticks() {
        return 60;
    }

    void game_tick();
    void setup();
    void render();

    void on_input(NunchukInput);

    void on_message_received(uint8_t adress, uint8_t data);

    ~INFRATESTSCENE();
};


#endif //KBS_INFRATESTSCENE_H
