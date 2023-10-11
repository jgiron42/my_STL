#ifndef EXCEPTION_HPP
# define EXCEPTION_HPP
namespace ft {
	class exception {
	public:
		exception() throw() {}

		exception(const exception &other) throw() { *this = other; }

		virtual ~exception() {};

		exception &operator=(const exception &) throw() { return *this; };

		virtual const char *what() const throw() {
			return "ft::exception";
		}
	};
}
#endif
