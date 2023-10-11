#ifndef FT_CONTAINERS_ALGORITHM_HPP
# define FT_CONTAINERS_ALGORITHM_HPP
#include "functional.hpp"
#include "iterator.hpp"
namespace ft
{
	template< class T >
	void swap( T& a, T& b )
	{
		T tmp = a;
		a = b;
		b = tmp;
	}


	template<class InputIt1, class InputIt2>
	bool lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2)
	{
		for ( ; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2 ) {
			if (*first1 < *first2) return true;
			if (*first2 < *first1) return false;
		}
		return (first1 == last1) && (first2 != last2);
	}

	template<class InputIt1, class InputIt2, class Compare>
	bool lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, Compare comp)
	{
		for ( ; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2 ) {
			if (comp(*first1, *first2)) return true;
			if (comp(*first2, *first1)) return false;
		}
		return (first1 == last1) && (first2 != last2);
	}

	template<class InputIt1, class InputIt2>
	bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2)
	{
		for (; first1 != last1; ++first1, ++first2) {
			if (!(*first1 == *first2)) {
				return false;
			}
		}
		return (first1 == last1);
	}

	template< class RandomIt >
	void push_heap(RandomIt first, RandomIt last )
	{
		return push_heap(first, last, ft::less<typename RandomIt::value_type>());
	}

	template< class RandomIt, class Compare >
	void push_heap(RandomIt first, RandomIt last, Compare comp)
	{
		if (first == last)
			return ;
		typename ft::iterator_traits<RandomIt>::difference_type pos = last - first - 1;
		typename ft::iterator_traits<RandomIt>::difference_type parent = (pos - 1) / 2;

		while (pos > 0 && comp(first[parent], first[pos]))
		{
			ft::swap(first[parent], first[pos]);
			pos = parent;
			parent = (pos - 1) / 2;
		}
	}

	template< class RandomIt >
	void pop_heap(RandomIt first, RandomIt last )
	{
		pop_heap(first, last, ft::less<typename RandomIt::value_type>());
	}

	template< class RandomIt, class Compare >
	void	heapify(RandomIt first, RandomIt last, typename ft::iterator_traits<RandomIt>::difference_type pos, Compare comp)
	{
		typename ft::iterator_traits<RandomIt>::difference_type left = (pos + 1) * 2;
		typename ft::iterator_traits<RandomIt>::difference_type right = (pos + 1) * 2 - 1;

		while (	((left < last - first) && (comp(first[pos], first[left]))) ||
				   ((right < last - first) && comp(first[pos], first[right])))
		{
			if ((left >= last - first) || comp(first[left], first[right]))
			{
				ft::swap(first[pos], first[right]);
				pos = right;
			}
			else
			{
				ft::swap(first[pos], first[left]);
				pos = left;
			}
			left = (pos + 1) * 2;
			right = (pos + 1) * 2 - 1;
		}
	}

	template< class RandomIt, class Compare >
	void /*Salut, c'est*/pop_heap/*de Stupeflip ! Eh ben moi, tous les matins, j'écoute 72.8 !*/(RandomIt first, RandomIt last, Compare comp)
	{
		if (first == last)
			return ;
		ft::swap(*first, *(last - 1));
		heapify(first, last - 1, 0, comp);
	}

	template< class RandomIt >
	void make_heap(RandomIt first, RandomIt last)
	{
		make_heap(first, last, ft::less<typename RandomIt::value_type>());
	}

	template< class RandomIt, class Compare >
	void make_heap( RandomIt first, RandomIt last, Compare comp ) {

		typename ft::iterator_traits<RandomIt>::difference_type pos = 0;
		typename ft::iterator_traits<RandomIt>::difference_type size = last - first;

		while (pos < size)
		{
			typename ft::iterator_traits<RandomIt>::difference_type left = pos * 2 + 2;
			typename ft::iterator_traits<RandomIt>::difference_type right = pos * 2 + 1;
			typename ft::iterator_traits<RandomIt>::difference_type parent = (pos - 1) / 2;
			if (left < size && comp(first[pos], first[left]))
				ft::swap(first[pos], first[left]);
			if (right < size && comp(first[pos], first[right]))
				ft::swap(first[pos], first[right]);
			if (parent >= 0 && comp(first[parent], first[pos]))
				pos = parent;
			else
				pos++;
		}
//		typename ft::iterator_traits<RandomIt>::difference_type pos = last - first;
//		while (pos >= 0) {
//			typename ft::iterator_traits<RandomIt>::difference_type left = (pos + 1) * 2;
//			typename ft::iterator_traits<RandomIt>::difference_type right = (pos + 1) * 2 - 1;
//
//			if (((left < last - first) && (!comp(first[pos], first[left]))) ||
//				((right < last - first) && !comp(first[pos], first[right]))) {
//				heapify(first, last, pos, comp);
//			}
//			pos--;
//		}
	}

	template< class InputIt, class T >
	InputIt find( InputIt first, InputIt last, const T& value )
	{
		for (;first != last; first++)
			if (*first == value)
				return first;
		return last;
	}

	template< class InputIt, class UnaryPredicate >
	InputIt find_if( InputIt first, InputIt last, UnaryPredicate p )
	{
		for (;first != last; first++)
			if (p(*first))
				return first;
		return last;
	}

	template< class InputIt, class UnaryPredicate >
	InputIt find_if_not( InputIt first, InputIt last, UnaryPredicate q )
	{
		for (;first != last; first++)
			if (!p(*first))
				return first;
		return last;
	}

	template< class InputIt, class UnaryPredicate >
	typename iterator_traits<InputIt>::difference_type
	count_if( InputIt first, InputIt last, UnaryPredicate p )
	{
		typename iterator_traits<InputIt>::difference_type ret = 0;

		for (;first != last; first++)
			if (p(*first))
				ret++;
		return ret;

	}

	template< class InputIt, class T >
	typename iterator_traits<InputIt>::difference_type count( InputIt first, InputIt last, const T& value )
	{
		return count_if(first, last, ft::binder1st<ft::equal_to<T> >(value));
	}

	template< class ForwardIt1, class ForwardIt2, class BinaryPredicate >
	bool is_permutation( ForwardIt1 first1, ForwardIt1 last1,
						 ForwardIt2 first2, BinaryPredicate p )
	{
		ForwardIt2 last2 = ft::next(first2, ft::distance(first1, last1));
		while (first1 != last1 && p(*first1, *first2))
		{
			first1++;
			first2++;
		}
		ForwardIt1 it = first1;
		while (it != last1)
		{
			if (ft::count_if(first2, last2, ft::binder1st<BinaryPredicate>(*it)) !=
				ft::count_if(first1, last1, ft::binder1st<BinaryPredicate>(*it)))
				return false;
			it++;
		}
		return true;
	}

	template< class ForwardIt1, class ForwardIt2 >
	bool is_permutation( ForwardIt1 first1, ForwardIt1 last1,
						 ForwardIt2 first2 )
	{
		return is_permutation(first1, last1, first2, ft::equal_to<typename ft::iterator_traits<ForwardIt1>::value_type>());
	}

	template< class T >
	const T& max( const T& a, const T& b )
	{
		return b < a ? a : b;
	}
	template< class T, class Compare >
	const T& max( const T& a, const T& b, Compare comp )
	{
		return comp(b, a) ? a : b;
	}

	template< class T >
	const T& min( const T& a, const T& b )
	{
		return a < b ? a : b;
	}

	template< class T, class Compare >
	const T& min( const T& a, const T& b, Compare comp )
	{
		return comp(a, b) ? a : b;
	}
}

#endif
