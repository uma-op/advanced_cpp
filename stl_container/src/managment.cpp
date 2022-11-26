#pragma once

#include "set.h"

template<typename T>
void Set<T>::insert(T elem) {
    ListedRBTree *n = new ListedRBTree(this, elem);
    this->_root->insert(n, this->_pivot);
}

template<typename T>
void Set<T>::erase(T elem) {
    if (this->empty()) return;

    ListedRBTree *cur = this->_root;

    while (!cur->is_leaf()) {
        if (elem < cur->value) {
            cur = cur->left;
        } else if (cur->value < elem) {
            cur = cur->right;
        } else {
            cur->erase();
            break;
        }
    }
}

template<typename T>
size_t Set<T>::size() const {
    return this->_size;
}

template<typename T>
bool Set<T>::empty() const {
    return this->size() == 0;
}
