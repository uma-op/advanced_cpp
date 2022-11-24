#pragma once

#include <iterator>

template<typename T>
class Set {
 private:
    class ListedRBTree {
     public:
        ListedRBTree *parent, *left, *right, *prev, *next;
        T value;
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

    class iterator : std::iterator<std::bidirectional_iterator_tag, T> {
     public:
        virtual iterator operator++(int);
        virtual iterator operator++();
        virtual iterator operator--(int);
        virtual iterator operator--();

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

     protected:
        ListedRBTree *value;
    };

    iterator begin() const;
    iterator end() const;

    void insert(T elem);
    void erase(T elem);

    size_t size() const {
        return this->_size;
    }

    bool empty() const {
        return this->size() == 0;
    }

    iterator find(T elem) const;
    iterator lower_bound(T elem) const;

 private:
    class forward_iterator : public iterator {
        iterator operator++() {
            if (this->value != Set::_pivot) this->value = this->value->next;
            return *this
        }

        iterator operator--() {
            return backward_iterator()
        }
    };

    class backward_iterator : public iterator {};
};