# Just a recent number
cmake_minimum_required(VERSION 3.18.4)
set(CMAKE_CXX_STANDARD 17)

# Determine the Arduino path, with all Arduino related libraries and generic cmake stuff
set(ARDUINO_PATH $ENV{HOME}/Arduino)

if (UNIX AND NOT APPLE)
    set(AVR_PORT /dev/ttyACM0)
endif()

if (APPLE)
    set(AVR_PORT /dev/tty.usbmodem2101)
endif()

# the toolchain to use
set(CMAKE_TOOLCHAIN_FILE ${ARDUINO_PATH}/etc/toolchain-avr.cmake)

# include standard extra targets, like 'upload'
set(CMAKE_PROJECT_INCLUDE ${ARDUINO_PATH}/etc/extratargets-avr.cmake)

# Project name (also the name of the target)
project(KBS)

# Create one target. To generate a list of source files: `find src -name \*.c\*`
add_executable(${PROJECT_NAME}
        src/main.cpp

        src/libraries/Queue/Queue.cpp
        src/libraries/InfraRed/infrared.cpp
        src/libraries/PDQ/PDQ_GFX/glcdfont.c
        src/libraries/PDQ/PDQ_ILI9341/PDQ_ILI9341.cpp

        src/interupts/ADCvect.cpp
        src/interupts/PCINT1vect.cpp
        src/interupts/TIMER0_COMPAvect.cpp
        src/interupts/TIMER1_COMPAvect.cpp
        src/interupts/TIMER2_COMPAvect.cpp

        src/core/ADC/ADC.cpp
        src/core/Timers/TimerZero.cpp
        src/core/Timers/TimerTwo.cpp
        src/core/SegmentDisplay/SegmentDisplay.cpp
        src/core/SWPush/SWPush.cpp
        src/core/EEPROM/EEPROM_WRAPPER.cpp

        src/core/Game/Game.cpp
        src/core/Game/Tetromino/Tetromino.cpp

        src/core/Game/Scene/Scene.cpp
        src/core/Game/Scene/Scenes/GameScene/GameScene.cpp
        src/core/Game/Scene/Scenes/StartScene/StartScene.cpp
        src/core/Game/Scene/Scenes/HighscoreScene/HighscoreScene.cpp
        src/core/Game/Scene/Scenes/InfraTestScene/InfraTestScene.cpp
)

# Add arduino core library
target_include_directories(${PROJECT_NAME} PRIVATE "${ARDUINO_PATH}/include/arduino")
target_link_directories(${PROJECT_NAME} PRIVATE "${ARDUINO_PATH}/lib")
target_link_libraries(${PROJECT_NAME}
        Wire
        SPI
        ArduinoCore
        )

# Rename the output to .elf as we will create multiple files
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME}.elf)

