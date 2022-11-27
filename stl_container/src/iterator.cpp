#pragma once

#include "set.h"

template<typename T>
Set<T>::iterator::iterator(ListedRBTree *_value, bool _is_forward) : value(_value), is_forward(_is_forward) {}

template<typename T>
typename Set<T>::iterator Set<T>::iterator::operator++(int) {
    iterator it = *this;
    ++(*this);
    return it;
}

template<typename T>
typename Set<T>::iterator& Set<T>::iterator::operator++() {
    if (this->value != this->value->s->_pivot || !this->is_forward) {
        this->value = this->value->next;
    }

    this->is_forward = true;
    return *this;
}

template<typename T>
typename Set<T>::iterator Set<T>::iterator::operator--(int) {
    iterator it = *this;
    --(*this);
    return it;
}

template<typename T>
typename Set<T>::iterator& Set<T>::iterator::operator--() {
    if (this->value != this->value->s->_pivot || this->is_forward) {
        this->value = this->value->prev;
    }

    this->is_forward = false;
    return *this;
}

template<typename T>
bool Set<T>::iterator::operator==(const iterator &other) const {
    return this->value == other.value;
}

template<typename T>
bool Set<T>::iterator::operator!=(const iterator &other) const {
    return !(*this == other);
}

template<typename T>
const T& Set<T>::iterator::operator*() const {
    return this->value->value;
}

template<typename T>
const T* Set<T>::iterator::operator->() const {
    return &(this->value->value);
}
