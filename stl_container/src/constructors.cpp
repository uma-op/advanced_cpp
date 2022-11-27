#pragma once

#include "set.h"

template<typename T>
Set<T>::Set() : _root(new ListedRBTree(this)), _pivot(new ListedRBTree(this)), _size(0) {
    this->_pivot->next = this->_pivot;
    this->_pivot->prev = this->_pivot;
}

template<typename T>
template<typename InputIt>
Set<T>::Set(InputIt begin, InputIt end) : Set() {
    for (; begin != end; begin++) this->insert(*begin);
}

template<typename T>
Set<T>::Set(std::initializer_list<T> data) : Set() {
    for (T elem : data) {
        this->insert(elem);
    }
}

template<typename T>
Set<T>::Set(const Set& other) : Set() {
    iterator b = other.begin();
    iterator e = other.end();

    while (b != e) {
        this->insert(*b);
        b++;
    }
}

template<typename T>
Set<T>::~Set() {
    ListedRBTree *b = this->_pivot->next, *t;
    ListedRBTree *e = this->_pivot;

    while (b != e) {
        t = b->next;
        delete b;
        b = t;
    }

    delete this->_pivot;
}

template<typename T>
Set<T>& Set<T>::operator=(const Set& other) {
    if (this == &other) return *this;

    this->~Set();

    this->_root = new ListedRBTree(this);
    this->_pivot = new ListedRBTree(this);

    this->_pivot->next = this->_pivot;
    this->_pivot->prev = this->_pivot;

    this->_size = 0;

    iterator b = other.begin();
    iterator e = other.end();

    while (b != e) {
        this->insert(*b);
        b++;
    }

    return *this;
}
