#pragma once

#include "set.h"

template<typename T>
typename Set<T>::iterator Set<T>::begin() const {
    return iterator{this->_pivot->next, true};
}

template<typename T>
typename Set<T>::iterator Set<T>::end() const {
    return iterator{this->_pivot, true};
}

template<typename T>
typename Set<T>::const_iterator Set<T>::cbegin() const {
    return const_iterator{this->_pivot->next, true};
}

template<typename T>
typename Set<T>::const_iterator Set<T>::cend() const {
    return const_iterator{this->_pivot, true};
}

template<typename T>
typename Set<T>::iterator Set<T>::find(T elem) const {
    ListedRBTree *cur = this->_root;

    while (!cur->is_leaf()) {
        if (elem < cur->value) {
            cur = cur->left;
        } else if (cur->value < elem) {
            cur = cur->right;
        } else {
            return iterator{cur, true};
        }
    }

    return this->end();
}

template<typename T>
typename Set<T>::iterator Set<T>::lower_bound(T elem) const {
    ListedRBTree *bound = this->_pivot, *cur = this->_root;

    while (!cur->is_leaf()) {
        if (elem < cur->value) {
            bound = cur;
            cur = cur->left;
        } else if (cur->value < elem) {
            cur = cur->right;
        } else {
            return iterator{cur, true};
        }
    }

    return iterator{bound, true};
}
