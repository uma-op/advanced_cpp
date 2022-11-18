#include "stl_set.h"

template<typename Key, typename Compare, typename Allocator>
std::iterator<std::bidirectional_iterator_tag, Key> Set<Key, Compare, Allocator>::cbegin() const {
    return this->data->min();
}

template<typename Key, typename Compare, typename Allocator>
std::iterator<std::bidirectional_iterator_tag, Key> Set<Key, Compare, Allocator>::cend() const {
    return this->data->max();
}