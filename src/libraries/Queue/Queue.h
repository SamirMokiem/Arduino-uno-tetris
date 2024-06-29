#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>

template<typename T>
class Queue {
public:
    Queue(uint8_t);
    void init(uint8_t length);
    void push(T item);
    T pop();
    uint8_t get_length();
    ~Queue();
private:
    uint8_t start_pointer = 0;
    uint8_t end_pointer = 0;
    uint8_t length = 0;
    T* arr[];
};

template class Queue<uint16_t>;

#endif