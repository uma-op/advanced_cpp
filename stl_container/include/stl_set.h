#pragma once

#include <functional>
#include <iterator>

template<typename Key, typename Compare = std::less<Key>, typename Allocator = std::allocator<Key>>
class Set {
  private:
    struct ListedAVLTree {
        ListedAVLTree *left, *right, *prev, *next;
		int height;
        Key value;

		ListedAVLTree(Key _value) : value(_value);
    };

    ListedAVLTree *t;
	ListedAVLTree *begin, *end;

	ListedAVLTree* rotate_left(ListedAVLTree *n);
	ListedAVLTree* rotate_right(ListedAVLTree *n);
	ListedAVLTree* insert_(ListedAVLTree *t, ListedAVLTree *n);
	ListedAVLTree* erase_(ListedAVLTree *t, Key val);

	ListedAVLTree* balance(ListedAVLTree *n);
	int bfactor(ListedAVLTree *n);
	ListedAVLTree* rm(ListedAVLTree *n);
	ListedAVLTree* min(ListedAVLTree *n);
	void normalize(ListedAVLTree *n);

  public:

    class iterator : public std::iterator<std::bidirectional_iterator_tag, ListedAVLTree> {
		bool operator!=(iterator other);
		bool operator==(iterator other);

		Key operator* ();

		iterator operator++();
		iterator operator++(iterator);
		iterator operator--();
		iterator operator--(iterator);
	};

    Set();

    template<typename InputIt>
    Set(InputIt begin, InputIt end);

    Set(std::initializer_list<Key> init);
    Set(const Set &other);
    Set& operator=(const Set &other);
    ~Set();

    iterator begin() const;
    iterator end() const;
	iterator find(Key val) const;
	iterator lower_bound(Key val) const;
	
	void insert(Key val);
	void erase(Key val);
};

#include "impl.cpp"