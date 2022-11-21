#pragma once

#include "stl_set.h"

template<typename Key, typename Compare, typename Allocator>
Set<Key, Compare, Allocator>::iterator Set<Key, Compare, Allocator>::begin() const {
    return Set<Key, Compare, Allocator>::iterator(this->begin);
}

template<typename Key, typename Compare, typename Allocator>
Set<Key, Compare, Allocator>::iterator Set<Key, Compare, Allocator>::end() const {
    return Set<Key, Compare, Allocator>::iterator(this->end);
}

template<typename Key, typename Compare, typename Allocator>
Set<Key, Compare, Allocator>::ListedAVLTree* Set<Key, Compare, Allocator>::rotate_left(ListedAVLTree *n) {
    if (n->right == nullptr) return;

    ListedAVLTree *p = n->right;
    n->right = p->left;
    p->left = n;

    this->normalize(n);
    this->normalize(p);

    return p;
};

template<typename Key, typename Compare, typename Allocator>
Set<Key, Compare, Allocator>::ListedAVLTree* Set<Key, Compare, Allocator>::rotate_right(ListedAVLTree *n) {
    if (n->left == nullptr) return;

    ListedAVLTree *p = n->left;
    n->left = p->right;
    p->right = n;

    this->normalize(n);
    this->normalize(p);
    
    return p;
};

template<typename Key, typename Compare, typename Allocator>
Set<Key, Compare, Allocator>::ListedAVLTree* Set<Key, Compare, Allocator>::insert_(ListedAVLTree *t, ListedAVLTree *n) {
    if (t == nullptr) return n;

	if(Compare(n->value, t->value)) t->left = insert(t->left, n);
	else t->right = insert(t->right, n);

	return balance(p);
}

template<typename Key, typename Compare, typename Allocator>
Set<Key, Compare, Allocator>::ListedAVLTree* Set<Key, Compare, Allocator>::erase_(ListedAVLTree *t, Key val) {
    if (t == nullptr) return nullptr;
    if (Compare(val, t->value)) {
        t->left = this->erase_(t->left, val);
    } else if (Compare(t->value, val)) {
        t->right = this->erase_(t->right, val);
    } else {
        ListedAVLTree* q = t->left;
		ListedAVLTree* r = t->right;

		if ( r == nullptr ) return q;

		ListedAVLTree* m = this->min(r);
		m->right = this->rm(r);
		m->left = q;
		return balance(min);
    }

    return basename(t);
}

template<typename Key, typename Compare, typename Allocator>
Set<Key, Compare, Allocator>::ListedAVLTree* Set<Key, Compare, Allocator>::balance(ListedAVLTree *n) {
    this->normalize(p);

	if(this->bfactor(p) == 2) {
		if(this->bfactor(p->right) < 0) p->right = this->rotate_right(p->right);
		return this->rotate_left(p);
	}
    if (this->bfactor(p) == -2) {
		if(this->bfactor(p->left) > 0) p->left = this->rotate_left(p->left);
		return this->rotate_right(p);
	}

	return p;
}

template<typename Key, typename Compare, typename Allocator>
int Set<Key, Compare, Allocator>::bfactor(ListedAVLTree *n) {
    int lh, rh;

    if (n->left == nullptr) lh = 0;
    else lh = n->left->height;

    if (n->right == nullptr) rh = 0;
    else rh = n->right->height;

    return rh - lh;
}

template<typename Key, typename Compare, typename Allocator>
Set<Key, Compare, Allocator>::ListedAVLTree* Set<Key, Compare, Allocator>::rm(ListedAVLTree *n) {
    if (n->left == nullptr) return n->right;
    n->left = this->rm(n->left);
    return balance(n);
}

template<typename Key, typename Compare, typename Allocator>
Set<Key, Compare, Allocator>::ListedAVLTree* Set<Key, Compare, Allocator>::min(ListedAVLTree *n) {
    while (n->left) n = n->left;
    return n;
}

template<typename Key, typename Compare, typename Allocator>
void Set<Key, Compare, Allocator>::normalize(ListedAVLTree *n) {
    int lh, rh;

    if (n->left == nullptr) lh = 0;
    else lh = n->left->height;

    if (n->right == nullptr) rh = 0;
    else rh = n->right->height;

    n->height = (lh > rh ? lh : rh) + 1;
}

template<typename Key, typename Compare, typename Allocator>
void Set<Key, Compare, Allocator>::insert(Key val) {
    ListedAVLTree *n = new ListedAVLTree(val);
    this->insert_(this->data, n);
}

template<typename Key, typename Compare, typename Allocator>
void Set<Key, Compare, Allocator>::erase(Key val) {
    this->erase_(this->data, val);
}