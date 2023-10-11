#ifndef FT_CONTAINERS_UNORDERED_MAP_HPP
# define FT_CONTAINERS_UNORDERED_MAP_HPP
# include <functional>
# include "hash_table.hpp"

namespace ft {
	template<
			class Key,
			class T,
			class Hash = ft::hash<Key>,
			class KeyEqual = ft::equal_to<Key>,
			class Allocator = std::allocator< ft::pair<const Key, T> >
	> class unordered_map : public hash_table<Key, ft::pair<const Key, T>, Hash, KeyEqual, Allocator, false, get_first>
	{
	private:
		typedef hash_table<Key, ft::pair<const Key, T>, Hash, KeyEqual, Allocator, false, get_first> hash_table_type;
	public:
		typedef T									mapped_type;

		unordered_map() : hash_table_type() {}

		explicit unordered_map( typename hash_table_type::size_type bucket_count,
								const Hash& hash = Hash(),
								const typename hash_table_type::key_equal& equal = typename hash_table_type::key_equal(),
								const Allocator& alloc = Allocator() ) : hash_table_type(bucket_count, hash, equal, alloc)
		{}

		explicit unordered_map( const Allocator& alloc ) : hash_table_type(alloc) {}

		template< class InputIt >
		unordered_map( InputIt first, InputIt last,
					   typename hash_table_type::size_type bucket_count = FT_HASH_TABLE_BASE_SIZE,
					   const Hash& hash = Hash(),
					   const typename hash_table_type::key_equal& equal = typename hash_table_type::key_equal(),
					   const Allocator& alloc = Allocator() ) : hash_table_type(first, last, bucket_count, hash, equal, alloc) {}

		unordered_map(const unordered_map &other) : hash_table_type(other) {}
		unordered_map( const unordered_map& other, const Allocator& alloc ) : hash_table_type(other, alloc) {}

		T& operator[]( const Key& key )
		{
			ft::pair<typename hash_table_type::iterator, bool> res = this->insert(ft::make_pair(key, mapped_type()));
			return res.first->second;
		}

	};

	template<
			class Key,
			class T,
			class Hash = ft::hash<Key>,
			class KeyEqual = ft::equal_to<Key>,
			class Allocator = std::allocator< ft::pair<const Key, T> >
	>
	class unordered_multimap : public hash_table<Key, ft::pair<const Key, T>, Hash, KeyEqual, Allocator, true, get_first> {
	private:
		typedef hash_table<Key, ft::pair<const Key, T>, Hash, KeyEqual, Allocator, true, get_first> hash_table_type;
	public:
		typedef T									mapped_type;

		unordered_multimap() : hash_table_type() {}

		explicit unordered_multimap( typename hash_table_type::size_type bucket_count,
								const Hash& hash = Hash(),
								const typename hash_table_type::key_equal& equal = typename hash_table_type::key_equal(),
								const Allocator& alloc = Allocator() ) : hash_table_type(bucket_count, hash, equal, alloc)
		{}

		explicit unordered_multimap( const Allocator& alloc ) : hash_table_type(alloc) {}

		template< class InputIt >
		unordered_multimap( InputIt first, InputIt last,
		typename hash_table_type::size_type bucket_count = FT_HASH_TABLE_BASE_SIZE,
		const Hash& hash = Hash(),
		const typename hash_table_type::key_equal& equal = typename hash_table_type::key_equal(),
		const Allocator& alloc = Allocator() ) : hash_table_type(first, last, bucket_count, hash, equal, alloc) {}

		unordered_multimap(const unordered_multimap &other) : hash_table_type(other) {}
		unordered_multimap( const unordered_multimap& other, const Allocator& alloc ) : hash_table_type(other, alloc) {}
	};

	template<
			class Key,
			class T,
			class Hash,
			class KeyEqual,
			class Allocator
	>
	void	swap(ft::unordered_map<Key, T, Hash, KeyEqual, Allocator> &lhs, ft::unordered_map<Key, T, Hash, KeyEqual, Allocator> &rhs)
	{
		rhs.swap(lhs);
	}


	template<
			class Key,
			class T,
			class Hash,
			class KeyEqual,
			class Allocator
	>
	void	swap(ft::unordered_multimap<Key, T, Hash, KeyEqual, Allocator> &lhs, ft::unordered_multimap<Key, T, Hash, KeyEqual, Allocator> &rhs)
	{
		rhs.swap(lhs);
	}
}

#endif