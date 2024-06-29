#include "../GLOBALS.h"
#include "../core/Game/Scene/Scenes/StartScene/StartScene.h"
#include "../core/Game/Game.h"
#include <avr/interrupt.h>

// Todo:: implement debounce if needed, so far the button has no functionality.
ISR(PCINT1_vect) {
    if (!(PINC & (1 << PINC1))) {
        switch_scene(new StartScene());
    }
}