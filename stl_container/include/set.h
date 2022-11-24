#pragma once

#include <iterator>

template<typename T>
class Set {
 private:
    class ListedRBTree {
     public:
        enum Color { RED, BLACK };

        ListedRBTree *parent, *left, *right, *prev, *next;
        Color color;

        T value;

        ListedRBTree() : parent(nullptr), left(nullptr), right(nullptr), prev(nullptr), next(nullptr), color(Color::BLACK) {}
        ListedRBTree(T value) : parent(nullptr), left(nullptr), right(nullptr), prev(nullptr), next(nullptr), color(Color::RED), value(value) {}
        
        void rotate_left() {
            if (this->right.is_leaf()) return;

            ListedRBTree *p = this->right;
            p->parent = this->parent;

            if (p->parent == nullptr) {
                Set::_root = p;
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
            if (this->left.is_leaf()) return;

            ListedRBTree *p = this->left;

            if (this->parent == nullptr) {
                Set::_root = p;
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
                ListedRBTree *after = before->next;

                before->next = other;
                after->prev = other;

                other->prev = before;
                other->next = after;

                if (this->parent == nullptr) {
                    Set::_root = other;
                } else if (this == this->parent->left) {
                    this->parent->left = other;
                } else {
                    this->parent->right = other;
                }

                other->parent = this->parent;

                other->left = new ListedRBTree();
                other->left->parent = other;

                other->right = new ListedRBTree();
                other->right->parent = other;

                delete this;

                other->ic1();

                return
            }

            if (other->value < this->value) {
                this->left->insert(other, before);
            } else {
                this->right->insert(other, this);
            }
        }

        void ic1() {
            if (this->parent == nullptr) this->color = Color::BLACK;
            else this->ic2();
        }

        void ic2() {
            if (this->parent->color == Color::BLACK) return
            this->ic3();
        }

        void ic3() {
            ListedRBTree *p, *g, *u;

            p = this->parent;
            g = p->parent;

            if (p == g->left) u = g->right;
            else u = g->left;

            if (!u->is_leaf() && u->color == Color::RED) {
                g->color = Color::RED;
                p->color = Color::BLACK;
                u->color = Color::BLACK;

                this->ic1();
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

            p->color = Color::BLACK;
            g->color = Color::RED;

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
    Set() : _root(new ListedRBTree()), _pivot(new ListedRBTree()) {
        this->_pivot->next = this->_pivot;
        this->_pivot->prev = this->_pivot;
    }

    template<typename InputIt>
    Set(InputIt begin, InputIt end) : Set() {
        for (;begin != end; begin++) this->insert(*begin);
    }

    Set(std::initializer_list<T> data);
    Set(const Set& other);
    ~Set();

    Set& operator=(const Set& other);

    class iterator : std::iterator<std::bidirectional_iterator_tag, T> {
     public:
        iterator operator++(int) {
            iterator it = *this;
            ++(*this);
            return it;
        }

        iterator& operator++() {
            if (this->value != Set::_pivot || !this->is_forward) this->value = this->value->next;
            this->is_forward = true;
            return *this;
        }

        iterator operator--(int) {
            iterator it = *this;
            --(*this);
            return it;
        }

        iterator& operator--() {
            if (this->value != Set::_pivot || this->is_forward) this->value = this->value->prev;
            this->is_forward = false;
            return *this;
        }

        bool operator==(const iterator &other) {
            return *(*this) == *other;
        }

        bool operator!=(const iterator &other) {
            return *(*this) != *other;
        }

        const T& operator*() {
            return this->value->value;
        }

        T* operator->() {
            return &(this->value->value);
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
        ListedRBTree *n = ListedRBTree(elem);
        this->_root->insert(n, this->_pivot);
    }

    void erase(T elem);

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
            else if (cur->value < elem) = cur->right;
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