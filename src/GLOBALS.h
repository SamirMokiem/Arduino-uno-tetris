#ifndef KBS_GLOBALS_H
#define KBS_GLOBALS_H

#include <SPI.h>

#include "core/Game/Scene/Scenes/GameScene/GameScene.h"

#define DISPLAY_PIXEL_WIDTH 320
#define DISPLAY_PIXEL_HEIGHT 240

#define PLAY_FIELD_START_PIXEL_X 100
#define PLAY_FIELD_START_PIXEL_Y 6

#define NUNCHUK_DEADZONE 120

#include "libraries/PDQ/PDQ_GFX/gfxfont.h"
#include "libraries/PDQ/PDQ_GFX/PDQ_GFX.h"
#include "libraries/PDQ/PDQ_ILI9341/PDQ_ILI9341.h"
#include "libraries/InfraRed/infrared.h"

extern PDQ_ILI9341 display;
extern Scene *active_scene;
extern InfraRed *infraRed;

volatile extern bool allowed_to_run_next_game_loop;

#endif //KBS_GLOBALS_H
