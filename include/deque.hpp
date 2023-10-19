#ifndef FT_CONTAINERS_deque_HPP
# define FT_CONTAINERS_deque_HPP
#include <memory>
#include <stdexcept.hpp>
#include <iostream>
#include "algorithm.hpp"
#include "utility.hpp"
#include "functional.hpp"
#include "iterator.hpp"
#include "type_traits.hpp"
#include <cstddef>

namespace ft {
	template<
			class T,
			class Allocator = std::allocator<T>
	>
	class deque {
	public:
		typedef T									value_type;
		typedef Allocator							allocator_type;
		typedef size_t								size_type;
		typedef ptrdiff_t							difference_type;
		typedef value_type							&reference;
		typedef const value_type					&const_reference;
		typedef typename Allocator::pointer 		pointer;
		typedef typename Allocator::const_pointer	const_pointer;
	private:

		template<typename pointed_type>
		struct iterator_t {
			typedef ptrdiff_t						difference_type;
			typedef pointed_type				value_type;
			typedef pointed_type					*pointer;
			typedef pointed_type					&reference;
			typedef ft::random_access_iterator_tag	iterator_category;

			pointer		*node;
			size_type	node_size;
			size_type	current;

			iterator_t() : node(NULL), node_size(0), current(0) {}

			explicit iterator_t(pointer *n, size_type n_s, size_type c) : node(n), node_size(n_s), current(c) {}

			template<typename U>
			iterator_t(iterator_t<U> const &other) : node((pointer *)other.node), node_size(other.node_size), current(other.current) {}

			iterator_t &operator++() {
				++current;
				if (current == node_size)
				{
					++node;
					current = 0;
				}
				return *this;
			}

			iterator_t operator++(int) {
				iterator_t ret = *this;
				++*this;
				return ret;
			}

			iterator_t &operator--() {
				if (current == 0)
				{
					--node;
					current = node_size;
				}
				current--;
				return *this;
			}

			iterator_t operator--(int) {
				iterator_t ret = *this;
				--*this;
				return ret;
			};

			reference operator*() {return (*node)[current]; }


			const_reference operator*() const { return (*node)[current]; }

			reference operator[](difference_type n) {
				return *(*this + n);
			}

			const_reference operator[](difference_type n) const {
				return *(*this + n);
			}

			iterator_t &operator+=(difference_type n)
			{
				this->node += n / (difference_type)this->node_size;
				n %= (difference_type)this->node_size;

				if ((difference_type)this->current + n >= (difference_type)this->node_size)
					this->node++;
				else if ((difference_type)this->current + n < 0)
					this->node--;

				this->current += n + this->node_size;
				this->current %= (difference_type)this->node_size;
				if (this->current < 0)
					this->current += this->node_size;
				return *this;
			}

			iterator_t &operator-=(difference_type n)
			{
				return this->operator+=(-n);
			}

			pointer operator->() const { return *node + current; }
			friend iterator_t operator+(iterator_t it, difference_type n)
			{
				it += n;
				return it;
			}
			friend iterator_t operator+(difference_type n, iterator_t it) {return it + n;}
			friend iterator_t operator-(iterator_t it, difference_type n) {return it + -n;}
		};
		public:
		template<typename LT, typename RT>
		friend difference_type operator-(iterator_t<LT> lhs, iterator_t<RT> rhs) {
			return ((typename iterator_t<LT>::pointer *)lhs.node - (typename iterator_t<LT>::pointer *)rhs.node) * lhs.node_size + (lhs.current - rhs.current);
		}
		template<typename LT, typename RT>
		friend bool operator==(iterator_t<LT> l, iterator_t<RT> r) { return l.node == r.node && l.current == r.current; }

		template<typename LT, typename RT>
		friend bool operator!=(iterator_t<LT> l, iterator_t<RT> r) { return l.node != r.node || l.current != r.current; }

		template<typename LT, typename RT>
		friend bool operator<(iterator_t<LT> l, iterator_t<RT> r) {
			return l.node < r.node || (l.node == r.node && l.current < r.current);
		}

		template<typename LT, typename RT>
		friend bool operator>(iterator_t<LT> l, iterator_t<RT> r) {
			return  r < l;
		}

		template<typename LT, typename RT>
		friend bool operator<=(iterator_t<LT> l, iterator_t<RT> r) {
			return  !(l > r);
		}

		template<typename LT, typename RT>
		friend bool operator>=(iterator_t<LT> l, iterator_t<RT> r) {
			return  !(l < r);
		}

		typedef iterator_t<value_type>					iterator;
		typedef iterator_t<const value_type>			const_iterator;
		typedef ft::reverse_iterator<iterator>			reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;
	private:
		pointer														*map;
		iterator													first;
		iterator													last;
		size_type													map_capacity;
		size_type													_size;
		allocator_type												allocator;
		typename allocator_type::template rebind<pointer>::other	map_allocator;
		static const size_type										node_size = 10;
	public:
		deque() : map(NULL), first(), last(), map_capacity(0), _size(0), allocator(), map_allocator() {
			this->init();
		}

		explicit deque( const Allocator& alloc ) : map(NULL), first(), last(), map_capacity(0), _size(0), allocator(alloc), map_allocator(alloc) {
			this->init();
		}

		explicit deque( size_type count,
					   const T& value = T(),
					   const Allocator& alloc = Allocator()) : map(NULL), first(), last(), map_capacity(0), _size(0), allocator(alloc), map_allocator(alloc) {
			this->init();
			this->insert(this->end(), count, value);
		}

		template< class InputIt >
		deque( InputIt f, InputIt l, const Allocator& alloc = Allocator()) : map(NULL), first(), last(), map_capacity(0), _size(0), allocator(alloc), map_allocator(alloc)
		{
			this->init();
			this->insert(this->end(), f, l);
		}

		deque( const deque& other ) : map(NULL), first(), last(), map_capacity(0), _size(0), allocator(other.allocator), map_allocator(other.allocator) {
			this->init();
			this->insert(this->end(), other.begin(), other.end());
		}

		~deque()
		{
			this->free();
		}

		deque& operator=( const deque& other ) {
			this->assign(other.begin(), other.end());
			return *this;
		}

		void assign( size_type count, const T& value ) {
			this->clear();
			this->insert(this->begin(), count, value);
		}

		template< class InputIt >
		void assign( InputIt f, InputIt l )
		{
			this->clear();
			this->insert(this->end(), f, l);
		}

		allocator_type get_allocator() const { return this->allocator; }

		reference operator[]( size_type pos ) { return this->first.operator[](pos); }

		const_reference operator[]( size_type pos ) const {return this->first.operator[](pos); }

		reference at( size_type pos ) {
			if (!(pos < this->_size))
				throw ft::out_of_range("pos is out of range");
			return (*this)[pos];
		}

		const_reference at( size_type pos ) const {
			if (!(pos < this->_size))
				throw ft::out_of_range("pos is out of range");
			return (*this)[pos];
		}

		reference front() {return *this->begin();}

		const_reference front() const {return *this->begin();}

		reference back() {return *this->rbegin();}

		const_reference back() const {return *this->rbegin();}

		iterator begin() {return this->first;}

		const_iterator begin() const {return const_iterator(this->first);}

		iterator end() {return this->last;}

		const_iterator end() const {return const_iterator(this->last);}

		reverse_iterator rbegin() { return reverse_iterator(this->end()); }

		const_reverse_iterator rbegin() const { return const_reverse_iterator(this->end()); }

		reverse_iterator rend() { return reverse_iterator(this->begin()); }

		const_reverse_iterator rend() const { return const_reverse_iterator(this->begin()); }

		bool empty() const { return this->_size == 0; }

		size_type size() const { return this->_size; }

		size_type max_size() const { return this->allocator.max_size(); };

		void clear()
		{
			this->erase(this->begin(), this->end());
		}

		iterator insert( const_iterator pos, const T& value)
		{
			return this->insert(pos, 1, value, ft::true_type());
		}

		void insert( const_iterator pos, size_type count, const T& value)
		{
			this->insert(pos, count, value, ft::true_type());
		}
	private:
		iterator insert( const_iterator pos, size_type count, const T& value, ft::true_type)
		{
			return this->insert(pos, generator_it<const T&>(value), generator_it<const T&>(count, value), ft::false_type());
		}

		template< class It >
		iterator insert( const_iterator pos, It f, It l, ft::random_access_iterator_tag)
		{
			size_t count = ft::distance(f, l);
			if (count == 0)
				return iterator((pointer *)pos.node, pos.node_size, pos.current);
			difference_type offset = pos - this->first;
			if ((size_type)(pos - this->begin()) > this->size() / 2)
			{
				if (count > (last.node_size - last.current - 1))
					this->add_nodes_back((count - (last.node_size - last.current - 1)) / node_size + ((count - (last.node_size - last.current - 1)) % node_size != 0));
				pos = this->first + offset;
				internal_insert(iterator(pos), f, l, this->end(), count);
				this->last += count;
			}
			else
			{
				if (count > first.current)
					this->add_nodes_front((count - first.current) / node_size + ((count - first.current) % node_size != 0));
				pos = this->first + offset;
				internal_insert(reverse_iterator(pos), ft::reverse_iterator<It>(l), ft::reverse_iterator<It>(f), this->rend(), count);
				this->first -= count;
			}
			return this->first + offset;
		}

		template< class It, class It2 >
		void internal_insert(It2 pos, It f, It l, It2 current_end, size_t count)
		{
			It2 new_end = current_end + count;
			It2 it1 = current_end - 1;
			It2 it2 = new_end - 1;
			while (it1 >= pos)
			{
				if (it2 < current_end)
					*it2 = *it1;
				else
					this->construct(it2.operator->(), 1, *it1);
				--it1;
				--it2;
			}
			it1 = pos;
			while (f != l)
			{
				if (it1 < current_end)
					*it1 = *f;
				else
					this->construct(it1.operator->(), 1, *f);
				it1++;
				f++;
			}
		}

		template< class InputIt >
		iterator insert( const_iterator pos, InputIt f, InputIt l, ft::input_iterator_tag)
		{
			size_type offset = pos - this->begin();
			for (;f != l; f++, pos++)
				pos = this->insert(pos, *f);
			return this->begin() + offset;
		}

		template< class InputIt >
		iterator insert( const_iterator pos, InputIt f, InputIt l, ft::false_type)
		{
			return this->insert(pos, f, l, typename ft::iterator_traits<InputIt>::iterator_category());
		}
	public:

		template< class InputIt >
		void insert( const_iterator pos, InputIt f, InputIt l)
		{
			this->insert(pos, f, l, typename ft::is_integral<InputIt>::type());
		}

		iterator erase( iterator pos )
		{
			return this->erase(pos, pos + 1);
		}

		iterator erase( iterator f, iterator l )
		{
			difference_type offset = f - this->begin();
			if (last - l < f - first)
			{
				iterator it1, it2;
				for (it1 = f, it2 = l; it2 < this->end(); it1++, it2++)
					*it1 = *it2;
				if (it2.node > it1.node) {
					this->destroy(it1.operator->(), (difference_type) it1.node_size - (difference_type) it1.current);
					this->erase_node(it1.node + 1, this->last.node + 1);
				} else
					this->destroy(it1.operator->(), it2 - it1);
				this->last = it1;
			}
			else
			{
				for (; f != this->first; --f, --l)
					l[-1] = f[-1];
				if (f.node != l.node)
				{
					this->destroy(*l.node, l.current);
					this->erase_node(f.node, l.node);
				}
				else
					this->destroy(*f.node + f.current, l - f);
				this->first = l;
			}
			return this->begin() + offset;
		}

		void push_back( const T& value ) {this->insert(const_iterator(this->end()), value);}

		void pop_back() {this->erase(this->end() - 1);}

		void push_front( const T& value ) {this->insert(const_iterator(this->begin()), value);}

		void pop_front() {this->erase(this->begin());}

		void resize( size_type count, T value = T() )
		{
			if (count > this->_size)
				this->insert(this->end(), count - this->_size, value);
			else
				this->erase(this->begin() + count, this->end());
		}

		void swap( deque& other )
		{
			ft::swap(this->map, other.map);
			ft::swap(this->first, other.first);
			ft::swap(this->last, other.last);
			ft::swap(this->map_allocator, other.map_allocator);
			ft::swap(this->_size, other._size);
			ft::swap(this->allocator, other.allocator);
			ft::swap(this->map_capacity, other.map_capacity);
		}

	private:
		void	init()
		{
			this->reserve_map(1);
		}
		void	reserve_map(size_type new_cap)
		{
			if (new_cap <= this->map_capacity)
				return;
			pointer *new_ptr = this->map_allocator.allocate(new_cap);
			for (size_t i = 0; i < new_cap; i++)
			{
				this->map_allocator.construct(new_ptr + i, pointer());
			}
			if (this->map)
			{
				for (size_t i = 0; i < this->map_capacity; i++)
				{
					if (this->first.node == this->map + i)
						this->first.node = new_ptr + ((new_cap - this->map_capacity) / 2) + i;
					if (this->last.node == this->map + i)
						this->last.node = new_ptr + ((new_cap - this->map_capacity) / 2) + i;
					new_ptr[(new_cap - this->map_capacity) / 2 + i] =
							this->map[i];
				}
				for (size_t i = 0; i < this->map_capacity; i++)
					this->map_allocator.destroy(this->map + i);
				this->map_allocator.deallocate(this->map, 1);
			}
			else
			{
				this->first = iterator(new_ptr + new_cap / 2, node_size, node_size / 2);
				this->last = this->first;
				*(this->first.node) = this->allocator.allocate(node_size);
			}
			this->map_capacity = new_cap;
			this->map = new_ptr;
		}
		void	add_nodes_back(difference_type count)
		{
			if (this->map + this->map_capacity - (this->last.node + 1) < count)
				this->reserve_map(this->map_capacity + count * 2);

			difference_type last_node = this->last.node - this->map;
			while (this->map[last_node]) {
				last_node++;
			}
			for (difference_type i = 0; i < count; i++)
			{
				this->map[last_node + i] = this->allocator.allocate(this->node_size);
			}
		}
		void	add_nodes_front(difference_type count)
		{
 			if (this->first.node - this->map < count)
				this->reserve_map(this->map_capacity + count * 2);

			difference_type first_node = this->first.node - this->map;
			while (this->map[first_node]) {
				first_node--;
			}
			for (difference_type i = 0; i < count; i++)
			{
				this->map[first_node - i] = this->allocator.allocate(this->node_size);
			}
		}
		void	free_map()
		{
			for (size_t i = 0; i < this->map_capacity; i++)
				this->map_allocator.destroy(this->map + i);
			this->map_allocator.deallocate(this->map, this->map_capacity);
			this->map = NULL;
		}
		void destroy(pointer p, size_type n) {
			for (size_type i = 0; i < n; i++)
				this->allocator.destroy(p + i);
			this->_size -= n;
		}
		void construct(pointer p, size_type n, const T& value)
		{
			for (size_type i = 0; i < n; i++)
				this->allocator.construct(p + i, value);
			this->_size += n;
		}
		void	free_node(pointer *n)
		{
			if (this->last.node == n)
				this->destroy(*n, this->last.current);
			else if (this->first.node == n)
				this->destroy(*n + this->first.current, this->first.node_size - this->first.current);
			else
				this->destroy(*n, node_size);
			this->allocator.deallocate(*n, node_size);
			*n = NULL;
		}
		void	erase_node(pointer *f, pointer *l)
		{
			for (pointer *n = f; n != l; n++)
				free_node(n);
			if (this->last.node - l < f - this->first.node)
				move_nodes(f, l, this->last.node - l + 1);
			else
				move_nodes(this->first.node + (l - f), this->first.node, f - this->first.node);
		}
		void	move_nodes(pointer *dst, pointer *src, size_type n)
		{
			if (dst < src)
				for (size_type i = 0; i < n; i++)
				{
					if (this->last.node == src + i)
						this->last.node = dst + i;
					if (this->first.node == src + i)
						this->first.node = dst + i;
					dst[i] = src[i];
					src[i] = NULL;
				}
			else if (dst > src)
				for (size_type i = n; i > 0; i--)
				{
					if (this->last.node == src + i - 1)
						this->last.node = dst + i - 1;
					if (this->first.node == src + i - 1)
						this->first.node = dst + i - 1;
					dst[i - 1] = src[i - 1];
					src[i - 1] = NULL;
				}
		}
		void free() {
			this->clear();
			this->allocator.deallocate(*this->last.node, this->node_size);
			this->free_map();
		}
	};

	template <class T, class Allocator>
	bool operator==(const deque<T, Allocator> &lhs, const deque<T, Allocator> &rhs) {
		return ((lhs.size() == rhs.size()) && ft::equal<typename deque<T, Allocator>::const_iterator, typename deque<T, Allocator>::const_iterator>(lhs.begin(), lhs.end(), rhs.begin()));
	}
	template <class T, class Allocator>
 	bool operator!=(const deque<T, Allocator> &lhs, const deque<T, Allocator> &rhs) {
		return (!(lhs == rhs));
	}
	template <class T, class Allocator>
 	bool operator<(const deque<T, Allocator> &lhs, const deque<T, Allocator> &rhs) {
		return (ft::lexicographical_compare<typename deque<T, Allocator>::const_iterator, typename deque<T, Allocator>::const_iterator>(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}
	template <class T, class Allocator>
	bool operator<=(const deque<T, Allocator> &lhs, const deque<T, Allocator> &rhs) {
		return (!(rhs < lhs));
	}
	template <class T, class Allocator>
	bool operator>(const deque<T, Allocator> &lhs, const deque<T, Allocator> &rhs) {
		return (rhs < lhs);
	}
	template <class T, class Allocator>
	bool operator>=(const deque<T, Allocator> &lhs, const deque<T, Allocator> &rhs) {
		return (!(lhs < rhs));
	}

	template< class T, class Alloc >
	void swap( ft::deque<T, Alloc>& lhs,//			std::cout  << last_node << std::

			   ft::deque<T, Alloc>& rhs)
	{
		lhs.swap(rhs);
	}
}
#endif