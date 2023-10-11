#ifndef FT_CONTAINERS_UNORDERED_SET_HPP
# define FT_CONTAINERS_UNORDERED_SET_HPP
# include <functional>
# include "hash_table.hpp"

namespace ft {
	template<
			class Key,
			class Hash = ft::hash<Key>,
			class KeyEqual = ft::equal_to<Key>,
			class Allocator = std::allocator<Key>
	> class unordered_set : public hash_table<Key, Key, Hash, KeyEqual, Allocator, false, ft::identity>
	{
	private:
		typedef hash_table<Key, Key, Hash, KeyEqual, Allocator, false, ft::identity> hash_table_type;
	public:
		typedef Key											value_type;

		unordered_set() : hash_table_type() {}

		explicit unordered_set( typename hash_table_type::size_type bucket_count,
								const Hash& hash = Hash(),
								const typename hash_table_type::key_equal& equal = typename hash_table_type::key_equal(),
								const Allocator& alloc = Allocator() ) : hash_table_type(bucket_count, hash, equal, alloc)
		{}

		explicit unordered_set( const Allocator& alloc ) : hash_table_type(alloc) {}

		template< class InputIt >
		unordered_set( InputIt first, InputIt last,
					   typename hash_table_type::size_type bucket_count = FT_HASH_TABLE_BASE_SIZE,
					   const Hash& hash = Hash(),
					   const typename hash_table_type::key_equal& equal = typename hash_table_type::key_equal(),
					   const Allocator& alloc = Allocator() ) : hash_table_type(first, last, bucket_count, hash, equal, alloc) {}

		unordered_set(const unordered_set &other) : hash_table_type(other) {}
		unordered_set( const unordered_set& other, const Allocator& alloc ) : hash_table_type(other, alloc) {}
	};

	template<
			class Key,
			class Hash = ft::hash<Key>,
			class KeyEqual = ft::equal_to<Key>,
			class Allocator = std::allocator<Key>
	>
	class unordered_multiset : public hash_table<Key, Key, Hash, KeyEqual, Allocator, true, ft::identity> {
	private:
		typedef hash_table<Key, Key, Hash, KeyEqual, Allocator, true, ft::identity> hash_table_type;
	public:
		typedef Key									value_type;

		unordered_multiset() : hash_table_type() {}

		explicit unordered_multiset( typename hash_table_type::size_type bucket_count,
								const Hash& hash = Hash(),
								const typename hash_table_type::key_equal& equal = typename hash_table_type::key_equal(),
								const Allocator& alloc = Allocator() ) : hash_table_type(bucket_count, hash, equal, alloc)
		{}

		explicit unordered_multiset( const Allocator& alloc ) : hash_table_type(alloc) {}

		template< class InputIt >
		unordered_multiset( InputIt first, InputIt last,
		typename hash_table_type::size_type bucket_count = FT_HASH_TABLE_BASE_SIZE,
		const Hash& hash = Hash(),
		const typename hash_table_type::key_equal& equal = typename hash_table_type::key_equal(),
		const Allocator& alloc = Allocator() ) : hash_table_type(first, last, bucket_count, hash, equal, alloc) {}

		unordered_multiset(const unordered_multiset &other) : hash_table_type(other) {}
		unordered_multiset( const unordered_multiset& other, const Allocator& alloc ) : hash_table_type(other, alloc) {}
	};


	template<
			class Key,
			class Hash,
			class KeyEqual,
			class Allocator
	>
	void	swap(ft::unordered_set<Key, Hash, KeyEqual, Allocator> &lhs, ft::unordered_set<Key, Hash, KeyEqual, Allocator> &rhs)
	{
		rhs.swap(lhs);
	}


	template<
			class Key,
			class Hash,
			class KeyEqual,
			class Allocator
	>
	void	swap(ft::unordered_multiset<Key, Hash, KeyEqual, Allocator> &lhs, ft::unordered_multiset<Key, Hash, KeyEqual, Allocator> &rhs)
	{
		rhs.swap(lhs);
	}
}

#endif