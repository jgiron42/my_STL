#ifndef FT_CONTAINERS_SET_HPP
#define FT_CONTAINERS_SET_HPP
#include "utility.hpp"
#include <memory>
#include <functional>
#include "bst.hpp"

namespace ft {
	template<
			class Key,
			class Compare = ft::less<Key>,
			class Allocator = std::allocator<Key>
	>
	class set : public bst<Key, Key, Compare, Allocator> {
	private:
		typedef bst<Key, Key, Compare, Allocator> bst_type;
	public:
		typedef Key											value_type;

		set() : bst_type() {}

		explicit set(const Compare &comp, const Allocator &alloc = Allocator()) :
			bst_type(comp, alloc){}

		template<class InputIt>
		set(InputIt first, InputIt last, const Compare &comp = Compare(), const Allocator &alloc = Allocator()) :
			bst_type(first, last, comp, alloc) {}

		set(const set &other) : bst_type(other) {}

	};

	template<
			class Key,
			class Compare = ft::less<Key>,
			class Allocator = std::allocator<Key>
	>
	class multiset : public bst<Key, Key, Compare, Allocator, true> {
	private:
		typedef bst<Key, Key, Compare, Allocator, true> bst_type;
	public:
		typedef Key									value_type;

		multiset() : bst_type() {}

		explicit multiset(const Compare &comp, const Allocator &alloc = Allocator()) :
			bst_type(comp, alloc){}

		template<class InputIt>
		multiset(InputIt first, InputIt last, const Compare &comp = Compare(), const Allocator &alloc = Allocator()) :
			bst_type(first, last, comp, alloc) {}

		multiset(const multiset &other) : bst_type(other) {}
	};



	template<
			class Key,
			class T,
			class Compare
	>
	void	swap(ft::set<Key, T, Compare> &lhs, ft::set<Key, T, Compare> &rhs)
	{
		rhs.swap(lhs);
	}



	template<
			class Key,
			class T,
			class Compare
	>
	void	swap(ft::multiset<Key, T, Compare> &lhs, ft::multiset<Key, T, Compare> &rhs)
	{
		rhs.swap(lhs);
	}
}

#endif
