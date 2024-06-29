#ifndef KBS_SCENE_H
#define KBS_SCENE_H

#include <stdint.h>
#include "../../Nunchuk/NunchukInputs.h"

class Scene {
public:
    virtual void setup();
    virtual void pre_render();
    virtual void render();
    virtual void post_render();
    virtual void game_tick();
    virtual void on_input(NunchukInput);
    virtual uint8_t get_game_ticks();
    virtual void on_message_received(uint8_t adress, uint8_t data);
};


#endif //KBS_SCENE_H
