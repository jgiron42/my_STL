#ifndef FT_CONTAINERS_PRIORITY_QUEUE_HPP
# define FT_CONTAINERS_PRIORITY_QUEUE_HPP
#include <memory>
#include <stdexcept>
#include <iostream>
#include "algorithm.hpp"
#include "utility.hpp"
#include "functional.hpp"
#include "iterator.hpp"
#include "type_traits.hpp"
#include "deque.hpp"
#include "vector.hpp"

namespace ft {

	template<
			class T,
			class Container = ft::deque<T>
	>
	class queue {
	public:
		typedef Container					container_type;
		typedef typename Container::value_type		value_type;
		typedef typename Container::size_type		size_type;
		typedef typename Container::reference		reference;
		typedef typename Container::const_reference	const_reference;
	protected:
		Container c;
	public:
		explicit queue( const Container& cont = Container() ) : c(cont) {}
		queue( const queue& other ) : c(other.c) {}
		queue& operator=( const queue& other )
		{
			this->c = other.c;
			return *this;
		}
		reference front() {return this->c.front();}

		const_reference front() const {return this->c.front();}

		reference back() {return this->c.back();}

		const_reference back() const {return this->c.back();}

		bool empty() const {return this->c.empty();}

		size_type size() const {return this->c.size();}

		void push( const value_type& value ) {this->c.push_back(value);}

		void pop() {this->c.pop_front();}

		friend bool operator==(const queue &lhs, const queue &rhs) {
			return (lhs.c == rhs.c);
		}
		friend bool operator!=(const queue &lhs, const queue &rhs) {
			return (lhs.c != rhs.c);
		}
		friend bool operator<(const queue &lhs, const queue &rhs) {
			return (lhs.c < rhs.c);
		}
		friend bool operator<=(const queue &lhs, const queue &rhs) {
			return (lhs.c <= rhs.c);
		}
		friend bool operator>(const queue &lhs, const queue &rhs) {
			return (lhs.c > rhs.c);
		}
		friend bool operator>=(const queue &lhs, const queue &rhs) {
			return (lhs.c >= rhs.c);
		}
	};


	template<
			class T,
			class Container = ft::vector<T>,
			class Compare = ft::less<typename Container::value_type>
	>
	class priority_queue {
	public:
		typedef Container							container_type;
		typedef typename Container::value_type		value_type;
		typedef typename Container::size_type		size_type;
		typedef typename Container::reference		reference;
		typedef typename Container::const_reference	const_reference;
		typedef Compare								value_compare;
	protected:
	public:
		Compare		comp;
		Container	c;
	public:
		explicit priority_queue( const Compare& compare = Compare(), const Container& cont = Container() ) : comp(compare), c(cont) {ft::make_heap(c.begin(), c.end(), comp);}

		priority_queue( const priority_queue& other ) : comp(other.comp), c(other.c) {}

		template< class InputIt >
		priority_queue( InputIt first, InputIt last,
						const Compare& compare = Compare(),
						const Container& cont = Container() )  : comp(compare), c(cont) {
			c.insert(c.end(), first, last);
			ft::make_heap(c.begin(), c.end(), comp);
		}

		priority_queue& operator=( const priority_queue& other ) {
			this->comp = other.comp;
			this->c = other.c;
			return *this;
		}

		const_reference top() const {return this->c.front();}

		bool empty() const {return this->c.empty();}

		size_type size() const {return this->c.size();}

		void push( const value_type& value ) {
			c.push_back(value);
			ft::push_heap(c.begin(), c.end(), comp);
		}

		void pop() {
			ft::pop_heap(c.begin(), c.end(), comp);
			c.pop_back();
		}
	};
}
#endif