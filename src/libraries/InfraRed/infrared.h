#ifndef INFRARED_H
#define INFRARED_H

#include "../Queue/Queue.h"

class InfraRed {
public:
    void init();
    void send_message(uint8_t adress, uint8_t mes);
    void deqeueu_item();
    void set_hertz(bool value);
    void parse_received_message(uint16_t mes);
    void send_message();
    void read_messsage();
    void IR_toggle(bool value);
    void timer1_init();
    void timer0_init();

    bool get_parity(char input);

    uint16_t parse_send_message(uint8_t adress, uint8_t mes);

    uint8_t ir_compare = 137;
    uint8_t ir_count = 1;

    Queue<uint16_t> *queue = new Queue<uint16_t>(5);
};

#endif