#ifndef FT_CONTAINERS_ITERATOR_HPP
# define FT_CONTAINERS_ITERATOR_HPP

#include <cstddef>
#include <iterator>
#include "type_traits.hpp"

namespace ft {
	typedef std::input_iterator_tag input_iterator_tag;
	typedef std::output_iterator_tag output_iterator_tag;
	typedef std::forward_iterator_tag forward_iterator_tag;
	typedef std::bidirectional_iterator_tag bidirectional_iterator_tag;
	typedef std::random_access_iterator_tag random_access_iterator_tag;

	template<class Iter>
	struct iterator_traits {
		typedef typename Iter::difference_type difference_type;
		typedef typename Iter::value_type value_type;
		typedef typename Iter::pointer pointer;
		typedef typename Iter::reference reference;
		typedef typename Iter::iterator_category iterator_category;
	};

	template<class T>
	struct iterator_traits<T *> {
		typedef ptrdiff_t difference_type;
		typedef T value_type;
		typedef T *pointer;
		typedef T &reference;
		typedef ft::random_access_iterator_tag iterator_category;
	};

	template<class T>
	struct iterator_traits<const T *> {
		typedef ptrdiff_t difference_type;
		typedef T value_type;
		typedef const T *pointer;
		typedef const T &reference;
		typedef ft::random_access_iterator_tag iterator_category;
	};

	template<class Iter>
	class pointer_iterator : public Iter {

		typename Iter::reference operator*() const {
			return *this->Iter::operator*();
		}

		typename Iter::pointer operator->() const {
			return (&**this);
		}

	};

	template<class Iter>
	class reverse_iterator {
	public:
		typedef Iter iterator_type;
		typedef typename iterator_traits<Iter>::iterator_category iterator_category;
		typedef typename iterator_traits<Iter>::value_type value_type;
		typedef typename iterator_traits<Iter>::difference_type difference_type;
		typedef typename iterator_traits<Iter>::pointer pointer;
		typedef typename iterator_traits<Iter>::reference reference;

		reverse_iterator() : current() {}

		explicit reverse_iterator(iterator_type x) : current(x) {}

		template<class U>
		reverse_iterator(const reverse_iterator<U> &other) : current(other.base()) {}

		template<class U>
		reverse_iterator &operator=(const reverse_iterator<U> &other) {
			this->current = other.base();
			return (*this);
		}

//		reverse_iterator& operator=( const reverse_iterator<U>& other ) {return(this->current = other.base());}
		iterator_type base() const { return (this->current); }

		reference operator*() const {
			Iter ret = this->current;
			return (*--ret);
		}

		pointer operator->() const {
			Iter ret = this->current;
			return (&(*--ret));
		}

		reference operator[](difference_type n) const {
			return (base()[-n - 1]);
		}

		reverse_iterator &operator++() {
			--current;
			return (*this);
		}

		reverse_iterator &operator--() {
			++current;
			return (*this);
		}

		reverse_iterator operator++(int) {
			reverse_iterator tmp(current);
			current--;
			return (tmp);
		}

		reverse_iterator operator--(int) {
			reverse_iterator tmp(current);
			current++;
			return (tmp);
		}

		reverse_iterator operator+(difference_type n) const {
			return (reverse_iterator(current - n));
		}

		reverse_iterator operator-(difference_type n) const {
			return (reverse_iterator(current + n));
		}

		reverse_iterator &operator+=(difference_type n) {
			current -= n;
			return (*this);
		}

		reverse_iterator &operator-=(difference_type n) {
			current += n;
			return (*this);
		}

	protected:
		iterator_type current;
	};

	template<class Iterator1, class Iterator2>
	bool operator==(const ft::reverse_iterator<Iterator1> &lhs, const ft::reverse_iterator<Iterator2> &rhs) {
		return (lhs.base() == rhs.base());
	}

	template<class Iterator1, class Iterator2>
	bool operator!=(const ft::reverse_iterator<Iterator1> &lhs, const ft::reverse_iterator<Iterator2> &rhs) {
		return (lhs.base() != rhs.base());
	}

	template<class Iterator1, class Iterator2>
	bool operator<(const ft::reverse_iterator<Iterator1> &lhs, const ft::reverse_iterator<Iterator2> &rhs) {
		return (lhs.base() > rhs.base());
	}

	template<class Iterator1, class Iterator2>
	bool operator<=(const ft::reverse_iterator<Iterator1> &lhs, const ft::reverse_iterator<Iterator2> &rhs) {
		return (lhs.base() >= rhs.base());
	}

	template<class Iterator1, class Iterator2>
	bool operator>(const ft::reverse_iterator<Iterator1> &lhs, const ft::reverse_iterator<Iterator2> &rhs) {
		return (lhs.base() < rhs.base());
	}

	template<class Iterator1, class Iterator2>
	bool operator>=(const ft::reverse_iterator<Iterator1> &lhs, const ft::reverse_iterator<Iterator2> &rhs) {
		return (lhs.base() <= rhs.base());
	}

	template<class Iter>
	reverse_iterator<Iter>
	operator+(typename reverse_iterator<Iter>::difference_type n, const reverse_iterator<Iter> &it) {
		return (reverse_iterator<Iter>(it.base() - n));
	}

	template<class L, class R>
	typename reverse_iterator<L>::difference_type
	operator-(const reverse_iterator<L> &lhs, const reverse_iterator<R> &rhs) {
		return (rhs.base() - lhs.base());
	}

	template<class It>
	typename ft::iterator_traits<It>::difference_type
	do_distance(It first, It last, ft::input_iterator_tag) {
		typename ft::iterator_traits<It>::difference_type result = 0;
		while (first != last) {
			++first;
			++result;
		}
		return result;
	}

	template<class It>
	typename ft::iterator_traits<It>::difference_type
	do_distance(It first, It last, ft::random_access_iterator_tag) {
		return last - first;
	}


	template<class It>
	typename ft::iterator_traits<It>::difference_type
	distance(It first, It last) {
		return do_distance(first, last, typename ft::iterator_traits<It>::iterator_category());
	}

	template <typename T>
	struct generator_it
	{
		typedef ptrdiff_t									difference_type;
		typedef const T										value_type;
		typedef typename remove_reference<value_type>::type	*pointer;
		typedef typename remove_reference<value_type>::type	&reference;
		typedef ft::random_access_iterator_tag				iterator_category;
		difference_type pos;
		value_type		value;

		generator_it(value_type v) : pos(0), value(v) {}
		generator_it(difference_type p, value_type v) : pos(p), value(v) {}
		generator_it &operator++() {
			pos++;
			return *this;
		}
		generator_it operator++(int) {
			generator_it ret = *this;
			++*this;
			return ret;
		}
		generator_it &operator--() {
			pos--;
			return *this;
		}
		generator_it operator--(int) {
			generator_it ret = *this;
			--*this;
			return ret;
		}

		reference operator*() {return this->value;}

		pointer operator->() const {return &this->value;}

		reference operator[](difference_type) {
			return this->value;
		}

		generator_it &operator+=(difference_type n)
		{
			this->pos += n;
			return *this;
		}
		generator_it &operator-=(difference_type n)
		{
			this->pos += n;
			return *this;
		}

	};
	template <typename T>
	generator_it<T> operator+(generator_it<T> it, typename generator_it<T>::difference_type n)
	{
		it += n;
		return it;
	}

	template <typename T>
	generator_it<T> operator+(typename generator_it<T>::difference_type n, generator_it<T> it) {return it + n;}

	template <typename T>
	generator_it<T> operator-(generator_it<T> it, typename generator_it<T>::difference_type n) {return it + -n;}

	template<typename LT, typename RT>
	typename generator_it<LT>::difference_type operator-(generator_it<LT> lhs, generator_it<RT> rhs) {
		return lhs.pos - rhs.pos;
	}
	template<typename LT, typename RT>
	bool operator==(generator_it<LT> l, generator_it<RT> r) { return l.pos == r.pos; }

	template<typename LT, typename RT>
	bool operator!=(generator_it<LT> l, generator_it<RT> r) { return l.pos != r.pos; }

	template<typename LT, typename RT>
	bool operator<(generator_it<LT> l, generator_it<RT> r) {
		return l.pos < r.pos;
	}

	template<typename LT, typename RT>
	bool operator>(generator_it<LT> l, generator_it<RT> r) {
		return  r < l;
	}

	template<typename LT, typename RT>
	bool operator<=(generator_it<LT> l, generator_it<RT> r) {
		return  !(l > r);
	}

	template<typename LT, typename RT>
	bool operator>=(generator_it<LT> l, generator_it<RT> r) {
		return  !(l < r);
	}

	template< class InputIt, class Distance >
	void internal_advance( InputIt& it, Distance n, ft::bidirectional_iterator_tag)
	{
		while (n < 0)
		{
			--it;
			n++;
		}
		while (n > 0)
		{
			++it;
			n--;
		}
	}

	template< class InputIt, class Distance >
	void internal_advance( InputIt& it, Distance n, ft::input_iterator_tag)
	{
		while (n > 0)
		{
			++it;
			n--;
		}
	}

	template< class InputIt, class Distance >
	void advance( InputIt& it, Distance n )
	{
		internal_advance(it, n, typename iterator_traits<InputIt>::iterator_category());
	}

	template<class InputIt>
	InputIt next(InputIt it, typename iterator_traits<InputIt>::difference_type n = 1)
	{
		ft::advance(it, n);
		return it;
	}

	template<class BidirIt>
	BidirIt prev(BidirIt it, typename iterator_traits<BidirIt>::difference_type n = 1)
	{
		ft::advance(it, -n);
		return it;
	}
}

#endif