#ifndef FT_CONTAINERS_HASH_TABLE_HPP
# define FT_CONTAINERS_HASH_TABLE_HPP
#include "utility.hpp"
#include "iterator.hpp"
#include <memory>
#include <assert.h>
#include <cstddef>
#include "algorithm.hpp"
#include "functional.hpp"
#include "list.hpp"
#include <list>
#include <vector>
#include "vector.hpp"
#include <iostream>

#define	FT_HASH_TABLE_BASE_SIZE 1

namespace ft {
template <
		class	Key,
		class	Value,
		class	Hash = ft::hash<Key>,
		class	KeyEqual = ft::equal_to<Key>,
		class	Allocator = std::allocator <Key>,
        bool	multi = false,
		class	KeyOfValue = ft::identity
        >
	class hash_table {
	public:
		typedef Key									key_type;
		typedef Value								value_type;
		typedef size_t								size_type;
		typedef ptrdiff_t							difference_type;
		typedef Hash								hasher;
		typedef KeyEqual							key_equal;
		typedef Allocator							allocator_type;
		typedef value_type&							reference;
		typedef const value_type&					const_reference;
		typedef typename Allocator::pointer			pointer;
		typedef typename Allocator::const_pointer	const_pointer;

	private:
		struct Node {
			Node		*next;
			value_type	value;
		};
		typedef typename allocator_type::template rebind<Node>::other				node_allocator;
		typedef Node																*bucket_type;
		typedef typename allocator_type::template rebind<bucket_type>::other		bucket_allocator;
		typedef std::vector<bucket_type, bucket_allocator>							table_type;
		typedef typename table_type::iterator										bucket_iterator;
		typedef typename table_type::const_iterator									const_bucket_iterator;

		template<typename pointed_type>
		struct local_iterator_t {
			typedef ptrdiff_t					difference_type;
			typedef pointed_type				value_type;
			typedef pointed_type				*pointer;
			typedef pointed_type				&reference;
			typedef ft::forward_iterator_tag	iterator_category;

			Node	*current;

			local_iterator_t() : current(NULL) {}

			local_iterator_t(Node *n) : current(n) {}

			template <class T>
			local_iterator_t(local_iterator_t<T> const &other) : current(other.current) {}

			local_iterator_t &operator++() {
				current = current->next;
				return *this;
			}

			local_iterator_t operator++(int) {
				local_iterator_t ret = *this;
				++*this;
				return ret;
			}

			reference operator*() const {
				return current->value;
			}

			pointer operator->() const {
				return &current->value;
			}

		};

	public:
		template<typename LT, typename RT>
		friend bool operator==(local_iterator_t<LT> l, local_iterator_t<RT> r) { return l.current == r.current; }

		template<typename LT, typename RT>
		friend bool operator!=(local_iterator_t<LT> l, local_iterator_t<RT> r) { return l.current != r.current; }

		typedef local_iterator_t<value_type>								local_iterator;
		typedef local_iterator_t<const value_type>							const_local_iterator;
	private:
		template<typename pointed_type>
		struct iterator_t {
			typedef ptrdiff_t					difference_type;
			typedef pointed_type				value_type;
			typedef pointed_type				*pointer;
			typedef pointed_type				&reference;
			typedef ft::forward_iterator_tag	iterator_category;

			bucket_type 				*current_bucket;
			bucket_type 				*end_of_buckets;
			Node						*current_node;

			iterator_t() {}

			iterator_t(bucket_type *cb, bucket_type *eob, Node * cn) : current_bucket(cb), end_of_buckets(eob), current_node(cn) {}

			template <class T>
			iterator_t(iterator_t<T> const &other) : current_bucket(other.current_bucket), end_of_buckets(other.end_of_buckets), current_node(other.current_node) {}

			iterator_t &operator++() {
				this->current_node = this->current_node->next;
				while (!this->current_node)
				{
					this->current_bucket++;
					if (this->current_bucket == this->end_of_buckets)
						break;
					this->current_node = *this->current_bucket;
				}
				return *this;
			}

			iterator_t operator++(int) {
				iterator_t ret = *this;
				++*this;
				return ret;
			}

			reference operator*() const {
				return this->current_node->value;
			}

			pointer operator->() const {
				return &this->current_node->value;
			}

		};
	public:
		template<typename LT, typename RT>
		friend bool operator==(iterator_t<LT> l, iterator_t<RT> r) { return l.current_bucket == r.current_bucket && l.current_node == r.current_node; }

		template<typename LT, typename RT>
		friend bool operator!=(iterator_t<LT> l, iterator_t<RT> r) { return l.current_bucket != r.current_bucket || l.current_node != r.current_node; }

		typedef iterator_t<value_type> iterator;
		typedef iterator_t<const value_type> const_iterator;
	protected:
		table_type		table;
		node_allocator 	_node_allocator;
		size_t			_size;
		size_t			_first_occupied_bucket;
		hasher			_hasher;
		key_equal		_key_equal;
		float			_max_load_factor;
	public:
		hash_table() : table(FT_HASH_TABLE_BASE_SIZE, NULL, Allocator()), _size(0), _first_occupied_bucket(FT_HASH_TABLE_BASE_SIZE), _max_load_factor(1.0) {}

		explicit hash_table( size_type bucket_count,
								const Hash& hash = Hash(),
								const key_equal& equal = key_equal(),
								const Allocator& alloc = Allocator() ) : table(bucket_count, NULL, alloc), _size(0), _node_allocator(alloc), _first_occupied_bucket(bucket_count), _hasher(hash), _key_equal(equal), _max_load_factor(1.0) {}

		explicit hash_table( const Allocator& alloc ) : table(FT_HASH_TABLE_BASE_SIZE, NULL, alloc), _size(0), _node_allocator(alloc), _first_occupied_bucket(FT_HASH_TABLE_BASE_SIZE), _max_load_factor(1.0) {}

		template< class InputIt >
		hash_table( InputIt first, InputIt last,
		size_type bucket_count = FT_HASH_TABLE_BASE_SIZE,
		const Hash& hash = Hash(),
		const key_equal& equal = key_equal(),
		const Allocator& alloc = Allocator() ) : table(bucket_count, NULL, alloc), _node_allocator(alloc), _size(0), _first_occupied_bucket(bucket_count), _hasher(hash), _key_equal(equal), _max_load_factor(1.0)
		{
			this->insert(first, last);
		}

		hash_table(const hash_table &other) : table(other._node_allocator), _node_allocator(other._node_allocator), _size(0), _hasher(other._hasher), _key_equal(other._key_equal)
		{
			*this = other;
		}

		hash_table( const hash_table& other, const Allocator& alloc )  : table(other.table.size(), NULL, alloc), _node_allocator(alloc), _size(0), _first_occupied_bucket(table.size()), _hasher(other._hasher), _key_equal(other._key_equal), _max_load_factor(other._max_load_factor)
		{
			this->insert(other.begin(), other.end());
		}

		~hash_table()
		{
			this->clear();
		}

		hash_table& operator=( const hash_table& other ) {
			this->clear();
			this->table = other.table;
			this->_node_allocator = other._node_allocator;
			for (size_type i = 0; i < this->table.size(); i++)
				this->table[i] = this->copy_list(this->table[i]);
			this->_first_occupied_bucket = other._first_occupied_bucket;
			this->_hasher = other._hasher;
			this->_key_equal = other._key_equal;
			this->_max_load_factor = other._max_load_factor;
			return *this;
		}

		allocator_type get_allocator() const { return this->_node_allocator; }

	private:
		iterator get_it(size_t bucket, Node *current)
		{
			return iterator ((bucket_type *)this->table.data() + bucket, (bucket_type *)this->table.data() + this->table.size(), current);
		}
		const_iterator get_it(size_t bucket, Node *current) const
		{
			return const_iterator ((bucket_type *)this->table.data() + bucket, (bucket_type *)this->table.data() + this->table.size(), current);
		}
	public:

		iterator begin() {
			return get_it(this->_first_occupied_bucket, this->_first_occupied_bucket < this->table.size() ? this->table.at(this->_first_occupied_bucket) : NULL);
		}

		const_iterator begin() const {
			return get_it(this->_first_occupied_bucket, this->_first_occupied_bucket < this->table.size() ? this->table.at(this->_first_occupied_bucket) : NULL);
		}

		iterator end() {
			return get_it(this->bucket_count(), NULL);
		}

		const_iterator end() const {
			return get_it(this->bucket_count(), NULL);
		}

		bool empty() const { return !this->_size; }

		size_type size() const { return this->_size; }

		size_type max_size() const { return this->_node_allocator.max_size(); }

		void clear()
		{
			for (size_t i = 0; i < this->table.size(); i++)
			{
				if (begin(i) != end(i))
					this->destroy_list(this->begin(i).current);
				this->table[i] = NULL;
			}
			this->_first_occupied_bucket = this->table.size();
		}

	private:

		void			check_load_factor(size_type count)
		{
			if ((float)(this->size() + count) / this->bucket_count() > this->_max_load_factor)
				this->reserve(this->size() + count);
		}

		local_iterator find_in_bucket(size_type bucket, const value_type& value)
		{
			local_iterator it;
			for (it = this->begin(bucket); it != this->end(bucket); it++)
				if (_key_equal(KeyOfValue()(*it), KeyOfValue()(value)))
					break;
			return it;
		}

		Node			*new_node(const Node &val)
		{
			Node *ret = this->_node_allocator.allocate(1);
			this->_node_allocator.construct(ret, val);
			this->_size++;
			return ret;
		}

		void	destroy_node(Node &node)
		{
			this->_node_allocator.destroy(static_cast<Node*>(&node));
			this->_node_allocator.deallocate(static_cast<Node*>(&node), 1);
			this->_size--;
		}

		ft::pair<iterator,bool> insert_in_bucket( size_type bucket, const value_type& value, local_iterator hint = NULL)
		{
			local_iterator it;
			Node *n;
			if (hint != this->end(bucket) && _key_equal(KeyOfValue()(*hint), KeyOfValue()(value)) && multi)
			{
				n = new_node((Node){hint.current->next, value});
				hint.current->next = n;
			}
			else
			{
				it = find_in_bucket(bucket, value);
				if (it != this->end(bucket) && !multi)
					return ft::pair<iterator,bool>(iterator(this->table.begin().operator->() +  bucket, this->table.end().operator->(), it.current), false);
				n = new_node((Node){it.current, value});
				get_node_ref(get_it(bucket, it.current)) = n;
			}

			if (bucket < this->_first_occupied_bucket)
				this->_first_occupied_bucket = bucket;

			return ft::pair<iterator,bool>(get_it(bucket, n), true);
		}

		ft::pair<iterator,bool> insert( const value_type& value , ft::true_type)
		{
			this->check_load_factor(1);
			return this->insert_in_bucket(this->bucket(KeyOfValue()(value)), value);
		}

		iterator insert(const value_type &value, ft::false_type) {
			return this->insert(value, ft::true_type()).first;
		}

	public:

		typename ft::conditional<multi, iterator, ft::pair<iterator, bool> >::type insert(const value_type &value) {
			return this->insert(value, ft::integral_constant<bool, !multi>());
		}

		iterator insert( const_iterator hint, const value_type& value )
		{
			this->check_load_factor(1);
			if (hint != this->end() && this->bucket(KeyOfValue()(value)) == (size_type)(hint.current_bucket - this->table.data())) {
				ft::pair<iterator, bool> ret = this->insert_in_bucket(hint.current_bucket - this->table.data(), value, hint.current_node);
				if (ret.second)
					return ret.first;
			}
			return this->insert(value, ft::true_type()).first;
		}

		template< class InputIt >
		void insert( InputIt first, InputIt last )
		{
			iterator hint = this->begin();
			while (first != last)
			{
				hint = this->insert(const_iterator(hint), *first);
				++first;
			}
		}

		iterator erase( iterator pos )
		{
			return this->erase(const_iterator(pos));
		}
	private:
		Node	*&get_node_ref(const_iterator pos)
		{

			if (pos.current_node == *pos.current_bucket)
				return *pos.current_bucket;
			else {
				const_iterator it(pos);
				it.current_node = *it.current_bucket;
				while (it.current_node && it.current_node->next != pos.current_node)
					it++;
				return it.current_node->next;
			}
		}

		void	destroy_list(Node *n)
		{
			while (n)
			{
				Node *tmp = n;
				n = n->next;
				destroy_node(*tmp);
			}
		}

		Node	*copy_list(Node *n)
		{
			if (!n)
				return NULL;
			n = new_node(*n);
			Node *head = n;
			while (head->next)
			{
				head->next = new_node(*head->next);
				head = head->next;
			}
			return n;
		}

	public:

		iterator erase( const_iterator pos )
		{
			iterator ret = ft::next(pos);

			if (pos == this->begin())
				this->_first_occupied_bucket = ret.current_bucket - this->table.data();

			this->get_node_ref(pos) = pos.current_node->next;

			this->destroy_node(*pos.current_node);

			return ret;
		}

		iterator erase( const_iterator first, const_iterator last )
		{
			if (first == last)
				return last;

			if (first == this->begin())
				this->_first_occupied_bucket = last.current_bucket - this->table.data();

			if (first.current_bucket != last.current_bucket) {
				this->get_node_ref(first) = NULL;
				this->destroy_list(first.current_node);
				for (bucket_type *b = first.current_bucket + 1; b != last.current_bucket; b++) {
					this->destroy_list(*b);
					*b = NULL;
				}
				if (last != this->end())
				{
					this->get_node_ref(last) = NULL;
					this->destroy_list(*last.current_bucket);
					*last.current_bucket = last.current_node;
				}
			}
			else
			{
				this->get_node_ref(last) = NULL;
				this->get_node_ref(first) = last.current_node;
				this->destroy_list(first.current_node);
			}
			return last;
		}

		size_type erase( const Key& key )
		{
			ft::pair<iterator, iterator> range = this->equal_range(key);
			size_type ret = ft::distance(range.first, range.second);
			this->erase(range.first, range.second);
			return ret;
		}

		void swap( hash_table& other )
		{
			ft::swap(this->table, other.table);
			ft::swap(this->_size, other._size);
			ft::swap(this->_hasher, other._hasher);
			ft::swap(this->_key_equal, other._key_equal);
			ft::swap(this->_node_allocator, other._node_allocator);
			ft::swap(this->_max_load_factor, other._max_load_factor);
			ft::swap(this->_first_occupied_bucket, other._first_occupied_bucket);
		}

		value_type & at( const Key& key )
		{
			iterator it = this->find(key);
			if (it == this->end())
				throw ft::out_of_range("key not found");
			return *it;
		}

		size_type count( const Key& key ) const
		{
			size_type ret = 0;
			iterator it = this->find(key);
			while (it != this->end() && this->_key_equal(KeyOfValue()(*it), key))
			{
				it++;
				ret++;
			}
			return ret;
		}

	private:

		const_iterator internal_find( const Key& key ) const
		{
			size_t bucket = this->bucket(key);
			for (const_local_iterator it = this->begin(bucket); it != this->end(bucket); it++)
				if (this->_key_equal(key, KeyOfValue()(*it)))
					return get_it(bucket, it.current);
			return this->end();
		}
	public:
		iterator find( const Key& key )
		{
			return this->internal_find(key);
		}

		const_iterator find( const Key& key ) const
		{
			return this->internal_find(key);
		}

	private:

	ft::pair<const_iterator,const_iterator> internal_equal_range( const Key& key ) const
	{
		size_t hash = this->_hasher(key);
		hash %= this->bucket_count();
		for (const_local_iterator it = this->begin(hash); it != this->end(hash); it++)
			if (this->_key_equal(key, KeyOfValue()(*it)))
			{
				const_iterator it2 = get_it(hash, it.current);
				while (it2 != this->end() && this->_key_equal(key, KeyOfValue()(*it2)))
					it2++;
				return ft::pair<const_iterator,const_iterator>(get_it(hash, it.current), it2);
			}
		return ft::pair<const_iterator,const_iterator>(this->end(), this->end());
	}
	public:

		ft::pair<const_iterator,const_iterator> equal_range( const Key& key ) const
		{
			return this->internal_equal_range(key);
		}


		ft::pair<iterator,iterator> equal_range( const Key& key )
		{
			return this->internal_equal_range(key);
		}

	public:

		local_iterator begin( size_type n )
		{
			return this->table[n];
		}

		const_local_iterator begin( size_type n ) const
		{
			return this->table[n];
		}

		local_iterator end( size_type)
		{
			return NULL;
		}

		const_local_iterator end( size_type) const
		{
			return NULL;
		}

		size_type bucket_count() const
		{
			return this->table.size();
		}

		size_type max_bucket_count() const
		{
			return this->table.max_size();
		}

		size_type bucket_size( size_type n ) const
		{
			return ft::distance(this->begin(n), this->end(n));
		}

		size_type bucket( const Key& key ) const
		{
			size_t hash = this->_hasher(key);
			hash %= this->bucket_count();
			return hash;
		}

		float load_factor() const
		{
			return this->size() / this->bucket_count();
		}

		float max_load_factor() const
		{
			return this->_max_load_factor;
		}

		void max_load_factor( float ml )
		{
			this->_max_load_factor = ml;
		}

		void rehash( size_type count )
		{
			size_type new_size = this->size() / max_load_factor();
			if (count > new_size)
				new_size = count;

			table_type new_table(new_size, NULL, this->_node_allocator);
			size_t	old_bucket = new_size + 1;
			Node	*previous_node = NULL;
			size_t first_bucket = new_size;
			for (iterator it = this->begin(); it != this->end();it++, previous_node->next = NULL)
			{
				size_t bucket = this->_hasher(KeyOfValue()(*it));
				bucket %= new_table.size();
				if (bucket < first_bucket)
					first_bucket = bucket;
				if (bucket == old_bucket)
					previous_node->next = it.current_node;
				else if (!new_table[bucket])
					new_table[bucket] = it.current_node;
				else
				{
					Node *n = new_table[bucket];
					while (n->next)
						n = n->next;
					n->next = it.current_node;
				}

				old_bucket = bucket;
				previous_node = it.current_node;
			}
			this->_first_occupied_bucket = first_bucket;
			this->table = new_table;
		}

		void reserve( size_type count )
		{
			count = count > 10 * this->bucket_count() ? count : 10 * bucket_count();
			this->rehash(count / max_load_factor() + 1);
		}

		hasher hash_function() const
		{
			return this->_hasher;
		}

		key_equal key_eq() const
		{
			return this->_key_equal;
		}
	};

	template< class K, class U, class H, class KE, class A, bool M, class KOV >
	bool operator!=( const ft::hash_table<K, U, H, KE, A, M, KOV>& lhs,
					 const ft::hash_table<K, U, H, KE, A, M, KOV>& rhs )
	{
		if (lhs.size() != rhs.size())
			return true;

		typename ft::hash_table<K, U, H, KE, A, M, KOV>::iterator lit1 = lhs.begin();
		typename ft::hash_table<K, U, H, KE, A, M, KOV>::iterator lit2 = lhs.begin();
		while (lit1 != lhs.end())
		{
			lit2 = lhs.equal_range(*lit1).second;
			ft::pair<typename ft::hash_table<K, U, H, KE, A, M, KOV>::iterator, typename ft::hash_table<K, U, H, KE, A, M, KOV>::iterator> ritp = rhs.equal_range(*lit1);
			if (ft::distance(ritp.first, ritp.second) != ft::distance(lit1, lit2))
				return true;
			if (!ft::is_permutation(lit1, lit2, ritp.first))
				return true;
			lit1 = lit2;
		}
		return false;
	}

	template< class K, class U, class H, class KE, class A, bool M, class KOV >
	bool operator==( const ft::hash_table<K, U, H, KE, A, M, KOV>& lhs,
					 const ft::hash_table<K, U, H, KE, A, M, KOV>& rhs )
	{
		return !(lhs != rhs);
	}

	template< class K, class U, class H, class KE, class A, bool M, class KOV >
	void swap( ft::hash_table<K, U, H, KE, A, M, KOV>& lhs,
			   ft::hash_table<K, U, H, KE, A, M, KOV>& rhs )
	{
		lhs.swap(rhs);
	}
}

#endif