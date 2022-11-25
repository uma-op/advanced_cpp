#pragma once

#include <iterator>
#include <iostream>

template<typename T>
class Set {
 private:
    class ListedRBTree {
     public:
        enum Color { RED, BLACK };

        ListedRBTree *parent, *left, *right, *prev, *next;
        Color color;

        Set *s;
        T value;

        ListedRBTree(Set *_s) :
            parent(nullptr),
            left(nullptr),
            right(nullptr),
            prev(nullptr),
            next(nullptr),
            color(BLACK),
            s(_s) {}

        ListedRBTree(Set *_s, T _value) :
            parent(nullptr),
            left(nullptr),
            right(nullptr),
            prev(nullptr),
            next(nullptr),
            color(RED),
            s(_s),
            value(_value) {}

        ~ListedRBTree() {
            if (this->is_leaf()) return;
            if (this->right->is_leaf()) delete this->right;
            if (this->left->is_leaf()) delete this->left;
        }
        
        void rotate_left() {
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

        void rotate_right() {
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

        void insert(ListedRBTree *other, ListedRBTree *before) {
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

        void ic1() {
            if (this->parent == nullptr) this->color = BLACK;
            else this->ic2();
        }

        void ic2() {
            if (this->parent->color == BLACK) return;
            this->ic3();
        }

        void ic3() {
            ListedRBTree *p, *g, *u;

            p = this->parent;
            g = p->parent;

            if (p == g->left) u = g->right;
            else u = g->left;

            if (u->color == RED) {
                g->color = RED;
                p->color = BLACK;
                u->color = BLACK;

                g->ic1();
            } else {
                this->ic4();
            }
        }

        void ic4() {
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

        void ic5() {
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

        void erase() {
            this->s->_size--;

            ListedRBTree *rep;

            if (!this->right->is_leaf()) rep = this->next;
            else if (!this->left->is_leaf()) rep = this->prev;
            else rep = this->left;

            this->replace(rep);
        }

        void replace(ListedRBTree *rep) {
            ListedRBTree *n;

            if (rep->is_leaf()) {
                this->prev->next = this->next;
                this->next->prev = this->prev;

                n = rep;
                this->left = new ListedRBTree(this->s);

                n->parent = this->parent;
                if (this->parent == nullptr) this->s->_root = n;
                else if (this == this->parent->left) this->parent->left = n;
                else this->parent->right = n;

                if (this->color == BLACK) n->dc1();

                delete this;
            } else {
                rep->prev->next = rep->next;
                rep->next->prev = rep->prev;

                this->value = rep->value;
                
                if (!rep->left->is_leaf()) n = rep->left;
                else if (!rep->right->is_leaf()) n = rep->right;
                else n = new ListedRBTree(this->s);

                n->parent = rep->parent;
                if (rep == rep->parent->left) rep->parent->left = n;
                else rep->parent->right = n;

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

        void dc1() {
            if (this->parent == nullptr) return;
            this->dc2();
        }

        void dc2() {
            ListedRBTree *p = this->parent, *s;

            if (this == p->left) s = p->right;
            else s = p->left;

            if (s->color == RED) {
                p->color = RED;
                s->color = BLACK;

                if (this == p->left) p->rotate_left();
                else p->rotate_right();
            }
            this->dc3();
        }

        void dc3() {
            ListedRBTree *p = this->parent, *s;

            if (this == p->left) s = p->right;
            else s = p->left;

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

        void dc4() {
            ListedRBTree *p = this->parent, *s;

            if (this == p->left) s = p->right;
            else s = p->left;

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

        void dc5() {
            ListedRBTree *p = this->parent, *s;

            if (this == p->left) s = p->right;
            else s = p->left;

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

        void dc6() {
            ListedRBTree *p = this->parent, *s;

            if (this == p->left) s = p->right;
            else s = p->left;

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

        bool is_leaf() { return this->left == this->right; }
    };

    ListedRBTree *_root;
    ListedRBTree *_pivot;

    size_t _size;

 public:
    Set() : _root(new ListedRBTree(this)), _pivot(new ListedRBTree(this)), _size(0) {
        this->_pivot->next = this->_pivot;
        this->_pivot->prev = this->_pivot;
    }

    template<typename InputIt>
    Set(InputIt begin, InputIt end) : Set() {
        for (;begin != end; begin++) this->insert(*begin);
    }

    Set(std::initializer_list<T> data) : Set() {
        for (T elem : data) {
            this->insert(elem);
        }
    }

    Set(const Set& other) : Set() {
        iterator b = other.begin();
        iterator e = other.end();

        while (b != e) {
            this->insert(*b);
            b++;
        }
    }

    ~Set() {
        ListedRBTree *b = this->_pivot->next, *t;
        ListedRBTree *e = this->_pivot;
        
        while (b != e) {
            t = b->next;
            delete b;
            b = t;
        }

        delete this->_pivot;
    }

    Set& operator=(const Set& other) {
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

    class iterator : std::iterator<std::bidirectional_iterator_tag, T> {
     public:
        iterator() = default;
        iterator(ListedRBTree *_value, bool _is_forward) : value(_value), is_forward(_is_forward) {}

        iterator operator++(int) {
            iterator it = *this;
            ++(*this);
            return it;
        }

        iterator& operator++() {
            if (this->value != this->value->s->_pivot || !this->is_forward) this->value = this->value->next;
            this->is_forward = true;
            return *this;
        }

        iterator operator--(int) {
            iterator it = *this;
            --(*this);
            return it;
        }

        iterator& operator--() {
            if (this->value != this->value->s->_pivot || this->is_forward) this->value = this->value->prev;
            this->is_forward = false;
            return *this;
        }

        bool operator==(const iterator &other) const {
            return this->value == other.value;
        }

        bool operator!=(const iterator &other) const {
            return !(*this == other);
        }

        T& operator*() {
            return this->value->value;
        }

        T* operator->() {
            return &(this->value->value);
        }

     protected:
        ListedRBTree *value;
        bool is_forward;
    };

    class const_iterator : public iterator {
     public:
        const_iterator() : iterator::iterator() {}
        const_iterator(ListedRBTree *_value, bool _is_forward) : iterator::iterator(_value, _is_forward) {}

        const T& operator*() const {
            return this->value->value;
        }

        const T* operator->() const {
            return &(this->value->value);
        }
    };

    iterator begin() const {
        return iterator{this->_pivot->next, true};
    }

    iterator end() const {
        return iterator{this->_pivot, true};
    }

    const_iterator cbegin() const {
        return const_iterator{this->_pivot->next, true};
    }

    const_iterator cend() const {
        return const_iterator{this->_pivot, true};
    }

    void insert(T elem) {
        ListedRBTree *n = new ListedRBTree(this, elem);
        this->_root->insert(n, this->_pivot);
    }

    void erase(T elem) {
        if (this->empty()) return;

        ListedRBTree *cur = this->_root;

        while (!cur->is_leaf()) {
            if (elem < cur->value) cur = cur->left;
            else if (cur->value < elem) cur = cur->right;
            else {
                cur->erase();
                break;
            }
        }
    }

    size_t size() const {
        return this->_size;
    }

    bool empty() const {
        return this->size() == 0;
    }

    iterator find(T elem) const {
        ListedRBTree *cur = this->_root;

        while (!cur->is_leaf()) {
            if (elem < cur->value) cur = cur->left;
            else if (cur->value < elem) cur = cur->right;
            else return iterator{cur, true};
        }

        return this->end();
    }

    iterator lower_bound(T elem) const {
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
};