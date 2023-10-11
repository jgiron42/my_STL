#ifndef STDEXCEPT_HPP
# define STDEXCEPT_HPP
# include <exception.hpp>
#include <string>
namespace ft {
	class logic_error : public exception {
	private:
	private:
		struct shared_string {
			std::string str;
			int 		nref;
			shared_string(const std::string &s) : str(s), nref(1) {}

		};
		shared_string *s;
	public:
		logic_error( const std::string& what_arg ) : s(new shared_string(what_arg)) {}
		logic_error( const char* what_arg ) : s(new shared_string(what_arg)) {}
		logic_error( const logic_error& other ) : ft::exception(), s(other.s) {
			s->nref++;
		}
		virtual ~logic_error()
		{
			if (this->s->nref == 1)
				delete this->s;
			else
				this->s->nref--;
		}
		logic_error& operator=( const logic_error& other )
		{
			if (this->s != other.s)
			{
				if (this->s->nref == 1)
					delete this->s;
				else
					this->s->nref--;
				this->s = other.s;
				s->nref++;
			}
			return *this;
		}
		virtual const char *what() const throw() {
			return this->s->str.data();
		}
	};

	class out_of_range : public logic_error {
	public:
		out_of_range( const std::string& what_arg ) : logic_error(what_arg) {}
		out_of_range( const char* what_arg ) : logic_error(what_arg) {}
		out_of_range( const out_of_range& other ) : logic_error(other) {}
	};
}
#endif