#pragma once

#include "set.h"

template<typename T>
Set<T>::ListedRBTree::ListedRBTree(Set *_s) :
    parent(nullptr),
    left(nullptr),
    right(nullptr),
    prev(nullptr),
    next(nullptr),
    color(BLACK),
    s(_s) {}

template<typename T>
Set<T>::ListedRBTree::ListedRBTree(Set *_s, T _value) :
    parent(nullptr),
    left(nullptr),
    right(nullptr),
    prev(nullptr),
    next(nullptr),
    color(RED),
    s(_s),
    value(_value) {}

template<typename T>
Set<T>::ListedRBTree::~ListedRBTree() {
    if (this->is_leaf()) return;
    if (this->right->is_leaf()) delete this->right;
    if (this->left->is_leaf()) delete this->left;
}

template<typename T>
void Set<T>::ListedRBTree::rotate_left() {
    if (this->right->is_leaf()) return;

    ListedRBTree *p = this->right;
    p->parent = this->parent;

    if (p->parent == nullptr) {
        this->s->_root = p;
    } else if (this == this->parent->left) {
        this->parent->left = p;
    } else {
        this->parent->right = p;
    }

    this->right = p->left;
    p->left->parent = this;

    p->left = this;
    this->parent = p;
}

template<typename T>
void Set<T>::ListedRBTree::rotate_right() {
    if (this->left->is_leaf()) return;

    ListedRBTree *p = this->left;

    if (this->parent == nullptr) {
        this->s->_root = p;
    } else if (this == this->parent->left) {
        this->parent->left = p;
    } else {
        this->parent->right = p;
    }

    p->parent = this->parent;

    this->left = p->right;
    p->right->parent = this;

    p->right = this;
    this->parent = p;
}

template<typename T>
void Set<T>::ListedRBTree::insert(ListedRBTree *other, ListedRBTree *before) {
    if (this->is_leaf()) {
        this->s->_size++;

        ListedRBTree *after = before->next;

        before->next = other;
        after->prev = other;

        other->prev = before;
        other->next = after;

        if (this->parent == nullptr) {
            this->s->_root = other;
        } else if (this == this->parent->left) {
            this->parent->left = other;
        } else {
            this->parent->right = other;
        }

        other->parent = this->parent;

        other->left = new ListedRBTree(this->s);
        other->left->parent = other;

        other->right = new ListedRBTree(this->s);
        other->right->parent = other;

        delete this;

        other->ic1();

        return;
    }

    if (other->value < this->value) {
        this->left->insert(other, before);
    } else if (this->value < other->value) {
        this->right->insert(other, this);
    }
}

template<typename T>
void Set<T>::ListedRBTree::ic1() {
    if (this->parent == nullptr) {
        this->color = BLACK;
    } else {
        this->ic2();
    }
}

template<typename T>
void Set<T>::ListedRBTree::ic2() {
    if (this->parent->color == BLACK) return;
    this->ic3();
}

template<typename T>
void Set<T>::ListedRBTree::ic3() {
    ListedRBTree *p, *g, *u;

    p = this->parent;
    g = p->parent;

    if (p == g->left) {
        u = g->right;
    } else {
        u = g->left;
    }

    if (u->color == RED) {
        g->color = RED;
        p->color = BLACK;
        u->color = BLACK;

        g->ic1();
    } else {
        this->ic4();
    }
}

template<typename T>
void Set<T>::ListedRBTree::ic4() {
    ListedRBTree *n, *p, *g;

    n = this;
    p = n->parent;
    g = p->parent;

    if (p == g->left && this == p->right) {
        p->rotate_left();
        n = p;
    } else if (p == g->right && this == p->left) {
        p->rotate_right();
        n = p;
    }

    n->ic5();
}

template<typename T>
void Set<T>::ListedRBTree::ic5() {
    ListedRBTree *p, *g;

    p = this->parent;
    g = p->parent;

    p->color = BLACK;
    g->color = RED;

    if (p == g->left) {
        g->rotate_right();
    } else {
        g->rotate_left();
    }
}

template<typename T>
void Set<T>::ListedRBTree::erase() {
    this->s->_size--;

    ListedRBTree *rep;

    if (!this->right->is_leaf()) {
        rep = this->next;
    } else if (!this->left->is_leaf()) {
        rep = this->prev;
    } else {
        rep = this->left;
    }

    this->replace(rep);
}

template<typename T>
void Set<T>::ListedRBTree::replace(ListedRBTree *rep) {
    ListedRBTree *n;

    if (rep->is_leaf()) {
        this->prev->next = this->next;
        this->next->prev = this->prev;

        n = rep;
        this->left = new ListedRBTree(this->s);

        n->parent = this->parent;
        if (this->parent == nullptr) {
            this->s->_root = n;
        } else if (this == this->parent->left) {
            this->parent->left = n;
        } else {
            this->parent->right = n;
        }

        if (this->color == BLACK) n->dc1();

        delete this;
    } else {
        rep->prev->next = rep->next;
        rep->next->prev = rep->prev;

        this->value = rep->value;

        if (!rep->left->is_leaf()) {
            n = rep->left;
        } else if (!rep->right->is_leaf()) {
            n = rep->right;
        } else {
            n = new ListedRBTree(this->s);
        }

        n->parent = rep->parent;
        if (rep == rep->parent->left) {
            rep->parent->left = n;
        } else {
            rep->parent->right = n;
        }

        if (rep->color == BLACK) {
            if (n->color == RED) {
                n->color = BLACK;
            } else {
                n->dc1();
            }
        }

        delete rep;
    }
}

template<typename T>
void Set<T>::ListedRBTree::dc1() {
    if (this->parent == nullptr) return;
    this->dc2();
}

template<typename T>
void Set<T>::ListedRBTree::dc2() {
    ListedRBTree *p = this->parent, *s;

    if (this == p->left) {
        s = p->right;
    } else {
        s = p->left;
    }

    if (s->color == RED) {
        p->color = RED;
        s->color = BLACK;

        if (this == p->left) {
            p->rotate_left();
        } else {
            p->rotate_right();
        }
    }
    this->dc3();
}

template<typename T>
void Set<T>::ListedRBTree::dc3() {
    ListedRBTree *p = this->parent, *s;

    if (this == p->left) {
        s = p->right;
    } else {
        s = p->left;
    }

    if (
        p->color == BLACK &&
        s->color == BLACK &&
        s->left->color == BLACK &&
        s->right->color == BLACK
    ) {
            s->color = RED;
            this->dc1();
    } else {
        this->dc4();
    }
}

template<typename T>
void Set<T>::ListedRBTree::dc4() {
    ListedRBTree *p = this->parent, *s;

    if (this == p->left) {
        s = p->right;
    } else {
        s = p->left;
    }

    if (
        p->color == RED &&
        s->color == BLACK &&
        s->left->color == BLACK &&
        s->right->color == BLACK
    ) {
        s->color = RED;
        p->color = BLACK;
    } else {
        this->dc5();
    }
}

template<typename T>
void Set<T>::ListedRBTree::dc5() {
    ListedRBTree *p = this->parent, *s;

    if (this == p->left) {
        s = p->right;
    } else {
        s = p->left;
    }

    if (s->color == BLACK) {
        if (
            this == p->left &&
            s->right->color == BLACK &&
            s->left->color == RED
        ) {
            s->color = RED;
            s->left->color = BLACK;
            s->rotate_right();
        } else if (
            this == p->right &&
            s->left->color == BLACK &&
            s->right->color == RED
        ) {
            s->color = RED;
            s->right->color = BLACK;
            s->rotate_left();
        }
    }

    this->dc6();
}

template<typename T>
void Set<T>::ListedRBTree::dc6() {
    ListedRBTree *p = this->parent, *s;

    if (this == p->left) {
        s = p->right;
    } else {
        s = p->left;
    }

    s->color = p->color;
    p->color = BLACK;

    if (this == p->left) {
        s->right->color = BLACK;
        p->rotate_left();
    } else {
        s->left->color = BLACK;
        p->rotate_right();
    }
}

template<typename T>
bool Set<T>::ListedRBTree::is_leaf() { return this->left == this->right; }
