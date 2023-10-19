#ifndef FT_CONTAINERS_BST_HPP
#define FT_CONTAINERS_BST_HPP
#include "utility.hpp"
#include "iterator.hpp"
#include <memory>
#include <cstddef>
#include "algorithm.hpp"
#include "functional.hpp"
#include <iostream>

template<
		class	Key,
		class	Value,
		class	Compare = ft::less <Key>,
		class	Allocator = std::allocator <Key>,
        bool	multi = false,
		class	KeyOfValue = ft::identity
>
class bst {
public:
	typedef Key									key_type;
	typedef Value								value_type;
	typedef size_t								size_type;
	typedef ptrdiff_t							difference_type;
	typedef Allocator							allocator_type;
	typedef value_type&							reference;
	typedef const value_type&					const_reference;
	typedef typename Allocator::pointer			pointer;
	typedef typename Allocator::const_pointer	const_pointer;
	typedef Compare								key_compare;
	class value_compare {
	public:
		typedef bool		result_type;
		typedef value_type	first_argument_type;
		typedef value_type	second_argument_type;
		Compare comp;
		value_compare() : comp() {}
		value_compare( Compare c ) : comp(c) {}
		bool operator()( const value_type& lhs, const value_type& rhs ) const
		{
			return this->comp(KeyOfValue()(lhs), KeyOfValue()(rhs));
		}
	};
private:
	enum e_color {BLACK, RED};
	enum e_dir {LEFT, RIGHT};
	struct node_type_base {
		node_type_base() : l(NULL), r(NULL), p(NULL), color(BLACK) {}
		node_type_base(const node_type_base &other) : l(other.l), r(other.r), p(other.p), color(other.color) {}
		union {
			struct {
				node_type_base *l;
				node_type_base *r;
			};
			node_type_base *childs[2];
		};
		node_type_base	*p;
		e_color			color;
		node_type_base *&uncle() const
		{
			if (this->p->p->l == this->p)
				return this->p->p->r;
			else
				return this->p->p->l;
		}
		node_type_base *&sibbling() const
		{
			return this->p->l == this ? this->p->r : this->p->l;
		}
		node_type_base *&close_nephew() const
		{
			return this->sibbling()->childs[this->side()];
		}
		node_type_base *&distant_nephew() const
		{
			return this->sibbling()->childs[1 - this->side()];
		}
		node_type_base *&ptr() const
		{
			return this->p->l == this ? this->p->l : this->p->r;
		}
		int side() const
		{
			return this->p->l == this ? LEFT : RIGHT;
		}
	};
	struct node_type : public node_type_base {
		node_type(const value_type & value) : value(value) {}
		node_type(const node_type &other) : node_type_base(other), value(other.value) {}
		value_type value;
	};
	template<typename pointed_type>
	struct iterator_t {
		typedef ptrdiff_t difference_type;
		typedef bst::value_type value_type;
		typedef pointed_type *pointer;
		typedef pointed_type &reference;
		typedef ft::bidirectional_iterator_tag iterator_category;

		bst::node_type_base *current;

		iterator_t() : current(NULL) {}

		iterator_t(bst::node_type_base *n) : current(n) {}

		iterator_t(const bst::node_type_base *n) : current((bst::node_type_base *)n) {}

		template <class other_type>
		iterator_t(iterator_t<other_type> const &other) : current(other.current) {}

		iterator_t &operator++() {
			this->current = next_node(this->current);
			return *this;
		}

		iterator_t operator++(int) {
			iterator_t ret = *this;
			this->current = next_node(this->current);
			return ret;
		}

		iterator_t &operator--() {
			this->current = prev_node(this->current);
			return *this;
		}

		iterator_t operator--(int) {
			iterator_t ret = *this;
			this->current = prev_node(this->current);
			return ret;
		};

		reference operator*() const { return static_cast<node_type*>(this->current)->value; }

		pointer operator->() const { return &static_cast<node_type*>(this->current)->value; }

	};
public:
	template<typename LT, typename RT>
	friend bool operator==(iterator_t<LT> l, iterator_t<RT> r) { return l.current == r.current; }

	template<typename LT, typename RT>
	friend bool operator!=(iterator_t<LT> l, iterator_t<RT> r) { return l.current != r.current; }

	typedef iterator_t<value_type> iterator;
	typedef iterator_t<const value_type> const_iterator;
	typedef ft::reverse_iterator<iterator> reverse_iterator;
	typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;
private:
	Compare compare;
	value_compare _value_comp;
	size_type _size;
	typename allocator_type::template rebind<node_type>::other allocator;
	node_type_base super_root;
	node_type_base *first;
	node_type_base *last;
public:
	bst() : compare(), _value_comp(compare), _size(), allocator(), super_root(), first(&this->super_root), last(&this->super_root) {};

	explicit bst(const Compare &comp, const Allocator &alloc = Allocator()) : compare(comp), _value_comp(comp), _size(),
	allocator(alloc), super_root(), first(&this->super_root), last(&this->super_root) {}

	template<class InputIt>
	bst(InputIt first, InputIt last, const Compare &comp = Compare(), const Allocator &alloc = Allocator()) : compare(
			comp), _value_comp(comp), _size(), allocator(alloc), super_root(), first(&this->super_root), last(&this->super_root) {
		this->insert(first, last);
	}

	bst(const bst &other) : compare(other.compare), _value_comp(other._value_comp), _size(0), allocator(other.allocator),
	super_root(), first(&this->super_root), last(&this->super_root) {
		*this = other;
	}

	~bst() {
		this->clear();
	}

	bst &operator=(const bst &other) {
		this->clear();
		this->compare = other.compare;
		this->_value_comp = other._value_comp;
		this->allocator = other.allocator;
		this->assign_tree(copy_tree(other, other.super_root.l));
		return *this;
	}

	allocator_type get_allocator() const { return this->allocator; }

	iterator begin() { return iterator(this->first); }

	const_iterator begin() const { return const_iterator(this->first); }

	iterator end() { return iterator(&this->super_root); }

	const_iterator end() const { return const_iterator(&this->super_root); }

	reverse_iterator rbegin() { return reverse_iterator(this->end()); }

	const_reverse_iterator rbegin() const { return const_reverse_iterator(this->end()); }

	reverse_iterator rend() { return reverse_iterator(this->begin()); }

	const_reverse_iterator rend() const { return const_reverse_iterator(this->begin()); }

	bool empty() const { return this->_size == 0; }

	size_type size() const { return this->_size; }

	size_type max_size() const { return this->allocator.max_size(); };

	void clear() {
		destroy_tree(this->super_root.l);
		this->assign_tree(NULL);
		this->first = &this->super_root;
		this->last = &this->super_root;
	}

	typename ft::conditional<multi, iterator, ft::pair<iterator, bool> >::type insert(const value_type &value) {
		return this->insert(value, ft::integral_constant<bool, !multi>());
	}
private:
	ft::pair<iterator, bool> insert(const value_type &value, ft::true_type) {
		ft::pair<iterator, bool> ret = this->recursive_insert(this->super_root.l, &this->super_root, value);
		if (ret.second && ret.first.current == this->super_root.l)
			this->super_root.r = this->super_root.l;
		return ret;
	}

	iterator insert(const value_type &value, ft::false_type) {
		return this->insert(value, ft::true_type()).first;
	}
public:
	iterator insert(iterator pos, const value_type &value) {
		node_type_base *current = pos.current;
		node_type_base *next = next_node(current);
		if (multi)
		{
			// if current <= value < next
			if (current != &this->super_root && !this->_value_comp(value, static_cast<node_type *>(current)->value) &&
				(next == &this->super_root || !this->_value_comp(static_cast<node_type *>(next)->value, value))) {
				if (!current->r)
					return this->recursive_insert(current->r, current, value).first;
				return this->recursive_insert(next->l, next, value).first;
			}
			return this->insert(value, ft::false_type ());
		}
		else
		{
			// if current < value < next
			if (current != &this->super_root && this->_value_comp(static_cast<node_type *>(current)->value,value) &&
				(next == &this->super_root || this->_value_comp(value, static_cast<node_type *>(next)->value))) {
				if (!current->r)
					return this->recursive_insert(current->r, current, value).first;
				return this->recursive_insert(next->l, next, value).first;
			}
		}
		return this->insert(value, ft::true_type()).first;
	}

	template<class InputIt>
	void insert(InputIt f, InputIt l) {
		iterator tmp = this->begin();
		while (f != l) {
			tmp = this->insert(tmp, (value_type)*f);
			f++;
		}
	}

	void erase(iterator pos) {
		erase_node(*pos.current);
	}

	void erase(iterator f, iterator l) {
		while (f != l) {
			iterator next = f;
			++next;
			this->erase_node(*f.current);
			f = next;
		}
	}

	size_type erase(const Key &key) {
		if (!multi)
			return this->erase_multi(this->super_root.l, key);
		ft::pair<iterator, iterator> range = this->equal_range(key);
		size_type ret = ft::distance(range.first, range.second);
		this->erase(range.first, range.second);
		return ret;

		return this->erase_multi(this->super_root.l, key);
	}

	void swap(bst &other) {

		ft::swap(this->compare, other.compare);
		ft::swap(this->_value_comp, other._value_comp);
		ft::swap(this->first, other.first);
		ft::swap(this->last, other.last);
		node_type_base *tree = this->super_root.l;
		this->assign_tree(other.super_root.l);
		other.assign_tree(tree);
		ft::swap(this->allocator, other.allocator);
		ft::swap(this->_size, other._size);
	}

	size_type count(const Key &key) const {return this->recursive_count(this->super_root.l, key);}

	iterator find(const Key &key) {
		node_type_base *tmp = this->recursive_find(this->super_root.l, key);
		if (!tmp)
			return this->end();
		return iterator(tmp);
	}

	const_iterator find(const Key &key) const
	{
		node_type_base *tmp = this->recursive_find(this->super_root.l, key);
		if (!tmp)
			return this->end();
		return const_iterator(tmp);
	}

	ft::pair<iterator,iterator> equal_range( const Key& key )
	{
		return ft::make_pair<iterator,iterator>(this->lower_bound(key), this->upper_bound(key));
	}

	ft::pair<const_iterator,const_iterator> equal_range( const Key& key ) const
	{
		return ft::make_pair<const_iterator,const_iterator>(this->lower_bound(key), this->upper_bound(key));
	}

	iterator upper_bound( const Key& key ) {
		return internal_upper_bound(key);
	}

	const_iterator upper_bound( const Key& key ) const {
		return internal_upper_bound(key);
	}

	iterator lower_bound( const Key& key ) {
		return internal_lower_bound(key);
	}

	const_iterator lower_bound( const Key& key ) const {
		return internal_lower_bound(key);
	}

	key_compare key_comp() const {return this->compare;}

	value_compare value_comp() const {return this->_value_comp;}

	friend bool operator==(const bst &lhs, const bst &rhs) {
		return ((lhs.size() == rhs.size()) && ft::equal<const_iterator, const_iterator>(lhs.begin(), lhs.end(), rhs.begin()));
	}
	friend bool operator!=(const bst &lhs, const bst &rhs) {
		return (!(lhs == rhs));
	}
	friend bool operator<(const bst &lhs, const bst &rhs) {
		return (ft::lexicographical_compare<const_iterator, const_iterator>(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}
	friend bool operator<=(const bst &lhs, const bst &rhs) {
		return (!(rhs < lhs));
	}
	friend bool operator>(const bst &lhs, const bst &rhs) {
		return (rhs < lhs);
	}
	friend bool operator>=(const bst &lhs, const bst &rhs) {
		return (!(lhs < rhs));
	}
private:

	ft::pair<iterator , bool>	recursive_insert(node_type_base *& current, node_type_base *parent, const value_type& value)
	{
		if (!current)
		{
			current = new_node(value);
			current->p = parent;
			if (parent == &this->super_root)
				this->assign_tree(current);
			if (parent == this->first && parent->l == current)
				this->first = current;
			if (parent == this->last && parent->r == current)
				this->last = current;
			ft::pair<iterator, bool> ret = ft::make_pair(iterator(current), true);
			this->fix_insert(current);
#ifdef DEBUG_BST_HPP
			if (!this->rb_test())
				this->print();
#endif
			return ret;
		}
		if (this->_value_comp(value, (static_cast<node_type*>(current)->value)))
			return recursive_insert(current->l, current, value);
		if (multi)
			return recursive_insert(current->r, current, value);
		else
		{
			if (this->_value_comp((static_cast<node_type *>(current)->value), (value)))
				return recursive_insert(current->r, current, value);
			return ft::make_pair(iterator(current), false);
		}
	}

	void	fix_insert(node_type_base *n)
	{
		n->color = RED;
		while (n->p->color == RED && n->p != &this->super_root) {
			if (n->p->p == &this->super_root) {
				n->p->color = BLACK;
				return;
			}
			if (n->uncle() && n->uncle()->color == RED) {
				n->p->color = BLACK;
				n->p->sibbling()->color = BLACK;
				n->p->p->color = RED;
				n = n->p->p;
			} else {
				if (n->side() != n->p->side()) {
					rotate(n->p, n->p->side());
					n = n->childs[n->side()];
				}
				rotate(n->p->p, 1 - n->p->side());
				n->color = BLACK;
				n = n->p;
			}
		}
	}

	void	erase_node(node_type_base &ref)
	{
		node_type_base *&n = ref.p->l == &ref ? ref.p->l : ref.p->r;

		if (&ref == this->first)
			this->first = next_node(&ref);
		if (&ref == this->last)
			this->last = prev_node(&ref);
		if (n->l && n->r)
		{
			node_type_base *prev = prev_node(&ref);
			swap_node(prev, &ref);
			if (ref.p->l == &ref)
				return erase_node(*ref.p->l);
			else
				return erase_node(*ref.p->r);
		}
		fix_erase(n);
		node_type_base *&n2 = ref.p->l == &ref ? ref.p->l : ref.p->r;
		if (n2->r)
		{
			n2->r->p = n2->p;
			n2 = n2->r;
		}
		else if (n->l)
		{
			n2->l->p = n2->p;
			n2 = n2->l;
		}
		else
			n = NULL;
		if (ref.p == &this->super_root)
			this->super_root.r = n;
#ifdef DEBUG_BST_HPP
		if (!rb_test())
			print();
#endif
		destroy_node(ref);
	}

	void	fix_erase(node_type_base *n) {
		if (n->color == RED)
			return;
		if (n->l || n->r)
		{
			(n->l ? n->l : n->r)->color = BLACK;
			return;
		}
		// n is now a black leaf
		while(n->p != &this->super_root)
		{
			if (n->p->color == BLACK) {
				if (n->sibbling()->color == BLACK) {
					if ((!n->distant_nephew() || n->distant_nephew()->color == BLACK)
						&& (n->close_nephew() && n->close_nephew()->color == RED)) {
						rotate(n->sibbling(), n->sibbling()->side());
						n->sibbling()->color = BLACK;
						n->distant_nephew()->color = RED;
					}
					if (n->distant_nephew() && n->distant_nephew()->color == RED) {
						n->distant_nephew()->color = BLACK;
						rotate(n->p, n->side());
						return;
					}
					// push up
					n->sibbling()->color = RED;
					n->p->color = BLACK;
					n = n->p;
				} else {
					rotate(n->p, n->side());
					n->p->color = RED;
					n->p->p->color = BLACK;
				}
			} else if (n->p->color == RED) {
				if (n->sibbling() && n->close_nephew() && n->close_nephew()->color == RED &&
					n->sibbling() && n->distant_nephew() && n->distant_nephew()->color == RED) {
					n->p->color = BLACK;
					n->sibbling()->color = RED;
					n->distant_nephew()->color = BLACK;
					rotate(n->p, n->side());
					return;
				}
				if (n->sibbling() && n->close_nephew() && n->close_nephew()->color == RED &&
					(!n->distant_nephew() || n->distant_nephew()->color == BLACK)) {
					n->sibbling()->color = RED;
					n->close_nephew()->color = BLACK;
					rotate(n->sibbling(), n->sibbling()->side());
				}
				if (n->sibbling() && n->distant_nephew() && n->distant_nephew()->color == RED)
					rotate(n->p, n->side());
				else {
					n->p->color = BLACK;
					if (n->sibbling()) {
						n->sibbling()->color = RED;
						if (n->distant_nephew())
							n->distant_nephew()->color = BLACK;
						if (n->close_nephew())
							n->close_nephew()->color = BLACK;
					}
				}
				return;
			}
		}
	}

	size_type erase_multi(node_type_base *current, const Key &key)
	{
		if (!current)
			return 0;
		if (this->compare(key, KeyOfValue()(static_cast<node_type*>(current)->value)))
			return this->erase_multi(current->l, key);
		else if (this->compare(KeyOfValue()(static_cast<node_type*>(current)->value), key))
			return this->erase_multi(current->r, key);
		else if (multi)
		{
			size_type ret = 1 + this->erase_multi(current->l, key) + this->erase_multi(current->r, key);
			this->erase_node(*current);
			return ret;
		}
		else
		{
			this->erase_node(*current);
			return 1;
		}
	}

	size_type recursive_count(node_type_base *current, const Key &key) const
	{
		if (!current)
			return 0;
		if (this->compare(key, KeyOfValue()(static_cast<node_type*>(current)->value)))
			return this->recursive_count(current->l, key);
		else if (this->compare(KeyOfValue()(static_cast<node_type*>(current)->value), key))
			return this->recursive_count(current->r, key);
		else if (multi)
			return 1 + this->recursive_count(current->l, key) + this->recursive_count(current->r, key);
		else
			return 1;
	}

	node_type_base	*recursive_find(node_type_base *current, const Key &key) const
	{
		if (!current)
			return NULL;
		if (this->compare(key, KeyOfValue()(static_cast<node_type*>(current)->value)))
			return this->recursive_find(current->l, key);
		else if (this->compare(KeyOfValue()(static_cast<node_type*>(current)->value), key))
			return this->recursive_find(current->r, key);
		else
			return current;
	}

	iterator internal_lower_bound(const Key& key) const
	{
		node_type_base *n = (node_type_base *)this->super_root.l;
		node_type_base *ret = (node_type_base *)&this->super_root;
		while (n)
		{
			if ((ret == &this->super_root || !this->_value_comp(static_cast<node_type*>(ret)->value, static_cast<node_type*>(n)->value)) &&
				!this->compare(KeyOfValue()(static_cast<node_type*>(n)->value), key))
				ret = n;
			if (this->compare(KeyOfValue()(static_cast<node_type*>(n)->value), key))
				n = n->r;
			else if (this->compare(key, KeyOfValue()(static_cast<node_type*>(n)->value)))
				n = n->l;
			else if (multi)
				n = n->l;
			else
				return iterator(ret);
		}
		return iterator(ret);
	}

	iterator internal_upper_bound(const Key& key) const
	{
		node_type_base *n = (node_type_base *)this->super_root.l;
		node_type_base *ret = (node_type_base *)&this->super_root;
		while (n)
		{
			if ((ret == &this->super_root || !this->_value_comp(static_cast<node_type*>(ret)->value, static_cast<node_type*>(n)->value)) &&
				this->compare(key, KeyOfValue()(static_cast<node_type*>(n)->value)))
				ret = n;
			if (this->compare(KeyOfValue()(static_cast<node_type*>(n)->value), key))
				n = n->r;
			else if (this->compare(key, KeyOfValue()(static_cast<node_type*>(n)->value)))
				n = n->l;
			else
				n = n->r;
		}
		return iterator(ret);
	}

	node_type *new_node(const node_type &val)
	{
		node_type *ret = this->allocator.allocate(1);
		this->allocator.construct(ret, val);
		this->_size++;
		return ret;
	}

	void	assign_tree(node_type_base *tree)
	{
		this->super_root.l = tree;
		this->super_root.r = tree;
		if (tree)
			tree->p = &this->super_root;
		else
		{
			this->first = &this->super_root;
			this->last = &this->super_root;
		}
	}

	void	destroy_node(node_type_base &node)
	{
		this->allocator.destroy(static_cast<node_type*>(&node));
		this->allocator.deallocate(static_cast<node_type*>(&node), 1);
		this->_size--;
	}

	node_type_base *copy_tree(const bst& other_bst, const node_type_base *other)
	{
		if (!other)
			return NULL;
		node_type_base *ret = new_node(*static_cast<const node_type*>(other));
		if (ret->r)
		{
			ret->r = copy_tree(other_bst, ret->r);
			ret->r->p = ret;
		}
		if (ret->l)
		{
			ret->l = copy_tree(other_bst, ret->l);
			ret->l->p = ret;
		}
		if (other_bst.first == other)
			this->first = ret;
		if (other_bst.last == other)
			this->last = ret;
		return ret;
	}

	void	destroy_tree(node_type_base *tree)
	{
		if (!tree)
			return;
		if (tree->l)
			destroy_tree(tree->l);
		if (tree->r)
			destroy_tree(tree->r);
		destroy_node(*tree);
	}

	static node_type_base *next_node(node_type_base *n)
	{
		if (n->r)
		{
			n = n->r;
			while (n->l)
				n = n->l;
		}
		else
		{
			while (n->p && n->p->r == n)
				n = n->p;
			if (n->p)
				n = n->p;
		}
		return n;
	}

	static node_type_base *prev_node(node_type_base *n)
	{
		if (n->l)
		{
			n = n->l;
			while (n->r)
				n = n->r;
		}
		else
		{
			while (n->p && n->p->l == n)
				n = n->p;
			if (n->p)
				n = n->p;
		}
		return n;
	}

	void 	swap_node(node_type_base *a, node_type_base *b)
	{
		if (a->p == b)
			return swap_node(b, a); // if the two nodes are directly linked ensure that the parent is always a

		if (b->p == a) {
			node_type_base swap = *a;
			a->p = b;
			a->l = b->l;
			a->r = b->r;

			b->p = swap.p;
			if (b->p->r == a)
				b->p->r = b;
			if (b->p->l == a)
				b->p->l = b;
			b->r = swap.r;
			b->l = swap.l;
			if (swap.l == b)
				b->l = a;
			else
				b->r = a;
		} else {
			if (b->p->r == b)
				b->p->r = a;
			if (b->p->l == b)
				b->p->l = a;
			if (a->p->r == a)
				a->p->r = b;
			if (a->p->l == a)
				a->p->l = b;
			ft::swap<node_type_base*>(a->p, b->p);
			ft::swap<node_type_base*>(a->r, b->r);
			ft::swap<node_type_base*>(a->l, b->l);
		}
		if (b->r)
			b->r->p = b;
		if (b->l)
			b->l->p = b;
		if (a->r)
			a->r->p = a;
		if (a->l)
			a->l->p = a;
		ft::swap(a->color, b->color);

		if (a->p == &this->super_root)
			this->assign_tree(a);
		if (b->p == &this->super_root)
			this->assign_tree(b);
		if (this->first == a)
			this->first = b;
		else if (this->first == b)
			this->first = a;
		if (this->last == a)
			this->last = b;
		else if (this->last == b)
			this->last = a;
	}

	node_type_base *rotate(node_type_base *n, int direction)
	{
		int opposite = 1 - direction;
		n->ptr() = n->childs[opposite];
		n->childs[opposite]->p = n->p;
		node_type_base *rl = n->childs[opposite]->childs[direction];
		n->childs[opposite]->childs[direction] = n;
		n->p = n->childs[opposite];
		n->childs[opposite] = rl;
		if (rl)
			rl->p = n;
		if (this->super_root.l == n->p)
			this->assign_tree(n->p);
		return n->p;
	}
public:
#ifdef DEBUG_BST_HPP
	template <class T>
	static size_t  length(T) {return 0;};
	static size_t	length(int n) {return n < 10 ? 1 : length<int>(n / 10);}
	static size_t	length(std::string s) {return s.length();}
	bool print() const {
		char tmp[1000] = {};
		if (!this->empty())
			print_node(this->super_root.l, 0, tmp);
		return true;
//		return rb_test();
	}
	bool	rb_test() const
	{
		if (this->super_root.r != this->super_root.l)
		{
			std::cout << "super_root malformed" << std::endl;
			return (0);
		}
		if (is_rb_shaped(this->super_root.l) == -1)
		{
			std::cout << "!!!!!!! NOT IN RB-SHAPE !!!!!!!!" << std::endl;
			return (0);
		}
		return (1);
	}
		int is_rb_shaped(node_type_base *n) const
		{
			if (!n)
				return (0);
			if ((n->p && n->p != &this->super_root) && n->p->color == RED && n->color == RED)
				return (-1);
			if ((n->l && n->l->p != n) || (n->r && n->r->p != n))
			{
				std::cout << "malformed link" << std::endl;
				return  -1;
			}
			int r1 = is_rb_shaped(n->l);
			int r2 = is_rb_shaped(n->r);
			if (r1 == -1 || r2 == -1 || r1 != r2)
				return (-1);
			return (r1 + (n->color == BLACK));
		}
	void print_node(node_type_base* n, int depth, char *tmp = (char[1000]){}) const  {
			if (n->l)
				print_node(n->l, depth + 1, tmp);
			for  (int i = 0; i < depth; i++)
			{
				if (i + 1 == depth)
				{
					if (n->p->l == n)
					{
						std::cout << "      ╔";
						tmp[i] = 1;
					}
					else
					{
						std::cout << "      ╚";
						tmp[i]  = 0;
					}
				}
				else
				{
					if (tmp[i])
						std::cout << "      ║";
					else
						std::cout << "       ";
				}
			}
			if (n->color == RED)
				std::cout << "\033[0;41m";
			if (n->color == BLACK)
				std::cout << "\033[0;0m";
			key_type value = KeyOfValue()(static_cast<node_type*>(n)->value);
			std::cout << value << "\033[0;44m";

			if (n->l || n->r)
				for (size_t i = 0; i + length(value) < 6; i++)
					std::cout << "═";

			if (n->r && n->l)
			{
				std::cout << "╣";
				tmp[depth] = 1;
			}
			if (n->l && !n->r)
			{
				std::cout << "╝";
				tmp[depth] = 0;
			}
			if (n->r && !n->l)
			{
				tmp[depth] = 1;
				std::cout << "╗";
			}
			std::cout  << std::endl;

			if (n->r)
				print_node(n->r, depth + 1, tmp);
		}
#endif
};

template<
		class	Key,
		class	Value,
		class	Compare,
		class	Allocator,
		bool	multi,
		class	KeyOfValue
>
void swap( bst<Key, Value, Compare, Allocator, multi, KeyOfValue>& lhs,
		   bst<Key, Value, Compare, Allocator, multi, KeyOfValue>& rhs )
{
	lhs.swap(rhs);
}

#endif

