#ifndef FT_CONTAINERS_vector_HPP
# define FT_CONTAINERS_vector_HPP
#include <memory>
#include <stdexcept.hpp>
#include <iostream>
#include "algorithm.hpp"
//#include "utility.hpp"
#include "functional.hpp"
#include "iterator.hpp"
#include "type_traits.hpp"

namespace ft {
	template<
			class T,
			class Allocator = std::allocator<T>
	>
	class vector {
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
	public:
		typedef pointer									iterator;
		typedef const value_type *						const_iterator;
		typedef ft::reverse_iterator<iterator>			reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;
	private:
		pointer			data;
		size_type		_size;
		size_type		_capacity;
		allocator_type	allocator;
	public:
		vector() : data(NULL), _size(0), _capacity(0), allocator() {}
		explicit vector( const Allocator& alloc ) : data(NULL), _size(0), _capacity(0), allocator(alloc) {}
		explicit vector( size_type count,
					   const T& value = T(),
					   const Allocator& alloc = Allocator()) : data(NULL), _size(0), _capacity(0), allocator(alloc) {
			this->insert(this->end(), count, value);
		}
		template< class InputIt >
		vector( InputIt f, InputIt l, const Allocator& alloc = Allocator()) : data(NULL), _size(0), _capacity(0), allocator(alloc)
		{
			this->insert(this->end(), f, l);
		}
		vector( const vector& other ) : data(NULL), _size(0), _capacity(0), allocator(other.allocator) {
			this->insert(this->end(), other.begin(), other.end());
		}
		~vector()
		{
			this->clear();
			this->free();
		}
		vector& operator=( const vector& other ) {
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

		reference operator[]( size_type pos ) {return this->data[pos];}

		const_reference operator[]( size_type pos ) const {return this->data[pos];}

		reference at( size_type pos ) {
			if (!(pos < this->_size))
				throw ft::out_of_range("pos is out of range");
			return this->data[pos];
		}

		const_reference at( size_type pos ) const {
			if (!(pos < this->_size))
				throw ft::out_of_range("pos is out of range");
			return this->data[pos];
		}

		reference front() {return *this->begin();}

		const_reference front() const {return *this->begin();}

		reference back() {return *this->rbegin();}

		const_reference back() const {return *this->rbegin();}

		iterator begin() {return iterator(this->data);}

		const_iterator begin() const {return const_iterator(this->data);}

		iterator end() {return iterator(this->data + this->_size);}

		const_iterator end() const {return const_iterator(this->data + this->_size);}

		reverse_iterator rbegin() { return reverse_iterator(this->end()); }

		const_reverse_iterator rbegin() const { return const_reverse_iterator(this->end()); }

		reverse_iterator rend() { return reverse_iterator(this->begin()); }

		const_reverse_iterator rend() const { return const_reverse_iterator(this->begin()); }

		bool empty() const { return this->_size == 0; }

		size_type size() const { return this->_size; }

		size_type max_size() const { return this->allocator.max_size(); };

		void reserve( size_type new_cap )
		{
			if (new_cap <= this->_capacity)
				return;
			pointer tmp = this->allocator.allocate(new_cap);
			for (size_type i = 0; i < this->_size; i++)
				this->allocator.construct(tmp + i, this->data[i]);
			this->destroy(this->data, this->_size);
			this->free();
			this->data = tmp;
			this->_capacity = new_cap;
		}

		size_type capacity() const {return this->_capacity;}

		void clear()
		{
			this->destroy(this->data, this->_size);
			this->_size = 0;
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
			size_type offset = pos - this->data;
			this->make_room((pointer)pos, count);
			pos = this->data + offset;
			for (size_type i = 0; i < count; i++)
			{
				if (pos + i >= this->data + this->_size)
					this->construct((pointer)pos + i, 1, value);
				else
					((pointer)pos)[i] = value;
			}
			this->_size += count;
			return (iterator)pos;
		}

		template< class It >
		iterator insert( const_iterator pos, It f, It l, ft::random_access_iterator_tag)
		{
			size_type offset = pos - this->data;
			this->make_room((pointer)pos, l - f);
			pointer p = this->data + offset;
			for (size_type i = 0; i + f < l; i++)
			{
				if (p + i >= this->data + this->_size)
					this->construct(p + i, 1, f[i]);
				else
					p[i] = f[i];
			}
			this->_size += l - f;
			return p;
		}

		template< class InputIt >
		iterator insert( const_iterator pos, InputIt f, InputIt l, ft::input_iterator_tag)
		{
			size_type offset = pos - this->data;
			for (;f != l; f++, pos++)
				pos = this->insert(pos, *f);
			return this->data + offset;
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

		iterator erase( iterator first, iterator last )
		{
			this->move(first, last, this->end() - last, false);
			this->destroy(this->end() - (last - first), last - first);
			this->_size -= last - first;
			return first;
		}

		void push_back( const T& value ) {this->insert(const_iterator(this->end()), value);}

		void pop_back() {this->erase(this->end() - 1);}

		void resize( size_type count, T value = T() )
		{
			if (count > this->_size)
				this->insert(this->end(), count - this->_size, value);
			else
				this->erase(this->begin() + count, this->end());
		}

		void swap( vector& other )
		{
			ft::swap(this->data, other.data);
			ft::swap(this->allocator, other.allocator);
			ft::swap(this->_size, other._size);
			ft::swap(this->_capacity, other._capacity);
		}

	private:
		void destroy(pointer p, size_type n) {
			for (size_type i = 0; i < n; i++)
				this->allocator.destroy(p + i);
		}
		void construct(pointer p, size_type n, const T& value)
		{
			for (size_type i = 0; i < n; i++)
				this->allocator.construct(p + i, value);
		}
		void make_room(pointer p, size_type count)
		{
			size_t offset = p - this->data;
			this->reserve_for(count);
			p = this->data + offset;
			this->move(p + count, p, (this->data + this->_size) - p);
		}
		void move(pointer dst, pointer src, size_type count, bool destroy = false)
		{
			if (dst < src)
				for (size_type i = 0; i < count; i++)
					this->move_one(dst + i, src + i, destroy);
			else
				for (size_type i = count; i > 0; i--)
					this->move_one(dst + i - 1, src + i - 1, destroy);
		}
		void move_one(pointer dst, pointer src, bool destroy = false)
		{
			if (dst >= this->data + this->_size)
			{
				this->allocator.construct(dst, *src);
				if (destroy)
					this->allocator.destroy(src);
			}
			else
			{
				*dst = *src;
				if (destroy)
					this->allocator.destroy(src);
			}
		}
		void free() {
			this->allocator.deallocate(this->data, this->_capacity);
			this->data = NULL;
			this->_capacity = 0;
		}
		void reserve_for(size_type n)
		{
			size_type new_cap = this->_capacity;
			if (!new_cap)
				new_cap = 1;
			while (this->_size + n > new_cap)
				new_cap *= 2;
			this->reserve(new_cap);
		}
	};

	template <class T, class Allocator>
	bool operator==(const vector<T, Allocator> &lhs, const vector<T, Allocator> &rhs) {
		return ((lhs.size() == rhs.size()) && ft::equal<typename vector<T, Allocator>::const_iterator, typename vector<T, Allocator>::const_iterator>(lhs.begin(), lhs.end(), rhs.begin()));
	}
	template <class T, class Allocator>
 	bool operator!=(const vector<T, Allocator> &lhs, const vector<T, Allocator> &rhs) {
		return (!(lhs == rhs));
	}
	template <class T, class Allocator>
 	bool operator<(const vector<T, Allocator> &lhs, const vector<T, Allocator> &rhs) {
		return (ft::lexicographical_compare<typename vector<T, Allocator>::const_iterator, typename vector<T, Allocator>::const_iterator>(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}
	template <class T, class Allocator>
	bool operator<=(const vector<T, Allocator> &lhs, const vector<T, Allocator> &rhs) {
		return (!(rhs < lhs));
	}
	template <class T, class Allocator>
	bool operator>(const vector<T, Allocator> &lhs, const vector<T, Allocator> &rhs) {
		return (rhs < lhs);
	}
	template <class T, class Allocator>
	bool operator>=(const vector<T, Allocator> &lhs, const vector<T, Allocator> &rhs) {
		return (!(lhs < rhs));
	}

	template< class T, class Alloc >
	void swap( ft::vector<T, Alloc>& lhs,
			   ft::vector<T, Alloc>& rhs)
	{
		lhs.swap(rhs);
	}
}
#endif