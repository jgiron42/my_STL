#ifdef TEST_FT
# define NAMESPACE ft
# if defined(TEST_UNORDERED_MAP) || defined(TEST_UNORDERED_MULTIMAP)
#  include "unordered_map.hpp"
# endif
# if defined(TEST_UNORDERED_SET) || defined(TEST_UNORDERED_MULTISET)
#  include "unordered_set.hpp"
# endif
# if defined(TEST_MAP) || defined(TEST_MULTIMAP)
#  include "map.hpp"
# endif
# if defined(TEST_SET) || defined(TEST_MULTISET)
#  include "set.hpp"
# endif
# ifdef TEST_VECTOR
#  include "vector.hpp"
# endif
# ifdef TEST_STACK
#  include "stack.hpp"
# endif
# ifdef TEST_DEQUE
#  include "deque.hpp"
# endif
# ifdef TEST_LIST
#  include "list.hpp"
# endif
# ifdef TEST_XORLIST
#  include "xorlist.hpp"
# endif
# ifdef TEST_PRIORITY_QUEUE
#  include "queue.hpp"
# endif
#elif defined(TEST_STD)
# define NAMESPACE std
# if defined(TEST_UNORDERED_MAP) || defined(TEST_UNORDERED_MULTIMAP)
#  include <unordered_map>
# endif
# if defined(TEST_UNORDERED_SET) || defined(TEST_UNORDERED_MULTISET)
#  include <unordered_set>
# endif
# if defined(TEST_MAP) || defined(TEST_MULTIMAP)
#  include <map>
# endif
# if defined(TEST_SET) || defined(TEST_MULTISET)
#  include <set>
# endif
# ifdef TEST_VECTOR
#  include <vector>
# endif
# ifdef TEST_STACK
#  include <stack>
# endif
# ifdef TEST_DEQUE
#  include <deque>
# endif
# ifdef TEST_LIST
#  include <list>
# endif
# ifdef TEST_XORLIST
#  warning no xorlist in stl
#  include <list>
#define xorlist list
# endif
# ifdef TEST_PRIORITY_QUEUE
#  include <queue>
# endif
#endif

#include "container.hpp"
#include "utils/stateful_comparator.hpp"
#if defined (TEST_SET) || defined(TEST_MAP) || defined(TEST_MULTISET) || defined(TEST_MULTIMAP)
# include "associative.hpp"
#endif
#if defined (TEST_UNORDERED_SET) || defined(TEST_UNORDERED_MAP) || defined(TEST_UNORDERED_MULTISET) || defined(TEST_UNORDERED_MULTIMAP)
# include "unordered_associative.hpp"
#endif
#if defined (TEST_VECTOR) || defined(TEST_DEQUE) || defined(TEST_LIST) || defined(TEST_XORLIST) || defined(TEST_STACK)
# include "list_specific.hpp"
# include "sequence.hpp"
#endif
#if defined (TEST_LIST)
# include "list_specific.hpp"
#endif
#if defined(TEST_PRIORITY_QUEUE)
# include "priority_queue.hpp"
#endif

#ifdef TEST_UNORDERED_MAP
void test_unordered_map(size_t ntest)
{
	typedef NAMESPACE::unordered_map<std::string, std::string> C;
	monkey::test_container<C> (container::get_table<C>() + unordered_associative::get_table<C>(), ntest);
}
#endif

#ifdef TEST_UNORDERED_MULTIMAP
void test_unordered_multimap(size_t ntest)
{
	typedef NAMESPACE::unordered_multimap<std::string, std::string> C;
	monkey::test_container<C> (container::get_table<C>() + unordered_associative::get_table<C>(), ntest);
}
#endif

#ifdef TEST_UNORDERED_SET
void test_unordered_set(size_t ntest)
{
	typedef NAMESPACE::unordered_set<std::string> C;
	monkey::test_container<C> (container::get_table<C>() + unordered_associative::get_table<C>(), ntest);
}
#endif

#ifdef TEST_UNORDERED_MULTISET
void test_unordered_multiset(size_t ntest)
{
	typedef NAMESPACE::unordered_multiset<std::string> C;
	monkey::test_container<C> (container::get_table<C>() + unordered_associative::get_table<C>(), ntest);
}
#endif

#ifdef TEST_MAP
void test_map(size_t ntest)
{
	typedef NAMESPACE::map<std::string, std::string, stateful_comparator> C;
	monkey::test_container<C> (container::get_table<C>() + associative::get_table<C>(), ntest);
}
#endif

#ifdef TEST_SET
void test_set(size_t ntest)
{
	typedef NAMESPACE::set<std::string, stateful_comparator> C;
	monkey::test_container<C> (container::get_table<C>() + associative::get_table<C>(), ntest);
}
#endif

#ifdef TEST_MULTIMAP
void test_multimap(size_t ntest)
{
	typedef NAMESPACE::multimap<std::string, std::string, stateful_comparator> C;
	monkey::test_container<C> (container::get_table<C>() + associative::get_table<C>(), ntest);
}
#endif

#ifdef TEST_MULTISET
void test_multiset(size_t ntest)
{
	typedef NAMESPACE::multiset<std::string, stateful_comparator> C;
	monkey::test_container<C> (container::get_table<C>() + associative::get_table<C>(), ntest);
}
#endif

#ifdef TEST_VECTOR
void test_vector(size_t ntest)
{
	typedef NAMESPACE::vector<std::string> C;
	monkey::test_container<C> (
			container::get_table<C>() +
			sequence::get_table<C>() +
			sequence::at<C> +
			sequence::operator_at<C>
			, ntest);
}
#endif


#ifdef TEST_STACK
void test_stack(size_t ntest)
{
	typedef NAMESPACE::stack<std::string> C;
	monkey::test_container<C> (
			monkey::function_table<C>() +
			container::default_constructor<C> +
			container::empty<C> +
			container::copy_constructor<C> +
			container::operator_assign<C>
			, ntest);
}
#endif

#ifdef TEST_DEQUE
void test_deque(size_t ntest)
{
	typedef NAMESPACE::deque<std::string> C;
	monkey::test_container<C> (
			container::get_table<C>() +
			sequence::get_table<C>() +
			sequence::push_front<C> +
			sequence::pop_front<C> +
			sequence::at<C> +
			sequence::operator_at<C>
			        , ntest);
}
#endif

#ifdef TEST_LIST
void test_list(size_t ntest)
{
	typedef NAMESPACE::list<std::string> C;
	monkey::test_container<C> (
			container::get_table<C>() +
			sequence::get_table<C>() +
			sequence::push_front<C> +
			sequence::pop_front<C> +
	        list_specific::get_table<C>()
			        , ntest);
}
#endif

#ifdef TEST_XORLIST
void test_xorlist(size_t ntest)
{
	typedef NAMESPACE::xorlist<std::string> C;
	monkey::test_container<C> (
			container::get_table<C>() +
			sequence::get_table<C>() +
			sequence::push_front<C> +
			sequence::pop_front<C> +
	        list_specific::get_table<C>()
			        , ntest);
}
#endif

#ifdef TEST_PRIORITY_QUEUE
void test_priority_queue(size_t ntest)
{
	typedef NAMESPACE::priority_queue<std::string, NAMESPACE::vector<std::string>, stateful_comparator> C;
	monkey::test_container<C> (
			priority_queue::get_table<C>()
			        , ntest);
}
#endif

int main(int argc, char **argv)
{
	unsigned int seed = 0;
	size_t ntest = 10000;
	if (argc > 1)
		seed = atol(argv[1]);
	if (argc > 2)
		ntest = atol(argv[2]);
	srand(seed);

	#ifdef TEST_UNORDERED_MAP
	test_unordered_map(ntest);
	#endif

	#ifdef TEST_UNORDERED_MULTIMAP
	test_unordered_multimap(ntest);
	#endif

	#ifdef TEST_UNORDERED_SET
	test_unordered_set(ntest);
	#endif

	#ifdef TEST_UNORDERED_MULTISET
	test_unordered_multiset(ntest);
	#endif

	#ifdef TEST_MAP
	test_map(ntest);
	#endif

	#ifdef TEST_SET
	test_set(ntest);
	#endif

	#ifdef TEST_MULTIMAP
	test_multimap(ntest);
	#endif

	#ifdef TEST_MULTISET
	test_multiset(ntest);
	#endif

	#ifdef TEST_VECTOR
	test_vector(ntest);
	#endif

	#ifdef TEST_DEQUE
	test_deque(ntest);
	#endif

	#ifdef TEST_LIST
	test_list(ntest);
	#endif

	#ifdef TEST_XORLIST
	test_xorlist(ntest);
	#endif

	#ifdef TEST_PRIORITY_QUEUE
	test_priority_queue(ntest);
	#endif

	#ifdef TEST_STACK
	test_stack(ntest);
	#endif
}