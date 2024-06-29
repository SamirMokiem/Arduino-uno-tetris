#include "Queue.h"

template<typename T>
Queue<T>::Queue(uint8_t length) {
    this->length = length;
    this->arr[length] = new T[length];
}

template<typename T>
void Queue<T>::push(T item) {
    this->arr[end_pointer] = item;
    this->end_pointer = this->end_pointer >= this->length - 1 ? 0 : this->end_pointer + 1;
}

template<typename T>
T Queue<T>::pop() {
    T item = this->arr[start_pointer];
    this->start_pointer = this->start_pointer >= this->length - 1 ? 0 : this->start_pointer + 1;
    return item;
}

template<typename T>
uint8_t Queue<T>::get_length() {
    uint8_t counter = 0;
    for (int index = this->start_pointer; index < this->end_pointer; index = index >= this->length - 1 ? 0 : index + 1)
        counter++;
    return counter;
}

template<typename T>
Queue<T>::~Queue() {
    delete[] this->arr;
}