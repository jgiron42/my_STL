#ifndef FT_CONTAINERS_TYPE_TRAITS_HPP
#define FT_CONTAINERS_TYPE_TRAITS_HPP
#include <cstddef>
namespace ft {
	template<class T, T v>
	struct integral_constant {
		static const T value = v;
		typedef integral_constant type;
		typedef T value_type;

		operator value_type() const { return value; }
	};

	typedef integral_constant<bool, true> true_type;
	typedef integral_constant<bool, false> false_type;

	template<class T>
	struct is_integral : public false_type {};

	template<>
	struct is_integral<bool> : public true_type {};
	template<>
	struct is_integral<char> : public true_type {};
	template<>
	struct is_integral<wchar_t> : public true_type {};
	template<>
	struct is_integral<short> : public true_type {};
	template<>
	struct is_integral<int> : public true_type {};
	template<>
	struct is_integral<long> : public true_type {};
	template<>
	struct is_integral<long long> : public true_type {};

	template<>
	struct is_integral<unsigned char> : public true_type {};
	template<>
	struct is_integral<unsigned short> : public true_type {};
	template<>
	struct is_integral<unsigned int> : public true_type {};
	template<>
	struct is_integral<unsigned long> : public true_type {};
	template<>
	struct is_integral<unsigned long long> : public true_type {};

	template< class T > struct remove_reference      { typedef T type; };
	template< class T > struct remove_reference<T&>  { typedef T type; };

	template<bool B, class T = void>
	struct enable_if {};

	template<class T>
	struct enable_if<true, T> { typedef T type; };

	template<bool B, class T, class F>
	struct conditional { typedef T type; };

	template<class T, class F>
	struct conditional<false, T, F> { typedef F type; };

	template<class T> struct remove_pointer { typedef T type; };
	template<class T> struct remove_pointer<T*> { typedef T type; };
	template<class T> struct remove_pointer<T* const> { typedef T type; };
	template<class T> struct remove_pointer<T* volatile> { typedef T type; };
	template<class T> struct remove_pointer<T* const volatile> { typedef T type; };

}
#endif
