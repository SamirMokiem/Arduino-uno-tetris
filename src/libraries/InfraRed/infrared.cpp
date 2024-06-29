#include <avr/io.h>
#include "infrared.h"

#include "../../GLOBALS.h"

#define SIGCOMP 255
#define SIGCOUN 10 // was 8

/* <0.065 difference> <~38Khz> */
#define IRCOMPSLOW 59
#define IRCOUNSLOW 6

/* <0.181 difference> <~56Khz> */
#define IRCOMPFAST 137
#define IRCOUNFAST 1

#define ADRESSPARITY 9
#define DATAPARTITY 0
#define ADRESSLOCAL 0x3C00
#define DATALOCAL 0x01FE

#define BIT (((PIND&(1<<PORTD2))>>PORTD2)^1)

static uint16_t message = 0x0000;
static bool sending = false;

#pragma region public

// initialize infra red communication
void InfraRed::init() {
    DDRD |= (1 << DDD6);
    timer0_init();
    timer1_init();
}

// initialize message sending
void InfraRed::send_message(uint8_t adress, uint8_t mes) {
    uint16_t pakket = parse_send_message(adress, mes);
    this->queue->push(pakket);
    deqeueu_item();
}

void InfraRed::deqeueu_item(){
    if (this->queue->get_length() == 0 || sending){
        return;
    }

    message = this->queue->pop();
    sending = true;
}

// set hertz, 
// true => 38Hrz, 
// false => 56Hrz
void InfraRed::set_hertz(bool value) {
    if (value) {
        this->ir_compare = IRCOMPSLOW;
        this->ir_count = IRCOUNSLOW;
    } else {
        this->ir_compare = IRCOMPFAST;
        this->ir_count = IRCOUNFAST;
    }
}

#pragma endregion
#pragma region private

// incode message into custom protocol
uint16_t InfraRed::parse_send_message(uint8_t adress, uint8_t mes) {
    uint16_t result = 0xC000;                           // set initialize bits
    result |= (adress << 10);                           // set adress bits
    result |= (get_parity(adress) << ADRESSPARITY);     // set parity bit
    result |= (mes << 1);                               // set data byte
    result |= (get_parity(mes) << DATAPARTITY);         // set parity bit
    return result;
}

// decode message from custom protocol
void InfraRed::parse_received_message(uint16_t mes) {
    char adress = 0x00;
    char data = 0x00;

    adress = (mes & ADRESSLOCAL) >> 10;             // get adress
    data = (mes & DATALOCAL) >> 1;                  // get data
    bool adress_parity = get_parity(adress);        // generate partity
    bool data_parity = get_parity(data);            // generate partity
    bool adress_p = (mes & 0x0200) >> ADRESSPARITY; // get adress parity
    bool data_p = (mes & 0x0001);                   // get data partity
    if (!((adress_parity != adress_p) || (data_parity != data_p))) {
        active_scene->on_message_received(adress, data);
    }
}

// send last bit of message variable ones per update
void InfraRed::send_message() {
    static unsigned int counter = 0;
    // check if message needs to be send
    if (sending) {
        if (counter < 16){
            IR_toggle((message & 0x8000) >> 15); // get last bit
            message <<= 1;
        }
        if (counter >= 26) {
            // if sending is done, stop loop
            sending = false;
            counter = 0;
            deqeueu_item();
        } else
            counter++;
    }
}

void InfraRed::read_messsage() {
    static uint16_t read_mes = 0x0000;
    static char read_counter = 15;

    if (read_counter >= 15) {
        // look for initialize bits
        read_mes &= ~2;
        read_mes <<= 1;
        read_mes |= BIT;
        if (read_mes == 3) {
            // if initialize bits are found
            read_counter = 1;
        }
    } else {
        // save data
        read_mes <<= 1;
        read_mes |= BIT;
        read_counter++;
        if (read_counter >= 15) {
            // if message has been received
            parse_received_message(read_mes);
        }
    }
}

void InfraRed::IR_toggle(bool value) {
    if (!value) {
        // disable interupt
        TIMSK0 &= ~(1 << OCIE0A);
        PORTD &= ~(1 << PORTD6);
    } else {
        // enable interupt
        TIMSK0 |= (1 << OCIE0A);
    }
}

// generate partiy bit based on input
bool InfraRed::get_parity(char input) {
    bool parity = false;
    while (input) {
        parity = !parity;
        input = input & (input - 1);
    }
    return parity;
}

void InfraRed::timer0_init() {
    TCCR0A |= (1 << WGM01); // set on CTC
    TCCR0B |= (1 << CS00); // scaler
    OCR0A = this->ir_compare; // compare
}

void InfraRed::timer1_init() {
    TCCR1A |= (1 << WGM11); // set on CTC
    TCCR1B |= (1 << CS10); // scaler
    OCR1A = SIGCOMP; // compare
    TIMSK1 |= (1 << OCIE1A); // enable interupt
}

#pragma endregion