#ifndef FT_CONTAINERS_LIST_HPP
# define FT_CONTAINERS_LIST_HPP
#include <memory>
#include "algorithm.hpp"
#include "utility.hpp"
#include "functional.hpp"
#include "iterator.hpp"
#include "type_traits.hpp"
#include <iostream>

namespace ft {
	template<
			class T,
			class Allocator = std::allocator<T>
	>
	class list {
	public:
		typedef T value_type;
		typedef Allocator allocator_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef value_type &reference;
		typedef const value_type &const_reference;
		typedef typename Allocator::pointer pointer;
		typedef typename Allocator::const_pointer const_pointer;
	private:
		struct node_type_base {
			node_type_base() : prev(NULL), next(NULL) {}
			node_type_base(node_type_base *p, node_type_base *n) : prev(p), next(n) {}
			node_type_base(const node_type_base &other) : prev(other.prev), next(other.next) {}
			node_type_base *prev;
			node_type_base *next;
		};

		struct node_type : public node_type_base {
			node_type(const value_type &v) : value(v) {}
			node_type(const node_type &other) : node_type_base(other), value(other.value) {}
			value_type value;
		};
		template<typename pointed_type>
		struct iterator_t {
			typedef ptrdiff_t difference_type;
			typedef list::value_type value_type;
			typedef pointed_type *pointer;
			typedef pointed_type &reference;
			typedef ft::bidirectional_iterator_tag iterator_category;

			list::node_type_base *current;

			iterator_t() : current(NULL) {}

			explicit iterator_t(list::node_type_base *c) : current(c) {}

			explicit iterator_t(const list::node_type_base *c) : current((list::node_type_base *)c) {}

			template<typename U>
			iterator_t(iterator_t<U> const &other) : current(other.current) {}

			iterator_t &operator++() {
				current = current->next;
				return *this;
			}

			iterator_t operator++(int) {
				iterator_t ret = *this;
				++*this;
				return ret;
			}

			iterator_t &operator--() {
				current = current->prev;
				return *this;
			}

			iterator_t operator--(int) {
				iterator_t ret = *this;
				--*this;
				return ret;
			};

			reference operator*() const { return static_cast<node_type*>(this->current)->value; }

			pointer operator->() const { return &static_cast<node_type*>(this->current)->value; }
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
		node_type_base	past_the_end;
		size_type		_size;
		typename allocator_type::template rebind<node_type>::other allocator;
	public:
		list() : past_the_end(&this->past_the_end, &this->past_the_end), _size(0), allocator() {}
		explicit list( const Allocator& alloc ) : past_the_end(&this->past_the_end, &this->past_the_end), _size(0), allocator(alloc) {}
		explicit list( size_type count,
					   const T& value = T(),
					   const Allocator& alloc = Allocator()) : past_the_end(&this->past_the_end, &this->past_the_end), _size(0), allocator(alloc) {
			this->insert(this->end(), count, value);
		}
		template< class InputIt >
		list( InputIt f, InputIt l,
			  const Allocator& alloc = Allocator()) : past_the_end(&this->past_the_end, &this->past_the_end), _size(0), allocator(alloc)
		{
			this->insert(this->end(), f, l);
		}
		list( const list& other ) : past_the_end(&this->past_the_end, &this->past_the_end), _size(0), allocator(other.allocator) {
			this->insert(this->end(), other.begin(), other.end());
		}
		~list()
		{
			this->clear();
		}
		list& operator=( const list& other ) {
			this->assign(other.begin(), other.end());
			return *this;
		}
		void assign( size_type count, const T& value ) {
			this->clear();
			this->insert(this->end(), count, value);
		}
		template< class InputIt >
		void assign( InputIt f, InputIt l )
		{
			this->clear();
			this->insert(this->end(), f, l);
		}

		allocator_type get_allocator() const { return this->allocator; }

		reference front() {return *this->begin();}

		const_reference front() const {return *this->begin();}

		reference back() {return *this->rbegin();}

		const_reference back() const {return *this->rbegin();}

		iterator begin() {return iterator(this->past_the_end.next);}

		const_iterator begin() const {return const_iterator(this->past_the_end.next);}

		iterator end() {return iterator(&this->past_the_end);}

		const_iterator end() const {return const_iterator(&this->past_the_end);}

		reverse_iterator rbegin() { return reverse_iterator(this->end()); }

		const_reverse_iterator rbegin() const { return const_reverse_iterator(this->end()); }

		reverse_iterator rend() { return reverse_iterator(this->begin()); }

		const_reverse_iterator rend() const { return const_reverse_iterator(this->begin()); }

		bool empty() const { return this->_size == 0; }

		size_type size() const { return this->_size; }

		size_type max_size() const { return this->allocator.max_size(); };

		void clear()
		{
			while (this->past_the_end.next != &this->past_the_end)
			{
				node_type_base *tmp = this->past_the_end.next;
				this->past_the_end.next = this->past_the_end.next->next;
				destroy_node(*tmp);
			}
			this->past_the_end.prev = &this->past_the_end;
		}

		iterator insert( const_iterator pos, const T& value )
		{
			return this->insert_node(pos, new_node(value));
		}

		void insert( const_iterator pos, size_type count, const T& value )
		{
			this->insert(pos, count, value, ft::true_type());
		}
	private:
		iterator insert( const_iterator pos, size_type count, const T& value, ft::true_type )
		{
			for (;count > 0; count--)
				pos = this->insert(pos, value);
			return pos;
		}

		template< class InputIt >
		iterator insert( const_iterator pos, InputIt f, InputIt l, ft::false_type )
		{
			if (f == l)
				return pos;
			iterator ret = this->insert(pos, *f);
			++f;
			for (;f != l; f++)
				this->insert(pos, *f);
			return ret;
		}
	public:
		template< class InputIt >
		void insert( const_iterator pos, InputIt f, InputIt l)
		{
			this->insert(pos, f, l, typename ft::is_integral<InputIt>::type());
		}

		iterator erase( iterator pos )
		{
			iterator ret(pos.current->next);
			pos.current->prev->next = pos.current->next;
			pos.current->next->prev = pos.current->prev;
			destroy_node(*pos.current);
			return ret;
		}

		iterator erase( iterator first, iterator last )
		{
			while (first != last)
				first = this->erase(first);
			return first;
		}

		void push_back( const T& value ) {this->insert(const_iterator(this->end()), value);}

		void pop_back() {this->erase(--this->end());}

		void push_front( const T& value ) {this->insert(this->begin(), value);}

		void pop_front() {this->erase(this->begin());}

		void resize( size_type count, T value = T() )
		{
			if (count > this->_size)
				this->insert(this->end(), count - this->_size, value);
			else
				while (this->_size > count)
					this->erase(--this->end());
		}

		void swap( list& other )
		{
			ft::swap(this->past_the_end.next, other.past_the_end.next);
			this->past_the_end.next->prev = &this->past_the_end;
			other.past_the_end.next->prev = &other.past_the_end;
			ft::swap(this->past_the_end.prev, other.past_the_end.prev);
			this->past_the_end.prev->next = &this->past_the_end;
			other.past_the_end.prev->next = &other.past_the_end;
			ft::swap(this->allocator, other.allocator);
			ft::swap(this->_size, other._size);
		}

		void merge( list& other ) {this->merge(other, ft::less<value_type>());}

		template < class Compare >
		void merge( list& other, Compare comp )
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
					this->insert_node(it, tmp.current);
				}
				else
					it++;
			}
			other.past_the_end.prev = &other.past_the_end;
			other.past_the_end.next = &other.past_the_end;
			this->_size += other.size();
			other._size = 0;
		}

		void splice( const_iterator pos, list& other )
		{
			this->splice(pos, other, other.begin(), other.end());
		}

		void splice( const_iterator pos, list& other, const_iterator it )
		{
			iterator next = it;
			++next;
			this->splice(pos, other, it, next);
		}

		void splice( const_iterator pos, list& other,
					 const_iterator f, const_iterator l)
		{
			if (f == l)
				return;
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

			node_type_base	*A = pos.current->prev,
							*B = pos.current,
							*C = f.current->prev,
							*D = f.current,
							*E = l.current->prev,
							*F = l.current;

			A->next = D;
			D->prev = A;
			E->next = B;
			B->prev = E;
			C->next = F;
			F->prev = C;
		}

		void remove( const T& value ) {return remove_if(ft::binder1st<equal_to<T> >(equal_to<T>(), value));}

		template< class UnaryPredicate >
		void remove_if( UnaryPredicate p )
		{
			for (iterator it = this->begin(); it != this->end();)
				if (p(*it))
					it = this->erase(it);
				else
					it++;
		}

		void reverse() {
			for (iterator it = this->begin(); it != this->end();)
			{
				iterator tmp = it;
				++it;
				ft::swap(tmp.current->next, tmp.current->prev);
			}
			ft::swap(past_the_end.next, past_the_end.prev);
		}

		void unique()
		{
			this->unique(equal_to<value_type>());
		}

		template< class BinaryPredicate >
		void unique( BinaryPredicate p )
		{
			for (iterator next = this->begin(), it = next++; next != this->end();)
				if (p(*it, *next))
					next = this->erase(next);
				else
				{
					it++;
					next++;
				}
		}

		void sort() {sort(ft::less<value_type>());}

// merge sort
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
							this->splice(g1, *this, g2);
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
			pos.current->prev->next = node;
			node->prev = pos.current->prev;

			pos.current->prev = node;
			node->next = pos.current;
			return iterator(node);
		}

		node_type *new_node(const node_type &val)
		{
			node_type *ret = this->allocator.allocate(1);
			this->allocator.construct(ret, val);
			this->_size++;
			return ret;
		}
		void	destroy_node(node_type_base &node)
		{
			this->allocator.destroy(static_cast<node_type*>(&node));
			this->allocator.deallocate(static_cast<node_type*>(&node), 1);
			this->_size--;
		}
	};

	template <class T, class Allocator>
	bool operator==(const list<T, Allocator> &lhs, const list<T, Allocator> &rhs) {
		return ((lhs.size() == rhs.size()) && ft::equal<typename list<T, Allocator>::const_iterator, typename list<T, Allocator>::const_iterator>(lhs.begin(), lhs.end(), rhs.begin()));
	}
	template <class T, class Allocator>
 	bool operator!=(const list<T, Allocator> &lhs, const list<T, Allocator> &rhs) {
		return (!(lhs == rhs));
	}
	template <class T, class Allocator>
 	bool operator<(const list<T, Allocator> &lhs, const list<T, Allocator> &rhs) {
		return (ft::lexicographical_compare<typename list<T, Allocator>::const_iterator, typename list<T, Allocator>::const_iterator>(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}
	template <class T, class Allocator>
	bool operator<=(const list<T, Allocator> &lhs, const list<T, Allocator> &rhs) {
		return (!(rhs < lhs));
	}
	template <class T, class Allocator>
	bool operator>(const list<T, Allocator> &lhs, const list<T, Allocator> &rhs) {
		return (rhs < lhs);
	}
	template <class T, class Allocator>
	bool operator>=(const list<T, Allocator> &lhs, const list<T, Allocator> &rhs) {
		return (!(lhs < rhs));
	}

	template< class T, class Alloc >
	void swap( ft::list<T, Alloc>& lhs,
			   ft::list<T, Alloc>& rhs)
	{
		lhs.swap(rhs);
	}
}
#endif