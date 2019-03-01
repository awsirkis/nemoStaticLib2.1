#pragma once
#if defined(__has_include)
#if __has_include(<typeinfo>)
#include <typeinfo>
#elif __has_include(<typeinfo.h>)
#include <typeinfo.h>
#endif
#if __has_include(<iterator>)
#include <iterator>
#endif
#if __has_include(<array>)
#include <array>
#endif
#endif
#define _max_bucket_count 10000

template <class key, class Value> class HASH_MAP
{
	int _bucket_count = 100;
	int array_size = 0;

	struct HASH_NODE {
		key KEY;
		Value VALUE;
		HASH_NODE(key k, Value v) {
			KEY = k;
			VALUE = v;
		}
		~HASH_NODE() {
			delete KEY;
			delete VALUE;
		}
		HASH_NODE operator=(const HASH_NODE& H) {
			KEY = H.KEY;
			VALUE = H.VALUE;
		}
	};
	HASH_NODE* buckets;
	//const std::type_info& key_type = typeid(key);
	//const std::type_info& mapped_type = typeid(Value);
	//const std::type_info& value_type = pair < key_type, mapped_type> ;
	std::hash<key> hasher;
public:
	HASH_MAP() { 
		buckets = new HASH_NODE[bucket_count]; 
	};
	~HASH_MAP() { 
		delete[] buckets; 
	};
	HASH_MAP(const HASH_MAP&H) { 
		delete[] buckets;
		buckets = new HASH_NODE[H.bucket_count];
		for (int i = 0; i < _bucket_count; i++) {
			buckets[i] = new HASH_NODE(H.buckets[i].KEY, H.buckets[i].VALUE);
		}
	};
	HASH_MAP operator=(const HASH_MAP& H) {
		delete[] buckets;
		buckets = new HASH_NODE[H.bucket_count];
		for (int i = 0; i < _bucket_count; i++) {
			buckets[i] = new HASH_NODE(H.buckets[i].KEY, H.buckets[i].VALUE);
		}
	};

	// Size Operators, NoExcep
	bool empty() const { 
		return array_size == 0; 
	};
	int size() const {
		return array_size;
	};
	//inline int max_size() const; // UNNECSSARY FOR NEMOSTATICLIB

	// Access Functions
	Value& operator[](const key&) const {
		return buckets[std::hash<key>(key)];
	};
	Value& at(const key&) const {
		return buckets[std::hash<key>(key)];
	};
	//iterator find(const key&) const; // UNNECESSARY FOR NEMOSTATICLIB
	int count(const key& k) const {
		int found = 0;
		auto ptr = begin();
		for (ptr; ptr < end(); ptr++) {
			if (ptr->key == k) {
				found++;
			}
		}
		return found;
	};
	//pair<int, int> equal_range(const key&) const; UNNECESSARY FOR NEMOSTATICLIB

	//Iterator Functions
	inline auto begin() const {
		return begin(buckets);
	};
	inline auto end() const {
		return end(buckets);
	};
	//const iterator cbegin() const
	//const iterator cend() const

	//Modifiers
	//used in SubgraphCount.cpp as type 6
	void insert(initializer_list<key, Value>) { size++; };
	//emplace
	//emplace_hint
	//erase - only used in std:vector
	//clear
	//swap

	//Buckets - UNUSED IN NEMO STATIC LIB
	inline size_t bucket_count() const { 
		return _bucket_count; 
	};
	inline size_t max_bucket_count() const { 
		return _max_bucket_count; 
	};
	//size_type bucket_size(size_type n) const;
	//size_type bucket(const key& k) const; // return bucket with key k

	//Hash Policy
	float load_factor() const noexcept { 
		return size() / bucket_count();
	};
	inline float max_load_factor() const noexcept { 
		return max_load_factor; 
	}; // get
	inline void max_load_factor(float z) { 
		max_load_factor = z; 
	};	// set
	// reserve - unused in NemoStaticLib

	//Observers - Unused
	//hasher hash_function() const;
	//key_equal key_eq() const;
	//allocator_type get_allocator() const noexcept;
};