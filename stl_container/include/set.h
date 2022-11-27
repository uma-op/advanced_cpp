#pragma once

#include <iterator>
#include <iostream>

template<typename T>
class Set {
 private:
    class ListedRBTree {
        void rotate_left();
        void rotate_right();

        void ic1();
        void ic2();
        void ic3();
        void ic4();
        void ic5();

        void dc1();
        void dc2();
        void dc3();
        void dc4();
        void dc5();
        void dc6();

        void replace(ListedRBTree *rep);

     public:
        enum Color { RED, BLACK };

        ListedRBTree *parent, *left, *right, *prev, *next;
        Color color;

        Set *s;
        T value;

        ListedRBTree(Set *_s);
        ListedRBTree(Set *_s, T _value);
        ~ListedRBTree();

        void insert(ListedRBTree *other, ListedRBTree *before);
        void erase();

        bool is_leaf();
    };

    ListedRBTree *_root;
    ListedRBTree *_pivot;

    size_t _size;

 public:
    Set();

    template<typename InputIt>
    Set(InputIt begin, InputIt end);

    Set(std::initializer_list<T> data);
    Set(const Set& other);

    ~Set();

    Set& operator=(const Set& other);

    class iterator {
     public:
        iterator() = default;
        iterator(ListedRBTree *_value, bool _is_forward);

        iterator operator++(int);
        iterator& operator++();
        iterator operator--(int);
        iterator& operator--();

        bool operator==(const iterator &other) const;
        bool operator!=(const iterator &other) const;

        const T& operator*() const;
        const T* operator->() const;

     protected:
        ListedRBTree *value;
        bool is_forward;
    };

    using const_iterator = iterator;

    iterator begin() const;
    iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    void insert(T elem);
    void erase(T elem);

    size_t size() const;

    bool empty() const;

    iterator find(T elem) const;
    iterator lower_bound(T elem) const;
};

#include "rb_tree.cpp"
#include "constructors.cpp"
#include "iterator.cpp"
#include "access.cpp"
#include "managment.cpp"
