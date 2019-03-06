#ifndef HASH_MAP_H
#define HASH_MAP_H
#if defined(__has_include)
#if __has_include(<typeinfo>)
#include <typeinfo>
#elif __has_include(<typeinfo.h>)
#include <typeinfo.h>
#endif
#if __has_include(<iterator>)
#include <iterator>
#endif
#if __has_include(<functional>)
#include <functional>
#endif
#endif
#define _max_bucket_count 10000
#define _max_load_factor 0.75
template <class key, class Value> class HASH_MAP
{
	int _bucket_count = 100;
	int array_size = 0;

	struct HASH_NODE {
	public:
		key first = key();
		Value second = Value();
		HASH_NODE() {
		}
		HASH_NODE(key k, Value v) {
			first = k;
			second = v;
		}
		//reset node to null values, deallocating increased values 
		~HASH_NODE() {
			first = key();
			second = Value();
		}
		HASH_NODE operator=(const HASH_NODE& H) {
			first = H.first;
			second = H.second;
		}
	};
	HASH_NODE* buckets;
	const std::type_info& key_type = typeid(key);
	const std::type_info& mapped_type = typeid(Value);
	//const std::type_info& value_type = std::pair < key_type, mapped_type> ;
public:
	HASH_MAP<key, Value>() {
		buckets = new HASH_NODE[_bucket_count];
	};
	~HASH_MAP<key, Value>() {
		delete[] buckets;
	};
	HASH_MAP<key, Value>(const HASH_MAP<key, Value>&H) {
		_bucket_count = H._bucket_count;
		array_size = H.array_size;
		HASH_NODE* temp = new HASH_NODE[H._bucket_count];
		auto i = H.begin(), t_start = &temp[0], t = t_start;
		for (int j = 0; i < end() && j < H._bucket_count; j++, i++) {
			if (i->first != key()) {
				t = t_start + getStart(i->first);
				t->first = i->first;
				t->second = i->second;
			}
		}
		swap(temp, buckets); // swap addresses
		delete temp;
	};
	HASH_MAP<key, Value> operator=(const HASH_MAP<key, Value>& H) {
		_bucket_count = H._bucket_count;
		array_size = H.array_size;
		HASH_NODE* temp = new HASH_NODE[H._bucket_count];
		auto i = H.begin(), t_start = &temp[0], t = t_start;
		for (int j = 0; i < end() && j < H._bucket_count; j++, i++) {
			if (i->first != key()) {
				t = t_start + getStart(i->first);
				t->first = i->first;
				t->second = i->second;
			}
		}
		swap(temp, buckets); // swap addresses
		delete temp;
	};

	// Size Operators, NoExcep
	inline int size() const {
		return array_size;
	};
	//inline int max_size() const; // UNNECSSARY FOR NEMOSTATICLIB

	// Access Functions
	Value& operator[](const key& k) const {
		return (begin() + getStart(k))->second;
	};
	Value& operator[](key&& k) const {
		return (begin() + getStart(k))->second;
	};
	Value& at(const key& k) const {
		return (begin() + getStart(k))->second;
	};
	Value& at(key&& k) const {
		return (begin() + getStart(k))->second;
	};
	//iterator find(const key&) const; // UNNECESSARY FOR NEMOSTATICLIB
	// Graph.cpp - getOrCreateIndex()
	//
	// Start from the expected value that we would find a value with key at
	// go forward until we find it or we find an empty cell with the default key
	int count(const key& k) const{
		int found = 0;
		int start = getStart(k);
		HASH_NODE* ptr = begin() + start;
		for (ptr; ptr < end(); ptr++) {
			if (ptr->first == key()) {
				return found;
			}
			if (ptr->first == k) {
				found++;
			}
		}
		return found;
	};
	//pair<int, int> equal_range(const key&) const; UNNECESSARY FOR NEMOSTATICLIB

	//Iterator Functions
	inline auto begin() const {
		return &buckets[0];
	};
	inline auto end() const {
		return &buckets[_bucket_count];
	};
	//const iterator cbegin() const
	//const iterator cend() const

	//Modifiers
	//used in SubgraphCount.cpp as type 6
	void insert(std::pair<key, Value> p) {
		if (count(p.first))
			return;
		array_size++;
		int start = getStart(p.first);
		auto ptr = begin() + start;
		for (ptr; ptr < end(); ptr++) {
			if (ptr->first == key()) {
				ptr->first = p.first;
				ptr->second = p.second;
				break;
			}
		}
		if (load_factor() > max_load_factor()) {
			rehash();
		}
	};
	//emplace
	//emplace_hint
	//erase - only used in std:vector
	//clear
	//swap

	//Buckets
	inline size_t bucket_count() const {
		return _bucket_count;
	};
	inline size_t max_bucket_count() const {
		return _max_bucket_count;
	};
	//size_type bucket_size(size_type n) const;
	//size_type bucket(const key& k) const; // return bucket with key k

	//Hash Policy
	inline double load_factor() const noexcept {
		return size() / (double)bucket_count();
	};
	inline float max_load_factor() const noexcept {
		return _max_load_factor;
	};
	// reserve - unused in NemoStaticLib
	void rehash() {
		HASH_NODE* temp = new HASH_NODE[_bucket_count *= 2];
		auto i = begin(), t_start = &temp[0], t = t_start;
		for (int j = 0; i < end() && j < _bucket_count/2; j++, i++) {
			if (i->first != key()) {
				t = t_start + getStart(i->first);
				t->first = i->first;
				t->second = i->second;
			}
		}
		swap(temp, buckets); // swap addresses
		delete[] temp;
	};

	//Observers - Unused
	//hasher hash_function() const;
	//key_equal key_eq() const;
	//allocator_type get_allocator() const noexcept;
	inline int getStart(const key& k) const noexcept {
		return std::hash<key>{}(k) % _bucket_count;
	};
	void swap(HASH_NODE* h1, HASH_NODE* h2) {
		HASH_NODE* temp = h1;
		h1 = h2;
		h2 = temp;
	};
};


#endif