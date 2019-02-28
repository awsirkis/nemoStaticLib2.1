#pragma once
#if defined(__has_include)
#if __has_include(<typeinfo>)
#include <typeinfo>
#elif __has_include(<typeinfo.h>)
#include <typeinfo.h>
#endif
#endif
template <class key, class Value> class HASH_MAP
{
	float load_factor = 1.0;
	int max_bucket_count = 10000;
	int bucket_count = 0;

	const std::type_info& key_type = typeid(key);
	const std::type_info& mapped_type MAPPED = typeid(Value);
	const std::type_info& value_type VALUE = pair<const key_type, mapped_type>;
	hasher = Hash<key_type>
public:
	HASH_MAP();
	~HASH_MAP();
	HASH_MAP(const HASH_MAP&);
	HASH_MAP operator=(const HASH_MAP&);

	// Size Operators, NoExcep
	bool empty() const;
	int size() const;
	//inline int max_size() const; // UNNECSSARY FOR NEMOSTATICLIB

	// Access Functions
	Value& operator[](const key&) const;
	Value& at(const key&) const;
	//iterator find(const key&) const; // UNNECESSARY FOR NEMOSTATICLIB
	int count(const key&) const;
	//pair<int, int> equal_range(const key&) const; UNNECESSARY FOR NEMOSTATICLIB

	//Iterator Functions
	iterator begin() const;
	iterator end() const;
	//const iterator cbegin() const
	//const iterator cend() const

	//Modifiers
	//used in SubgraphCount.cpp as type 6
	void insert(initializer_list<value_type>);
	//emplace
	//emplace_hint
	//erase - only used in std:vector
	//clear
	//swap

	//Buckets - UNUSED IN NEMO STATIC LIB
	inline size_type bucket_count() const noexcept { return bucket_count; };
	inline size_type max_bucket_count() const noexcept { return max_bucket_count; };
	size_type bucket_size(size_type n) const;
	size_type bucket(const key&) const; // reutrn bucket with key k

	//Hash Policy
	inline float load_factor() const noexcept { return size() / bucket_count()};
	inline float max_load_factor() const noexcept { return load_factor; }; // get
	inline void max_load_factor(float z) { load - factor = z; };	// set
	// reserve - unused in NemoStaticLib

	//Observers - Unused
	//hasher hash_function() const;
	//key_equal key_eq() const;
	//allocator_type get_allocator() const noexcept;
};

