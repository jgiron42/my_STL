#ifndef FT_CONTAINERS_MAP_HPP
#define FT_CONTAINERS_MAP_HPP
#include "utility.hpp"
#include <memory>
#include <functional>
#include "bst.hpp"

namespace ft {

	template<
			class Key,
			class T,
			class Compare = ft::less <Key>,
			class Allocator = std::allocator <ft::pair<const Key, T> >
	>
	class map : public bst<Key, ft::pair<const Key, T>, Compare, Allocator, false, get_first> {
	private:
		typedef bst<Key, ft::pair<const Key, T>, Compare, Allocator, false, get_first> bst_type;
	public:
		typedef T									mapped_type;

		map() : bst_type() {}

		explicit map(const Compare &comp, const Allocator &alloc = Allocator()) :
			bst_type(comp, alloc){}

		template<class InputIt>
		map(InputIt first, InputIt last, const Compare &comp = Compare(), const Allocator &alloc = Allocator()) :
			bst_type(first, last, comp, alloc) {}

		map(const map &other) : bst_type(other) {}

		T& operator[]( const Key& key )
		{
			ft::pair<typename bst_type::iterator, bool> res = this->insert(ft::make_pair(key, mapped_type()));
			return res.first->second;
		}
	};

	template<
			class Key,
			class T,
			class Compare = ft::less <Key>,
			class Allocator = std::allocator <ft::pair<const Key, T> >
	>
	class multimap : public bst<Key, ft::pair<const Key, T>, Compare, Allocator, true, get_first> {
	private:
		typedef bst<Key, ft::pair<const Key, T>, Compare, Allocator, true, get_first> bst_type;
	public:
		typedef Key									key_type;
		typedef T									mapped_type;
		typedef ft::pair<const Key, T>				value_type;

		multimap() : bst_type() {}

		explicit multimap(const Compare &comp, const Allocator &alloc = Allocator()) :
			bst_type(comp, alloc){}

		template<class InputIt>
		multimap(InputIt first, InputIt last, const Compare &comp = Compare(), const Allocator &alloc = Allocator()) :
			bst_type(first, last, comp, alloc) {}

		multimap(const multimap &other) : bst_type(other) {}

	};


	template<
			class Key,
			class T,
			class Compare,
			class Allocator
	>
	void	swap(ft::map<Key, T, Compare, Allocator> &lhs, ft::map<Key, T, Compare, Allocator> &rhs)
	{
		rhs.swap(lhs);
	}


	template<
			class Key,
			class T,
			class Compare,
			class Allocator
	>
	void	swap(ft::multimap<Key, T, Compare, Allocator> &lhs, ft::multimap<Key, T, Compare, Allocator> &rhs)
	{
		rhs.swap(lhs);
	}
}

#endif
