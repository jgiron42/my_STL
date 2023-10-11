#ifndef FT_CONTAINERS_LIST_HPP
# define FT_CONTAINERS_LIST_HPP
#include <memory>
#include <iostream>
#include "algorithm.hpp"
#include "utility.hpp"
#include "functional.hpp"
#include "iterator.hpp"

namespace ft {

	// xorlist is a sequence container implemented as a xorlist (https://en.wikipedia.org/wiki/XOR_linked_list)
	// xorlist comply to the requirements Container, SequenceContainer and ReversibleContainer
	template<
			class T,
			class Allocator = std::allocator<T>
	>
	class xorlist {
	public:
		typedef T									value_type;
		typedef Allocator							allocator_type;
		typedef size_t								size_type;
		typedef ptrdiff_t							difference_type;
		typedef value_type							&reference;
		typedef const value_type					&const_reference;
		typedef typename Allocator::pointer			pointer;
		typedef typename Allocator::const_pointer	const_pointer;
	private:
		struct node_type_base {
			node_type_base(value_type v) : value(v) {}
			node_type_base(value_type &v) : value(v) {}
			node_type_base(const node_type_base &other) : xptr(other.xptr), value(other.value) {}
			node_type_base *xptr;
			value_type value;
		};
		template<typename pointed_type>
		struct iterator_t {
			typedef ptrdiff_t difference_type;
			typedef xorlist::value_type value_type;
			typedef pointed_type *pointer;
			typedef pointed_type &reference;
			typedef ft::bidirectional_iterator_tag iterator_category;

			xorlist::node_type_base *prev;
			xorlist::node_type_base *current;
			xorlist::node_type_base *next;

			iterator_t() : prev(NULL), current(NULL), next(NULL) {}

			explicit iterator_t(xorlist::node_type_base *p, xorlist::node_type_base *c, xorlist::node_type_base *n) : prev(p), current(c), next(n) {}

			explicit iterator_t(const xorlist::node_type_base *p, const xorlist::node_type_base *c, const xorlist::node_type_base *n) : prev((xorlist::node_type_base *)p), current((xorlist::node_type_base *)c), next((xorlist::node_type_base *)n) {}

			template<typename U>
			iterator_t(iterator_t<U> const &other) : prev(other.prev), current(other.current), next(other.next) {}

			iterator_t &operator++() {
				prev = current;
				current = next;
				if (current)
					next = (node_type_base*)((size_t)current->xptr ^ (size_t)prev);
				return *this;
			}

			iterator_t operator++(int) {
				iterator_t ret = *this;
				++*this;
				return ret;
			}

			iterator_t &operator--() {
				next = current;
				current = prev;
				if (current)
					prev = (node_type_base*)((size_t)current->xptr ^ (size_t)next);
				return *this;
			}

			iterator_t operator--(int) {
				iterator_t ret = *this;
				--*this;
				return ret;
			};

			reference operator*() const { return (this->current)->value; }

			pointer operator->() const { return &(this->current)->value; }
		};
		template<typename LT, typename RT>
		friend bool operator==(iterator_t<LT> l, iterator_t<RT> r) { return l.current == r.current; }

		template<typename LT, typename RT>
		friend bool operator!=(iterator_t<LT> l, iterator_t<RT> r) { return l.current != r.current; }
	public:
		typedef iterator_t<value_type> iterator;
		typedef iterator_t<const value_type> const_iterator;
		typedef ft::reverse_iterator<iterator> reverse_iterator;
		typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;
	private:
		node_type_base	*first;
		node_type_base	*last;
		size_type		_size;
		typename allocator_type::template rebind<node_type_base>::other allocator;
	public:
		// Description: Default constructor. Constructs an empty container with a default-constructed allocator.
		// Complexity:  Constant
		xorlist() : first(), last(), _size(0), allocator() {}

		// Description: Constructs an empty container with the given allocator alloc
		// Complexity:  Constant
		explicit xorlist( const Allocator& alloc ) : first(), last(), _size(0), allocator(alloc) {}

		// Description: Constructs the container with count copies of elements with value.
		// Complexity:  O(count)
		explicit xorlist( size_type count,
					   const T& value = T(),
					   const Allocator& alloc = Allocator()) : first(), last(), _size(0), allocator(alloc) {
			this->insert(this->end(), count, value);
		}

		// Description: Constructs the container with the contents of the range [first, last).
		// Complexity:  O(ft::distance(f, l))
		template< class InputIt >
		xorlist( InputIt f, InputIt l,
			  const Allocator& alloc = Allocator()) : first(), last(), _size(0), allocator(alloc)
		{
			this->insert(this->end(), f, l);
		}

		// Description: Copy constructor. Constructs the container with the copy of the contents of other.
		// Complexity:  O(other.size())
		xorlist( const xorlist& other ) : first(), last(), _size(0), allocator(other.allocator) {
			this->insert(this->end(), other.begin(), other.end());
		}


		// Description: Destructs the list. The destructors of the elements are called and the used storage is deallocated.
		// Complexity:  O(size())
		~xorlist()
		{
			this->clear();
		}

		// Description: Copy assignment operator. Replaces the contents with a copy of the contents of other.
		// Complexity:  O(other.size())
		// iterators:   All iterators are invalidated
		xorlist& operator=( const xorlist& other ) {
			this->assign(other.begin(), other.end());
			return *this;
		}

		// Description: Equivalent of *this = xorlist(count, value).
		// Complexity:  O(count)
		// iterators:   All iterators are invalidated
		void assign( size_type count, const T& value ) {
			this->clear();
			this->insert(this->end(), count, value);
		}

		// Description: Equivalent of *this = xorlist(f, l).
		// Complexity:  O(ft::distance(f, l))
		// iterators:   All iterators are invalidated
		template< class InputIt >
		void assign( InputIt f, InputIt l )
		{
			this->clear();
			this->insert(this->end(), f, l);
		}

		// Description:  return the allocator object used by the container
		// Complexity:   Constant
		// return value: a copy of the allocator used by the container
		allocator_type get_allocator() const { return this->allocator; }


		// Description:  return the first object stored in the list
		// Complexity:   Constant
		// return value: *this->begin()
		reference front() {return *this->begin();}
		const_reference front() const {return *this->begin();}

		// Description:  return the last object stored in the list
		// Complexity:   Constant
		// return value: *prev(this->end())
		reference back() {return *this->rbegin();}
		const_reference back() const {return *this->rbegin();}

		// Description:  return an iterator to the first object of the container
		// Complexity:   Constant
		// return value: an iterator to the first object of the container
		iterator begin() {return iterator(NULL, this->first, this->first ? this->first->xptr : NULL);}
		const_iterator begin() const {return const_iterator(NULL, this->first, this->first ? this->first->xptr : NULL);}


		// Description:  return an iterator pointing after the last object of the container
		// Complexity:   Constant
		// return value: an iterator pointing after the last object of the container
		iterator end() {return iterator(this->last , NULL, NULL);}
		const_iterator end() const {return const_iterator(this->last , NULL, NULL);}


		// Description:  return a reverse iterator to the first object of the reversed container
		// Complexity:   Constant
		// return value: reverse_iterator(this->end())
		reverse_iterator rbegin() { return reverse_iterator(this->end()); }
		const_reverse_iterator rbegin() const { return const_reverse_iterator(this->end()); }


		// Description:  return a reverse iterator pointing after the last object of the reversed container
		// Complexity:   Constant
		// return value: reverse_iterator(this->begin())
		reverse_iterator rend() { return reverse_iterator(this->begin()); }
		const_reverse_iterator rend() const { return const_reverse_iterator(this->begin()); }

		// Description:  Checks if the container has no elements
		// Complexity:   Constant
		// return value: true if the container is empty, false otherwise
		bool empty() const { return this->_size == 0; }


		// Description:  get the size of the container
		// Complexity:   Constant
		// return value: the size of the container
		size_type size() const { return this->_size; }

		// Description:  get the maximum possible size of the container
		// Complexity:   Constant
		// return value: this->get_allocator().max_size()
		size_type max_size() const { return this->allocator.max_size(); };

		// Description:  suppress all elements of the container
		// Complexity:   O(this->size())
		// iterators:    all iterators are invalidated
		void clear()
		{
			node_type_base *prev = NULL;
			while (this->first)
			{
				node_type_base *tmp = this->first;
				this->first = (node_type_base*)((size_t)prev ^ (size_t)this->first->xptr);
				prev = tmp;
				destroy_node(*prev);
			}
			this->last = NULL;
		}

		// Description:  insert the element value before the iterator pos
		// Complexity:   Constant
		// return value: an iterator pointing to the inserted element
		// iterators:    the iterators before and after pos are invalidated
		iterator insert( const_iterator pos, const T& value )
		{
			return this->insert_node(pos, new_node(value));
		}


		// Description:  insert count elements value before the iterator pos
		// Complexity:   O(count)
		// return value: an iterator pointing to the first inserted element
		// iterators:    the iterators before and after pos are invalidated
		iterator insert( const_iterator pos, size_type count, const T& value )
		{
			for (;count > 0; count--)
				pos = this->insert(pos, value);
			return pos;
		}

		// Description:  insert the range [f, l) before the iterator pos
		// Complexity:   O(distance(f,l))
		// return value: an iterator pointing to the first inserted element
		// iterators:    the iterators before and after pos are invalidated
		template< class InputIt >
		iterator insert( const_iterator pos, InputIt f, InputIt l )
		{
			if (f == l)
				return pos;
			iterator ret = this->insert(pos, *f);
			f++;
			pos = ft::next(ret);
			for (;f != l; f++)
				pos = ft::next(this->insert(pos, *f));
			return ret;
		}

		// Description:  erase the element pointed by pos
		// Complexity:   Constant
		// return value: an iterator pointing to the element after pos
		// iterators:    the iterators before and after pos are invalidated
		iterator erase( iterator pos ) {
			if (pos.prev)
				pos.prev->xptr = (node_type_base *) ((size_t) pos.prev->xptr ^ (size_t) pos.current ^
													 (size_t) pos.next);
			else
				this->first = pos.next;
			if (pos.next)
				pos.next->xptr = (node_type_base *) ((size_t) pos.next->xptr ^ (size_t) pos.current ^
													 (size_t) pos.prev);
			else
				this->last = pos.prev;
			destroy_node(*pos.current);
			return iterator(pos.prev, pos.next,
							pos.next ? (node_type_base *) ((size_t) pos.next->xptr ^ (size_t) pos.prev) : NULL);
		}

		// Description:  erase the element pointed by pos
		// Complexity:   O(distance(first, last))
		// return value: an iterator equivalent to last
		// iterators:    the iterators before first and after last are invalidated
		iterator erase( iterator first, iterator last )
		{
			while (first != last)
				first = this->erase(first);
			return first;
		}


		// Description:  insert value at the end of the container
		// Complexity:   Constant
		// iterators:    the end iterator and the iterator to the last element are invalidated
		void push_back( const T& value ) {this->insert(const_iterator(this->end()), value);}

		// Description:  remove the last value of the container
		// Complexity:   Constant
		// iterators:    the end iterator and the iterator to the last element and the element before are invalidated
		void pop_back() {this->erase(--this->end());}

		// Description:  insert value at the beginning of the container
		// Complexity:   Constant
		// iterators:    the iterator to the first element is invalidated
		void push_front( const T& value ) {this->insert(this->begin(), value);}

		// Description:  remove the first value of the container
		// Complexity:   Constant
		// iterators:    the iterator to the first element and the element after are invalidated
		void pop_front() {this->erase(this->begin());}


		// Description:  if count is greater than size, add element at the end until size() == count
		//				 if count is less than size, remove elements at the end until size() == count
		// Complexity:   O(abs(count - size))
		// iterators:    all iterators are invalidated
		void resize( size_type count, T value = T() )
		{
			if (count > this->_size)
				this->insert(this->end(), count - this->_size, value);
			else
				while (this->_size > count)
					this->erase(--this->end());
		}

		// Description:  swap the content of the container with other
		// Complexity:   Constant
		void swap( xorlist& other )
		{
			ft::swap(this->first, other.first);
			ft::swap(this->last, other.last);
			ft::swap(this->allocator, other.allocator);
			ft::swap(this->_size, other._size);
		}

		// Description:  merges other into *this. Both lists should be sorted. No elements
		//				 are copied, and the container other becomes empty after the merge.
		//				 This operation is stable: for equivalent elements in the two
		//				 lists, the elements from *this always precede the elements from
		//				 other, and the order of equivalent elements of *this and other
		//				 does not change.
		//				 The elements are compared using the < operator
		// Complexity:   O(this->size() + other.size())
		// iterators:    all iterators are invalidated
		void merge( xorlist& other ) {this->merge(other, ft::less<value_type>());}

		// Description:  merges other into *this. Both lists should be sorted. No elements
		//				 are copied, and the container other becomes empty after the merge.
		//				 This operation is stable: for equivalent elements in the two
		//				 lists, the elements from *this always precede the elements from
		//				 other, and the order of equivalent elements of *this and other
		//				 does not change.
		//				 The elements are compared using the binary predicate comp
		// Complexity:   O(this->size() + other.size())
		// iterators:    all iterators are invalidated
		template < class Compare >
		void merge( xorlist& other, Compare comp )
		{
			if (&other == this)
				return;
			iterator it = this->begin();
			iterator it_other = other.begin();
			while (it_other != other.end())
			{
				iterator tmp = it_other;
				if (it == this->end() || comp(*tmp, *it))
				{
					++it_other;
					it = ft::next(this->insert_node(it, tmp.current));
				}
				else
					it++;
			}
			other.first = NULL;
			other.last = NULL;
			this->_size += other.size();
			other._size = 0;
		}


		// Description:  move insert the content of the list other before the iterator pos.
		// Complexity:   O(other.size())
		// iterators:    the iterators before and after pos are invalidated
		// return value: an iterator to the inserted element
		iterator splice( const_iterator pos, xorlist& other )
		{
			return this->splice(pos, other, other.begin(), other.end());
		}

		// Description:  move insert the element pointed by it of the list other before the iterator pos.
		// Complexity:   Constant
		// iterators:    the iterators before and after pos are invalidated
		// return value: an iterator to the inserted element
		iterator splice( const_iterator pos, xorlist& other, const_iterator it )
		{
			return this->splice(pos, other, it, ft::next(it));
		}

		// Description:  move insert the range [f, l) of the list other before the iterator pos.
		// Complexity:   if this == &other Contant otherwise distance(f, l)
		// iterators:    the iterators before and after pos are invalidated
		// return value: an iterator to the first inserted element
		iterator splice( const_iterator pos, xorlist& other,
					 const_iterator f, const_iterator l)
		{
			if (f == l)
				return pos;
			if (&other != this)
			{
				size_type count = ft::distance(f, l);
				this->_size += count;
				other._size -= count;
			}

			// now:
			// this  ====AB=====
			// other ==CD===EF==
			// goal:
			// this  ====AD===EB=====
			// other ==CF==

			node_type_base	*A = pos.prev,
							*B = pos.current,
							*C = f.prev,
							*D = f.current,
							*E = l.prev,
							*F = l.current;
			if (A)
				A->xptr = (node_type_base*)((size_t)A->xptr ^ (size_t)B ^ (size_t)D);
			else
				this->first = D;

			D->xptr = (node_type_base*)((size_t)D->xptr ^ (size_t)(C) ^ (size_t)A);

			if (B)
				B->xptr = (node_type_base*)((size_t)B->xptr ^ (size_t)A ^ (size_t)E);
			else
				this->last = E;

			E->xptr = (node_type_base*)((size_t)E->xptr ^ (size_t)(F) ^ (size_t)B);


			if (C)
				C->xptr = (node_type_base*)((size_t)C->xptr ^ (size_t)D ^ (size_t)F);
			else
				other.first = F;
			if (F)
				F->xptr = (node_type_base*)((size_t)F->xptr ^ (size_t)E ^ (size_t)C);
			else
				other.last = C;

			return iterator(A, D, (node_type_base*)((size_t)D->xptr ^ (size_t)A));
		}

		// Description:  Remove all elements that compares equal to value.
		//				 the elements are compared using the == operator
		// Complexity:   O(this->size())
		// iterators:    all iterators are invalidated
		void remove( const T& value ) {return remove_if(ft::binder1st<equal_to<T> >(equal_to<T>(), value));}

		// Description:  Remove all elements for which predicate p return true.
		// Complexity:   O(this->size())
		// iterators:    all iterators are invalidated
		template< class UnaryPredicate >
		void remove_if( UnaryPredicate p )
		{
			for (iterator it = this->begin(); it != this->end();)
				if (p(*it))
					it = this->erase(it);
				else
					it++;
		}

		// Description:  reverse the order of the elements of the container.
		// Complexity:   Constant
		// iterators:    all iterators are invalidated
		void reverse() {ft::swap(this->first, this->last);}

		// Description:  Remove all consecutive duplicate elements. Only the first element in each group of equal elements is left.
		//				 the elements are compared using the == operator
		// Complexity:   O(this->size())
		// iterators:    all iterators are invalidated
		void unique()
		{
			this->unique(equal_to<value_type>());
		}

		// Description:  Remove all consecutive duplicate elements. Only the first element in each group of equal elements is left.
		//				 the elements are compared using predicate p
		// Complexity:   O(this->size())
		// iterators:    all iterators are invalidated
		template< class BinaryPredicate >
		void unique( BinaryPredicate p )
		{
			for (iterator it = this->begin(), next = ft::next(this->begin()); next != this->end();)
				if (p(*it, *next))
				{
					next = this->erase(next);
					it = ft::prev(next);
				}
				else
				{
					it++;
					next++;
				}
		}

		// Description:  Sorts the elements and preserves the order of equivalent elements.
		//				 Elements are compared using operator<.
		// Complexity:   O(log(this->size()) * this->size())
		// iterators:    all iterators are invalidated
		void sort() {sort(ft::less<value_type>());}

		// Description:  Sorts the elements and preserves the order of equivalent elements.
		//				 Elements are compared using the comparison object comp.
		// Complexity:   O(log(this->size()) * this->size())
		// iterators:    all iterators are invalidated
		template< class Compare >
		void sort( Compare comp )
		{
			size_type group_size = 1;
			while (group_size < this->size())
			{
				iterator g1 = this->begin();
				iterator g2 = this->begin();
				for (size_type group_pair = 0; (group_size * (group_pair * 2 + 1)) < this->size(); group_pair++)
				{
					// remaining elements in each groups
					size_type g1_element = group_size;
					size_type g2_element = ft::min(group_size, this->size() - (group_size * (group_pair * 2 + 1)));
					g2 = ft::next(g2, group_size);

					while (g1_element > 0 && g2_element > 0)
					{
						if (comp(*g2, *g1))
						{
							iterator tmp = ft::next(g2);
							tmp.prev = g2.prev;
							this->splice(g1, *this, g2);
							g1.prev = g2.current;
							g2 = tmp;
							g2_element--;
						}
						else
						{
							g1++;
							g1_element--;
						}
					}
					// move both iterators to the beginning of the next group pair
					g2 = ft::next(g2, g2_element);
					g1 = g2;
				}
				group_size *= 2;
			}
		}
	private:
		iterator	insert_node(const_iterator pos, node_type_base *node)
		{

			node_type_base *current = pos.current;
			node_type_base *prev = pos.prev;
			node_type_base *n = node;
			if (current)
				current->xptr = (node_type_base*)((size_t)n ^ (size_t)pos.next);
			else
				last = n;
			n->xptr = (node_type_base*)((size_t)current ^ (size_t)prev);
			if (prev)
				prev->xptr = (node_type_base*)((size_t)current ^ (size_t)prev->xptr ^ (size_t)n);
			else
				first = n;
			return iterator(prev, n, current);
		}

		node_type_base *new_node(const node_type_base &val)
		{
			node_type_base *ret = this->allocator.allocate(1);
			this->allocator.construct(ret, val);
			this->_size++;
			return ret;
		}
		void	destroy_node(node_type_base &node)
		{
			this->allocator.destroy(&node);
			this->allocator.deallocate(&node, 1);
			this->_size--;
		}
	};

	// Description:  Compares the contents of two xorlists.
	// Complexity:   O(this->size())
	// return value: true if the contents of the lists are equal, false otherwise.
	template <class T, class Allocator>
	bool operator==(const xorlist<T, Allocator> &lhs, const xorlist<T, Allocator> &rhs) {
		return ((lhs.size() == rhs.size()) && ft::equal<xorlist<T, Allocator>::const_iterator, xorlist<T, Allocator>::const_iterator>(lhs.begin(), lhs.end(), rhs.begin()));
	}

	// Description:  Compares the contents of two xorlists.
	// Complexity:   O(this->size())
	// return value: true if the contents of the lists are not equal, false otherwise.
	template <class T, class Allocator>
 	bool operator!=(const xorlist<T, Allocator> &lhs, const xorlist<T, Allocator> &rhs) {
		return (!(lhs == rhs));
	}

	// Description:  Compares the contents of two xorlists.
	// Complexity:   O(this->size())
	// return value: true if the contents of the lhs are lexicographically less than the contents of rhs, false otherwise.
	template <class T, class Allocator>
 	bool operator<(const xorlist<T, Allocator> &lhs, const xorlist<T, Allocator> &rhs) {
		return (ft::lexicographical_compare<xorlist<T, Allocator>::const_iterator, xorlist<T, Allocator>::const_iterator>(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}

	// Description:  Compares the contents of two xorlists.
	// Complexity:   O(this->size())
	// return value: true if the contents of the lhs are lexicographically less than or equal to the contents of rhs, false otherwise.
	template <class T, class Allocator>
	bool operator<=(const xorlist<T, Allocator> &lhs, const xorlist<T, Allocator> &rhs) {
		return (!(rhs < lhs));
	}

	// Description:  Compares the contents of two xorlists.
	// Complexity:   O(this->size())
	// return value: true if the contents of the lhs are lexicographically greater than the contents of rhs, false otherwise.
	template <class T, class Allocator>
	bool operator>(const xorlist<T, Allocator> &lhs, const xorlist<T, Allocator> &rhs) {
		return (rhs < lhs);
	}

	// Description:  Compares the contents of two xorlists.
	// Complexity:   O(this->size())
	// return value: true if the contents of the lhs are lexicographically greater than or equal to the contents of rhs, false otherwise.
	template <class T, class Allocator>
	bool operator>=(const xorlist<T, Allocator> &lhs, const xorlist<T, Allocator> &rhs) {
		return (!(lhs < rhs));
	}

	// Description:  Swap the contents of two xorlists.
	// Complexity:   Constant
	template< class T, class Alloc >
	void swap( ft::xorlist<T, Alloc>& lhs,
			   ft::xorlist<T, Alloc>& rhs)
	{
		lhs.swap(rhs);
	}
}
#endif