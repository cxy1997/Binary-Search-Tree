/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"
#include <iostream>
#include <queue>

namespace sjtu {

template<class Key,class T,class Compare = std::less<Key> > 
class map 
{
public:
	typedef pair<const Key, T> value_type;
private:
	Compare compare;
	struct node
	{
		value_type *storage;
		node *left, *right;
		int h;
		node():storage(NULL),left(NULL),right(NULL),h(1){}
	};
	node *root;
	int Size;
	void copyNode(const node* from, node*& to)
	{
		if (from != NULL)
		{
			to = new node;
			to->storage = new value_type(*(from->storage));
			to->h = from->h;
			copyNode(from->left, to->left);
			copyNode(from->right, to->right);
		}
		else to = NULL;
	}
	void makeEmpty()
	{
		Size = 0;
		if (root != NULL)
		{
			std::queue<node*> q;
			q.push(root);
			node *p;
			while (!q.empty())
			{
				p = q.front();
				q.pop();
				delete p->storage;
				if (p->left != NULL) q.push(p->left);
				if (p->right != NULL) q.push(p->right);
				delete p;
			}
			root = NULL;
		}
		/*if (t != NULL)
		{
			makeEmpty(t->left);
			makeEmpty(t->right);
			if (t->storage != NULL) delete t->storage;
			delete t;
			t = NULL;
		}*/
	}
	void Remove(node*& t, node* which)
	{
		if (t == which)
		{
			--Size;
			if (t->right == NULL)
			{
				node *tmp = t;
				t = t->left;
				delete tmp->storage;
				delete tmp;
				return;
			}
			if (t->left == NULL)
			{
				node *tmp = t;
				t = t->right;
				delete tmp->storage;
				delete tmp;
				return;
			}
			node *pp = t->right, *qq = NULL;
			while (pp->left != NULL)
			{
				qq = pp;
				pp = pp->left;
			}
			if (qq == NULL)
			{
				delete t->storage;
				t->storage = pp->storage;
				t->right = pp->right;
				delete pp;
			}
			else
			{
				delete t->storage;
				t->storage = pp->storage;
				qq->left = pp->right;
				delete pp;
			}
			return;
		}
		node *q = NULL, *p = t;
		while (p != NULL)
		{
			if (compare(which->storage->first, p->storage->first))
			{
				q = p;
				p = p->left;
			}
			else if (compare(p->storage->first, which->storage->first))
			{
				q = p;
				p = p->right;
			}
			else break;
		}
		if (p == NULL) throw invalid_iterator();
		
		if (p == q->left)
		{
			--Size;
			if (p->right == NULL)
			{
				q->left = q->left->left;
				delete p->storage;
				delete p;
				return;
			}
			if (p->left == NULL)
			{
				q->left = q->left->right;
				delete p->storage;
				delete p;
				return;
			}
			node *pp = p->right, *qq = NULL;
			while (pp->left != NULL)
			{
				qq = pp;
				pp = pp->left;
			}
			if (qq == NULL)
			{
				delete p->storage;
				p->storage = pp->storage;
				p->right = pp->right;
				delete pp;
			}
			else
			{
				delete p->storage;
				p->storage = pp->storage;
				qq->left = pp->right;
				delete pp;
			}
		} else if (p == q->right)
		{
			--Size;
			if (p->right == NULL)
			{
				q->right = q->right->left;
				delete p->storage;
				delete p;
				return;
			}
			if (p->left == NULL)
			{
				q->right = q->right->right;
				delete p->storage;
				delete p;
				return;
			}
			node *pp = p->right, *qq = NULL;
			while (pp->left != NULL)
			{
				qq = pp;
				pp = pp->left;
			}
			if (qq == NULL)
			{
				delete p->storage;
				p->storage = pp->storage;
				p->right = pp->right;
				delete pp;
			}
			else
			{
				delete p->storage;
				p->storage = pp->storage;
				qq->left = pp->right;
				delete pp;
			}
		}
	}
public:
	class const_iterator;
	class iterator {
		friend const_iterator;
	private:
		node *data, *itsRoot;
	public:
		iterator(node* d1=NULL, node* r1 = NULL) {
			data = d1;
			itsRoot = r1;
		}
		iterator(const iterator &other) {
			data = other.data;
			itsRoot = other.itsRoot;
		}
		node* pointer() { return data; }
		node* findRoot() { return itsRoot; }
		iterator operator++(int) 
		{
			iterator tmp = *this;
			++(*this);
			return tmp;
		}
		iterator & operator++() 
		{
			Compare compare;
			if (data->right != NULL) 
			{
				data = data->right;
				while (data->left != NULL) data = data->left;
				return *this;
			}
			else 
			{
				node *q = itsRoot, *suc = NULL;
				while (q != NULL) 
				{
					if (compare(data->storage->first,q->storage->first))
					{
						suc = q;
						q = q->left;
					}
					else if (compare(q->storage->first, data->storage->first))
						q = q->right;
					else
						break;
				}
				data = suc;
				return *this;

			}
		}
		iterator operator--(int) 
		{
			iterator tmp = *this;
			--(*this);
			return tmp;
		}
		iterator & operator--() 
		{
			if (data == NULL)
			{
				node *tmp = itsRoot;
				while (tmp->right != NULL) tmp = tmp->right;
				data = tmp;
				return *this;
			}
			Compare compare;
			if (data->left != NULL) 
			{
				data = data->left;
				while (data->right != NULL) data = data->right;
				return *this;
			}
			else 
			{
				node *q = itsRoot, *suc = NULL;
				while (q != NULL) 
				{
					if (compare(q->storage->first, data->storage->first)) 
					{
						suc = q;
						q = q->right;
					}
					else if (compare(data->storage->first, q->storage->first))
						q = q->left;
					else
						break;
				}
				data = suc;
				return *this;
			}
		}
		value_type & operator*() const 
		{
			return *(data->storage);
		}
		bool operator==(const iterator &rhs) const 
		{
			if (itsRoot != rhs.itsRoot) return false;
			return (data == rhs.data);
		}
		bool operator==(const const_iterator &rhs) const 
		{
			if (itsRoot != rhs.itsRoot) return false;
			return (data == rhs.data);
		}
		bool operator!=(const iterator &rhs) const 
		{
			return (itsRoot!= rhs.itsRoot || data!=rhs.data);
		}
		bool operator!=(const const_iterator &rhs) const 
		{
			return (itsRoot != rhs.itsRoot || data != rhs.data);
		}
		/**
		 * for the support of it->first. 
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		value_type* operator->() const noexcept 
		{
			return data->storage;
		}
	};
	class const_iterator {
		friend iterator;
		private:
			node *data, *itsRoot;
		public:
			const_iterator(node* d1 = NULL, node* r1 = NULL) {
				data = d1;
				itsRoot = r1;
			}
			const_iterator(const const_iterator &other) {
				data = other.data;
				itsRoot = other.itsRoot;
			}
			const_iterator(const iterator &other) {
				data = other.data;
				itsRoot = other.itsRoot;
			}
			node* pointer() { return data; }
			node* findRoot() { return itsRoot; }
			const_iterator operator++(int)
			{
				const_iterator tmp = *this;
				++(*this);
				return tmp;
			}
			const_iterator & operator++()
			{
				Compare compare;
				if (data->right != NULL)
				{
					data = data->right;
					while (data->left != NULL) data = data->left;
					return *this;
				}
				else
				{
					node *q = itsRoot, *suc = NULL;
					while (q != NULL)
					{
						if (compare(data->storage->first, q->storage->first))
						{
							suc = q;
							q = q->left;
						}
						else if (compare(q->storage->first, data->storage->first))
							q = q->right;
						else
							break;
					}
					data = suc;
					return *this;

				}
			}
			const_iterator operator--(int)
			{
				const_iterator tmp = *this;
				--(*this);
				return tmp;
			}
			const_iterator & operator--()
			{
				if (data == NULL)
				{
					for (data = itsRoot; data->right != NULL; data = data->right);
					return *this;
				}
				Compare compare;
				if (data->left != NULL)
				{
					data = data->left;
					while (data->right != NULL) data = data->right;
					return *this;
				}
				else
				{
					node *q = itsRoot, *suc = NULL;
					while (q != NULL)
					{
						if (compare(q->storage->first, data->storage->first))
						{
							suc = q;
							q = q->right;
						}
						else if (compare(data->storage->first, q->storage->first))
							q = q->left;
						else
							break;
					}
					data = suc;
					return *this;
				}
			}
			value_type & operator*() const
			{
				return *(data->storage);
			}
			bool operator==(const iterator &rhs) const
			{
				if (itsRoot != rhs.itsRoot) return false;
				return (data == rhs.data);
			}
			bool operator==(const const_iterator &rhs) const
			{
				if (itsRoot != rhs.itsRoot) return false;
				return (data == rhs.data);
			}
			bool operator!=(const iterator &rhs) const
			{
				return (itsRoot != rhs.itsRoot || data != rhs.data);
			}
			bool operator!=(const const_iterator &rhs) const
			{
				return (itsRoot != rhs.itsRoot || data != rhs.data);
			}
			value_type* operator->() const noexcept
			{
				return data->storage;
			}
	};
	map():root(NULL),Size(0) {}
	map(const map &other) 
	{
		Size = other.Size;
		copyNode(other.root, root);
	}
	map & operator=(const map &other) 
	{
		if (this == &other) return *this;
		Size = other.Size;
		if (root != NULL) makeEmpty();
		copyNode(other.root, root);
		return *this;
	}
	~map() 
	{
		clear();
	}
	iterator add(node* t, const Key& k)
	{
		T value;
		if (root == NULL)
		{
			++Size;
			root = new node;
			root->storage = new value_type(k,value);
			return iterator(root, root);
		}
		node *tmp = NULL;
		while (t != NULL)
		{
			tmp = t;
			if (compare(k, t->storage->first)) t=t->left;
			else t = t->right;
		}
		if (compare(k, tmp->storage->first))
		{
			++Size;
			t = tmp->left = new node;
			t->storage = new value_type(k, value);
			return iterator(t, root);
		} else 
	    {
			++Size;
			t = tmp->right = new node;
			t->storage = new value_type(k, value);
			return iterator(t, root);
		}
	}
	T & at(const Key &k) 
	{
		iterator it = find(k);
		if (it == end()) throw index_out_of_bound();
		return it.pointer()->storage->second;
	}
	const T & at(const Key &k) const 
	{
		const_iterator it = find(k);
		if (it == cend()) throw index_out_of_bound();
		return it.pointer()->storage->second;
	}
	T & operator[](const Key &k) 
	{
		iterator it = find(k);
		if (it == end())
		{
			it = add(root, k);
		}
		return it.pointer()->storage->second;
	}
	const T & operator[](const Key &k) const 
	{
		const_iterator it = find(k);
		if (it != cend()) return it.pointer()->storage->second;
		else throw runtime_error();
	}
	iterator begin() 
	{
		if (Size == 0) return iterator(NULL, root);
		node *tmp = root;
		while (tmp->left != NULL) tmp = tmp->left;
		return iterator(tmp, root);
	}
	const_iterator cbegin() const 
	{
		if (Size == 0) return const_iterator(NULL, root);
		node *tmp = root;
		while (tmp->left != NULL) tmp = tmp->left;
		return const_iterator(tmp, root);
	}
	iterator end() 
	{
		return iterator(NULL, root);
	}
	const_iterator cend() const 
	{
		return const_iterator(NULL, root);
	}
	bool empty() const { return (Size == 0); }
	size_t size() const { return Size; }
	void clear() 
	{
		makeEmpty();
	}
	pair<iterator, bool> insert(const value_type &val) 
	{
		iterator tmp = find(val.first);
		if (tmp != end()) return pair<iterator, bool>(tmp, false);
		tmp = add(root, val.first);
		tmp.pointer()->storage->second = val.second;
		return pair<iterator, bool>(tmp, true);
	}
	void erase(iterator pos) 
	{
		if (pos == end() || pos.findRoot()!=root) throw invalid_iterator();
		else Remove(root, pos.pointer());
	}
	size_t count(const Key &k)
	{
		if (find(k) != iterator(NULL,root)) return 1; 
		    else return 0;
	}
	iterator find(const Key &k) 
	{
		node *tmp = root;
		while (tmp != NULL) 
		{
			if (compare(tmp->storage->first, k))
			{
				tmp = tmp->right;
				//std::cout << "right\n";
			}
			else if (compare(k, tmp->storage->first))
			{
				tmp = tmp->left;
				//std::cout << "left\n";
			}
			else
				break;
		}
		return iterator(tmp, root);
	}
	const_iterator find(const Key &k) const 
	{
		node *tmp = root;
		while (tmp != NULL)
		{
			if (compare(tmp->storage->first, k))
				tmp = tmp->right;
			else if (compare(k, tmp->storage->first))
				tmp = tmp->left;
			else
				break;
		}
		return const_iterator(tmp, root);
	}
};

}

#endif
