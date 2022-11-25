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

        void erase();

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

        const T& operator*() const {
            return this->value->value;
        }

        T* operator->() const {
            return &(this->value->value);
        }

        bool is_dummy() const {
            return this->value == this->value->s->_pivot;
        }

     private:
        ListedRBTree *value;
        bool is_forward;
    };

    iterator begin() const {
        return iterator{this->_pivot->next, true};
    }

    iterator end() const {
        return iterator{this->_pivot, true};
    }

    void insert(T elem) {
        ListedRBTree *n = new ListedRBTree(this, elem);
        this->_root->insert(n, this->_pivot);
    }

    void erase(T elem) {}

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

    void print() {
        auto b = this->begin();
        auto e = this->end();

        while (b != e) {
            std::cout << *b << ' ';
            b++;
        }
        std::cout << std::endl;
    }
};