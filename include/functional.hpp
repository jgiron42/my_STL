#ifndef FT_CONTAINERS_FUNCTIONAL_HPP
# define FT_CONTAINERS_FUNCTIONAL_HPP

#include <string>

namespace ft {
	template<class T>
	struct less {
		typedef bool	result_type;
		typedef T		first_argument_type;
		typedef T		second_argument_type;

		result_type operator()(const T& lhs, const T& rhs) const
		{
			return lhs < rhs;
		}
	};

	template<class T>
	struct equal_to {
		typedef bool	result_type;
		typedef T		first_argument_type;
		typedef T		second_argument_type;

		result_type operator()(const T& lhs, const T& rhs) const
		{
			return lhs == rhs;
		}
	};

	template <typename ArgumentType, typename ResultType>
	struct unary_function
	{
		typedef ArgumentType argument_type;
		typedef ResultType result_type;
	};

	struct identity {
		template< class T>
		T& operator()( T& t ) const
		{
			return t;
		}
		template< class T>
		const T& operator()(const T& t ) const
		{
			return t;
		}
	};

	template< class Fn >
	class binder1st :
			public ft::unary_function<typename Fn::second_argument_type,
					typename Fn::result_type> {
	protected:
		Fn op;
		typename Fn::first_argument_type value;
	public:
		binder1st(const Fn& fn,
				  const typename Fn::first_argument_type& value) : op(fn), value(value) {}

		typename Fn::result_type
		operator()(const typename Fn::second_argument_type& x) const
		{
			return this->op(value, x);
		}

		typename Fn::result_type
		operator()(typename Fn::second_argument_type& x) const
		{
			return this->op(value, x);
		}
	};

	template< class Key >
	struct hash
	{
		hash() {}
		hash(const hash &) {}
		std::size_t operator()(const Key &key) const;
	};

	template<> struct hash<bool> : public identity {};

	template<> struct hash<char> : public identity {};
	template<> struct hash<signed char> : public identity {};
	template<> struct hash<unsigned char> : public identity {};
	template<> struct hash<wchar_t> : public identity {};
	template<> struct hash<short> : public identity {};
	template<> struct hash<unsigned short> : public identity {};
	template<> struct hash<int> : public identity {};
	template<> struct hash<unsigned int>;
	template<> struct hash<long> : public identity {};
	template<> struct hash<long long> : public identity {};
	template<> struct hash<unsigned long> : public identity {};
	template<> struct hash<unsigned long long> : public identity {};
	template<> struct hash<float>;
	template<> struct hash<double>;
	template<> struct hash<long double>;
//	template<> struct hash<std::nullptr_t>;
	template< class T > struct hash<T*>;
	template<> struct hash<std::string> {
		// http://www.cse.yorku.ca/~oz/hash.html
		std::size_t operator()(const std::string &key) const
		{
			unsigned long hash = 5381;
//			int c;

			for (std::string::const_iterator it = key.begin(); it != key.end(); it++)
				hash = ((hash << 5) + hash) + *it; /* hash * 33 + c */

			return hash;
		}
	};
}

#endif
