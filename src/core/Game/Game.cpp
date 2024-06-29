#include "Game.h"
#include "../../libraries/Nunchuck/Nunchuck.h"
#include "../../GLOBALS.h"
#include "Scene/Scenes/StartScene/StartScene.h"

Scene *active_scene;

void initialize_game() {
    nunchuk_init();
    switch_scene(new StartScene());
}

void game_loop() {
    volatile static uint8_t game_tick_counter = 0;

    // Todo:: improve this code
    volatile static bool allow_x = true;
    volatile static bool allow_y = true;
    volatile static bool allow_z = false;
    volatile static bool allow_c = false;

    if (nunchuk_read()) {
        if (nunchuk_joystickX() > NUNCHUK_DEADZONE && allow_x) {
            allow_x = false;
            active_scene->on_input(NunchukInput::RIGHT);
        }

        if (nunchuk_joystickX() < -NUNCHUK_DEADZONE && allow_x) {
            allow_x = false;
            active_scene->on_input(NunchukInput::LEFT);
        }

        if (nunchuk_joystickX() > -NUNCHUK_DEADZONE && nunchuk_joystickX() < NUNCHUK_DEADZONE) {
            allow_x = true;
        }

        // Larger dead zone for instant drop to prevent unwanted drops
        if (nunchuk_joystickY() > 126 && nunchuk_joystickX() > - 10 && nunchuk_joystickX() < 10 && allow_y) {
            allow_y = false;
            active_scene->on_input(NunchukInput::UP);
        }

        if (nunchuk_joystickY() < -NUNCHUK_DEADZONE && allow_y) {
            allow_y = false;
            active_scene->on_input(NunchukInput::DOWN);
        }

        if (nunchuk_joystickY() > -NUNCHUK_DEADZONE && nunchuk_joystickY() < NUNCHUK_DEADZONE) {
            allow_y = true;
        }

        if (nunchuk_buttonC() && allow_c) {
            allow_c = false;
            active_scene->on_input(NunchukInput::C);
        } else if (!nunchuk_buttonC() && !allow_c) {
            allow_c = true;
        }

        if (nunchuk_buttonZ() && allow_z) {
            allow_z = false;
            active_scene->on_input(NunchukInput::Z);
        } else if (!nunchuk_buttonZ() && !allow_z) {
            allow_z = true;
        }
    }

    active_scene->pre_render();
    active_scene->render();
    active_scene->post_render();

    if (game_tick_counter > active_scene->get_game_ticks()) {
        active_scene->game_tick();
        game_tick_counter = 0;
    }

    game_tick_counter++;
}

void switch_scene(Scene* scene) {
    delete active_scene;
    active_scene = scene;
    active_scene->setup();
    active_scene->game_tick();
}