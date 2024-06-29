# Arduino Uno Tetris (jan-2023)

## Overview

This project brings the classic game of Tetris to the Arduino Uno. The majority of the source code has been developed by
me, with enhancements made to the PDQ library for increased compatibility and improved rendering speed. Please respect
the original copyright/licenses associated with any third-party code.

This was my first time using low level c++, using registers and with multiple hardware components.

## Contributions

- **Main Code and PDQ Library Modifications:** Developed by me.
- **Infrared Protocol and Communication:** The InfraRed, ArduinoSort, and Queue libraries were written by another team
  member.

## Note

This project is not intended to be built and run as-is. It should only be used for inspiration and guidance.
Additionally, please be aware that the infrared communication may not work consistently at all times.

## Small Demos

### Pre-Alpha Gameplay Loop Test
Shows the core gameplay using the nunchuk for control.

![gameplay_loop.gif](github_demo/gameplay_loop.gif)

### Display Brightness Control Using a Potentiometer
The brightness control has been disabled in the final version due to the limited number of hardware timers in the Arduino Uno.

![brightness.gif](github_demo/brightness.gif)

### Synchronized Game Start and Score Sync
Demonstrates synchronized game start and communication with P2 for score updates.

![sync_start_multiplayer_score.gif](github_demo/sync_start_multiplayer_score.gif)

### Main Menu Button
Shows the main menu button functionality.

![main_menu_button.gif](github_demo/main_menu_button.gif)

### High Scores Stored in EEPROM, Garbage, and Hold System
- High score storage system
- Early version of the garbage system, which caused a crash in this demo. In the final version, garbage is synced across Arduinos, but in this demo, it was local only.
- Piece hold system

![high_score_garbage_hold.gif](github_demo/high_score_garbage_hold.gif)
