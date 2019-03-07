#ifndef HASH_MAP_H
#define HASH_MAP_H

#if defined(__has_include)
#if __has_include(<functional>)
#include <functional>
#endif
#endif
#define _max_bucket_count 1000000
#define _max_load_factor 0.75
#include <iostream>
template <class key, class Value> class HASH_MAP
{
	int _bucket_count = 10;
	int array_size = 0;
public:
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
			return *this;
		}
	};
private:
	HASH_NODE* buckets;
public:
	HASH_MAP<key, Value>() {
		buckets = new HASH_NODE[_bucket_count + 1];
	};
	~HASH_MAP<key, Value>() {
		delete[] buckets;
	};
	HASH_MAP<key, Value>(const HASH_MAP<key, Value>&H) {
		_bucket_count = H._bucket_count;
		array_size = H.array_size;
		HASH_NODE* temp = new HASH_NODE[H._bucket_count + 1];
		auto i = H.begin(), t_start = &temp[0], t = t_start;
		for (int j = 0; i < end() && j < H._bucket_count; j++, i++) {
			if (i->first != key()) {
				t = t_start + getStart(i->first);
				t->first = i->first;
				t->second = i->second;
			}
		}
		swap(temp, buckets); // swap addresses
		delete[] temp;
	};
	HASH_MAP<key, Value> operator=(const HASH_MAP<key, Value>& H) {
		_bucket_count = H._bucket_count;
		array_size = H.array_size;
		HASH_NODE* temp = new HASH_NODE[H._bucket_count + 1];
		auto i = H.begin(), t_start = &temp[0], t = t_start;
		for (int j = 0; i < end() && j < H._bucket_count; j++, i++) {
			if (i->first != key()) {
				t = t_start + getStart(i->first);
				t->first = i->first;
				t->second = i->second;
			}
		}
		swap(temp, buckets); // swap addresses
		delete[] temp;
	};

	// Size Operators, NoExcep
	inline int size() const noexcept {
		return array_size;
	};
	//inline int max_size() const; // UNNECSSARY FOR NEMOSTATICLIB

	// Access Functions
	Value& operator[](const key& k) const noexcept {
		for(auto ptr = begin() + getStart(k); ptr < end(); ptr++)
			if(ptr->first == k)
				return ptr->second;
		Value v = Value();
		Value& vp = v;
		return v;
	};
	Value& operator[](key&& k) const noexcept {
		for (auto ptr = begin() + getStart(k); ptr < end(); ptr++)
			if (ptr->first == k)
				return ptr->second;
		Value v = Value();
		Value& vp = v;
		return v;
	};
	Value& at(const key& k) const noexcept {
		for (auto ptr = begin() + getStart(k); ptr < end(); ptr++)
			if (ptr->first == k)
				return ptr->second;
		Value v = Value();
		Value& vp = v;
		return v;
	};
	Value& at(key&& k) const noexcept {
		Value v = Value();
		for (auto ptr = begin() + getStart(k); ptr < end(); ptr++)
			if (ptr->first == k)
				return ptr->second;
		Value v = Value();
		Value& vp = v;
		return v;
	};
	//iterator find(const key&) const; // UNNECESSARY FOR NEMOSTATICLIB
	// Graph.cpp - getOrCreateIndex()
	//
	// Start from the expected value that we would find a value with key at
	// go forward until we find it or we find an empty cell with the default key
	int count(const key& k) const noexcept {
		int found = 0;
		int start = getStart(k);
		auto ptr = begin() + start;
		count_loop:
		for (ptr; ptr < end(); ptr++) {
			if (ptr->first == key()) {
				return found;
			}
			else if (ptr->first == k) {
				found++;
			}
			if (ptr == end()) {
				ptr = begin();
				goto count_loop;
			}
		}
		return found;
	};

	//Iterator Functions
	inline auto begin() const noexcept {
		return &buckets[0];
	};
	inline auto end() const noexcept {
		return &buckets[_bucket_count];
	};
	const inline auto cbegin() const noexcept {
		return &buckets[0];
	};
	const inline auto cend() const noexcept {
		return &buckets[_bucket_count];
	};

	//Modifiers
	//used in SubgraphCount.cpp as type 6
	void insert(std::pair<key, Value> p) noexcept {
		if (count(p.first))
			return;
		array_size++;
		int start = getStart(p.first);
		
		auto ptr = begin() + start;
		insert_loop:
		for (ptr; ptr < end(); ptr++) {
			// if ptr reaches the end, go to the beginning
			if (ptr->first == key()) {
				ptr->first = p.first;
				ptr->second = p.second;
				break;
			}
		}
		if (ptr == end()) {
			ptr = begin();
			goto insert_loop;
		}
		// if the load factor is too high
		if (load_factor() > max_load_factor()) {
			rehash();
		}
		int z = 0;
	};

	//Buckets
	inline size_t bucket_count() const noexcept {
		return _bucket_count;
	};
	inline size_t max_bucket_count() const noexcept {
		return _max_bucket_count;
	};

	//Hash Policy
	inline double load_factor() const noexcept {
		return size() / (double)bucket_count();
	};
	inline float max_load_factor() const noexcept {
		return _max_load_factor;
	};
	void rehash() noexcept {
		if (_bucket_count * 2 < _max_bucket_count) {
			_bucket_count *= 2;
		}
		else if (_bucket_count < _max_bucket_count) {
			_bucket_count = _max_bucket_count;
		}
		else {
			std::cerr << "<<<<ERROR: MEMORY OVERFLOW, OVER 1,000,000 ELEMENTS INSERTED>>>>" << std::endl;
		}
		HASH_NODE* temp = new HASH_NODE[_bucket_count + 1];
		auto t_start = &temp[0], t_end = &temp[_bucket_count];

		auto END = begin() + (_bucket_count / 2);
		for (auto i = begin(); i < END; i++) {
			if (i->first != key()) {
				auto ptr = t_start + getStart(i->first);
				rehash_loop:
				for (ptr; ptr < t_end; ptr++) {
					if (ptr->first == key()) {
						ptr->first = i->first;
						ptr->second = i->second;
						break;
					}
				}
				if (ptr == t_end) {
					ptr = t_start;
					goto rehash_loop;
				}
			}
		}
		std::swap(temp, buckets);
		delete[] temp;
	};

	inline int getStart(const key& k) const noexcept {
		return std::hash<key>{}(k) % _bucket_count;
	};
	bool ptr_swap(HASH_NODE** h1, HASH_NODE** h2) noexcept {
		HASH_NODE* temp = *h1;
		*h1 = *h2;
		*h2 = temp;
		return true;
	};
};


#endif