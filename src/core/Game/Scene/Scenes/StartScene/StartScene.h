#ifndef KBS_STARTSCENE_H
#define KBS_STARTSCENE_H

#include "../../../../Nunchuk/NunchukInputs.h"
#include "../../Scene.h"

class StartScene : public Scene {
public:
    void setup();
    void on_input(NunchukInput);
    void render_active_item();
    void on_message_received(uint8_t adress, uint8_t data);

    uint8_t selected_item = 0; // Start = 0, Highscore = 1
};

#endif //KBS_STARTSCENE_H