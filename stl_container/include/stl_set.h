#pragma once

#include <functional>
#include <iterator>

template<typename Key, typename Compare = std::less<Key>, typename Allocator = std::allocator<Key>>
class Set {
  public:
    struct ListedAVLTree {
        ListedAVLTree *left, *right, *prev, *next;
		int height;
        Key value;

		ListedAVLTree(Key _value) : left(nullptr), right(nullptr), prev(nullptr), next(nullptr), height(1), value(_value) {};
    };

    ListedAVLTree *t;
	ListedAVLTree *begin, *end;
	Compare cmp;
	size_t lenght;

	ListedAVLTree* rotate_left(ListedAVLTree *n) {
		if (n->right == nullptr) return n;

		ListedAVLTree *p = n->right;
		n->right = p->left;
		p->left = n;

		this->normalize(n);
		this->normalize(p);

		return p;
	}

	ListedAVLTree* rotate_right(ListedAVLTree *n) {
		if (n->left == nullptr) return n;

		ListedAVLTree *p = n->left;
		n->left = p->right;
		p->right = n;

		this->normalize(n);
		this->normalize(p);
		
		return p;
	}
	ListedAVLTree* insert_(ListedAVLTree *t, ListedAVLTree *n, ListedAVLTree *b) {
		if (t == nullptr) {
			if (b == nullptr) {
				n->next = this->begin;
				this->begin = n;
				
				if (n->next != nullptr) n->next->prev = n;
			} else {
				n->prev = b;
				n->next = b->next;
				b->next = n;
				
				if (n->next != nullptr) n->next->prev = n;
			}

			if (n->next == nullptr) this->end = n;

			this->lenght++;

			return n;
		}

		if(this->cmp(n->value, t->value)) t->left = insert_(t->left, n, t);
		else t->right = insert_(t->right, n, b);

		return this->balance(t);
	}

	ListedAVLTree* erase_(ListedAVLTree *t, Key val) {
		if (t == nullptr) return nullptr;
		if (this->cmp(val, t->value)) {
			t->left = this->erase_(t->left, val);
		} else if (this->cmp(t->value, val)) {
			t->right = this->erase_(t->right, val);
		} else {
			if (t->prev != nullptr) t->prev->next = t->next;
			else {
				this->begin = t->next;
				this->begin->prev = nullptr;
			}

			if (t->next != nullptr) t->next->prev = t->prev;
			else {
				this->end = t->prev;
				this->end->next = nullptr;
			}

			this->lenght--;

			ListedAVLTree* q = t->left;
			ListedAVLTree* r = t->right;

			if ( r == nullptr ) return q;

			ListedAVLTree* m = this->min(r);
			m->right = this->rm(r);
			m->left = q;
			return this->balance(m);
		}

		return this->balance(t);
	}

	ListedAVLTree* balance(ListedAVLTree *n) {
		this->normalize(n);

		if(this->bfactor(n) == 2) {
			if(this->bfactor(n->right) < 0) n->right = this->rotate_right(n->right);
			return this->rotate_left(n);
		}
		if (this->bfactor(n) == -2) {
			if(this->bfactor(n->left) > 0) n->left = this->rotate_left(n->left);
			return this->rotate_right(n);
		}

		return n;
	}

	int bfactor(ListedAVLTree *n) {
		int lh, rh;

		if (n->left == nullptr) lh = 0;
		else lh = n->left->height;

		if (n->right == nullptr) rh = 0;
		else rh = n->right->height;

		return rh - lh;
	}

	ListedAVLTree* rm(ListedAVLTree *n) {
		if (n->left == nullptr) return n->right;
		n->left = this->rm(n->left);
		return this->balance(n);
	}

	ListedAVLTree* min(ListedAVLTree *n) {
		while (n->left) n = n->left;
    	return n;
	}

	void normalize(ListedAVLTree *n) {
		int lh, rh;

		if (n->left == nullptr) lh = 0;
		else lh = n->left->height;

		if (n->right == nullptr) rh = 0;
		else rh = n->right->height;

		n->height = (lh > rh ? lh : rh) + 1;
	}

	// public:

    class iterator : public std::iterator<std::bidirectional_iterator_tag, ListedAVLTree> {
	public:
		ListedAVLTree *value;

		iterator(ListedAVLTree *_value) : value(_value) {}

		bool operator!=(iterator other) {
			return this->value != other.value;
		}

		bool operator==(iterator other) {
			return this->value == other.value;
		}

		Key operator* () {
			return this->value->value;
		}

		iterator operator++(int) {
			this->value = value->next;
			return *this;
		}

		// iterator operator++(iterator) {};
		iterator operator--(int) {
			this->value = value->prev;
			return *this;
		}
		// iterator operator--(iterator);
	};

    Set() : t(nullptr), begin(nullptr), end(nullptr), cmp(), lenght(0) {}

    template<typename InputIt>
    Set(InputIt begin, InputIt end) : Set() {
		while (begin != end) {
			s.insert(*begin);
			begin++;
		}

		s.insert(*end);
	}

    Set(std::initializer_list<Key> init) : Set() {
		for (size_t i = 0; i < init.size(); i++) {
			this->insert(init[i]);
		}
	}

    Set(const Set &other) : Set(), Set(other.cbegin(), other.cend()) {}

    Set& operator=(const Set &other) {
		auto b = this->cbegin();
		auto d = b.value;
		while (b.value != nullptr) {
			d = b.value;
			b++;
			delete d;
		}

		this->t = nullptr;
		this->begin = nullptr;
		this->end = nullptr;
		this->lenght = 0;

		b = other.begin();
		while (b.value != nullptr) {
			this->insert(*b);
		}

		return this;
	}

    ~Set();

    iterator cbegin() const {
		return iterator(this->begin);
	}

    iterator cend() const {
		return iterator(this->end);
	}

	iterator find(Key val) const {
		iterator f(nullptr);
		ListedAVLTree *cur = this->t;

		while (cur != nullptr) {
			if (!(this->cmp(cur->value, val) || this->cmp(val, cur->value))) {
				f.value = cur;
				break;
			} else if (this->cmp(cur->value, val)) cur = cur->right;
			else cur = cur->left;
		} 

		return f;
	}

	iterator lower_bound(Key val) const {
		iterator lb(nullptr);
		ListedAVLTree *cur = this->t;

		while (cur != nullptr) {
			if (this->cmp(cur->value, val)) cur = cur->right;
			else {
				lb.value = cur;
				cur = cur->left;
			}
		}

		return lb;
	}
	
	void insert(Key val) {
		ListedAVLTree *n = new ListedAVLTree(val);
    	this->t = this->insert_(this->t, n, nullptr);
	}

	void erase(Key val) {
		this->erase_(this->t, val);
	}

	size_t size() {
		return this->lenght;
	}

	bool empty() {
		return this->lenght == 0;
	}
};

template<typename Key, typename Compare, typename Allocator>
Set<Key, Compare, Allocator>::~Set() {
	auto b = this->cbegin();
	auto d = b.value;
	while (b.value != nullptr) {
		d = b.value;
		b++;
		delete d;
	}
}