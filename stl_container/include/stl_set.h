#pragma once

#include <functional>
#include <iterator>

template<typename Key, typename Compare = std::less<Key>, typename Allocator = std::allocator<Key>>
class Set {
  private:
    class RBTree {
        RBTree *parent, left, right;
        Key *value;

        Key* min();
        Key* max();
    };

    RBTree *data;

  public:
    Set() = default;

    template<typename InputIt>
    Set(InputIt begin, InputIt end) = default;

    Set(std::initializer_list<Key> init) = default;
    Set(const Set &other) = default;
    Set& operator=(const Set &other) = default;
    ~Set() = default;

    std::iterator<std::bidirectional_iterator_tag, Key> cbegin() const;
    std::iterator<std::bidirectional_iterator_tag, Key> cend() const;
};

#include "impl.cpp"