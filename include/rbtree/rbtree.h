#ifndef RBTREE_H__
#define RBTREE_H__

#include <iostream>
#include <memory>
#include <list>
#include <string>

using namespace std;
// red-black tree

const auto RED = 0;
const auto BLACK = 1;
const auto NULL_NODE = -100;
const auto LEFT_CHILD = 0;
const auto RIGHT_CHILD = 1;


template <typename TKEY, typename TVALUE>
struct rb_slice
{
    TKEY key;
    TVALUE value;
};

template<typename TKEY, typename TVALUE>
class rb_tree_node: public rb_slice<TKEY, TVALUE>
{
public:
	rb_tree_node(const TKEY& key, const TVALUE& value, int color)
		: color_(color)
		, parent_(nullptr)
		, l_child_(nullptr)
		, r_child_(nullptr)
	{
		this->key = key;
		this->value = value;
	}

public:
	int		color_; 
	rb_tree_node*	parent_;
	rb_tree_node*	l_child_;
	rb_tree_node*	r_child_;
};

template<typename TKEY,typename TVALUE>
class rb_tree
{
public:
	
	typedef rb_slice<TKEY, TVALUE> node_slice;
	typedef rb_tree_node<TKEY, TVALUE>	node_type;
	typedef rb_tree_node<TKEY, TVALUE>*	node_pointer;
	typedef const node_pointer	const_node_pointer;

public:
	int size;

	rb_tree()
	: root_(nullptr)
	{
		size = 0;
	}

	node_slice* get(TKEY key) {
		node_pointer cur = this->root_;
		while (cur != nullptr)
		{
			 if( key < cur->key ) {
				 cur = cur->l_child_;
			 } else if (key > cur->key) {
				 cur = cur->r_child_;
			 } else {
				 return (node_slice *)cur;
			 }
		}
		return nullptr;
	}

	int insert(TKEY key, TVALUE value)
	{
		if (nullptr == this->root_)
		{
			this->size++;
			this->root_ = new node_type(key, value, BLACK);
			return 0;
		}

		return insert_node(nullptr, LEFT_CHILD, this->root_, key, value);
	}

	int erase(TKEY data)
	{
		if (nullptr == this->root_)
			return -1;
		return erase_node(nullptr, LEFT_CHILD, this->root_, data);
	}

	void dump()
	{
		mid_visit_node(this->root_);
	}

	int height()
	{
		return this->height_i(this->root_);
	}

	void check_rb_feature_5()
	{
		std::list<int> list;
		char trace[100] = { 0 };
		check_rb_feature_5_impl(nullptr, this->root_, trace, 0, list);
		int len = -1;
		for (auto it = list.begin(); it != list.end(); ++it)
		{
			if (-1 == len)
				len = *it;
			else if (len != *it)
			{
				std::cout << "check_rb_feature_5 failed: " << std::endl;
				return;
			}
		}	

		std::cout << "check_rb_feature_5 success, black node count:  " << len << std::endl;
	}
	
private:
	int insert_node(node_pointer pp, int pp_child_tag, node_pointer p, TKEY key, TVALUE value)
	{
		if (nullptr == p)
		{
			p = new node_type(key, value, RED);
			this->size++;
			LEFT_CHILD == pp_child_tag ? pp->l_child_ = p : pp->r_child_ = p;
			p->parent_ = pp;
			if (RED == pp->color_)
				this->adjust_balance(p);
			return 0;
		}
		else if (p->key == key) {
			p->key = key;
			p->value = value;
			return -1;
		} else if (key < p->key)
			return this->insert_node(p, LEFT_CHILD, p->l_child_, key, value);
		else
			return this->insert_node(p, RIGHT_CHILD, p->r_child_, key, value);
	}

	int erase_node(node_pointer pp, int pp_child_tag, node_pointer p, TKEY key)
	{
		if (nullptr == p)
			return -1;
		else if (p->key == key)
		{		
			this->size--;

			node_pointer fake_null = nullptr;
			int delete_node_color = p->color_;
			node_pointer & ref_p = (pp ? (LEFT_CHILD == pp_child_tag ? pp->l_child_ : pp->r_child_) : this->root_);

			if (p->l_child_ && p->r_child_)
			{	
				auto prev = p->l_child_;
				auto curr = prev;
				for (; curr && curr->r_child_; prev = curr, curr = curr->r_child_);
				delete_node_color = curr->color_;
				if (curr == prev)
				{						
					p->key = curr->key;
					p->l_child_ = curr->l_child_;
					if (curr->l_child_)
						curr->l_child_->parent_ = p;
					pp = p;
					p = p->l_child_;
					pp_child_tag = LEFT_CHILD;	
					delete curr;
				}
				else
				{
					p->key = curr->key;
					pp = prev;
					pp_child_tag = RIGHT_CHILD;
					p = prev->r_child_ = curr->l_child_;
					if (curr->l_child_)
						curr->l_child_->parent_ = prev;
					delete curr;
				}
			}
			else if (p->l_child_)
			{				
				ref_p = p->l_child_;
				p->l_child_->parent_ = pp;
				delete p;
				p = ref_p;
			}
			else if (p->r_child_)
			{				
				ref_p = p->r_child_;
				p->r_child_->parent_ = pp;
				delete p;
				p = ref_p;	
			}
			else
			{
				p = ref_p = nullptr;
			}
									
			if (BLACK == delete_node_color && pp)
			{
				if (nullptr == p)
				{
					fake_null = p = new node_type(NULL_NODE, BLACK);
					p->parent_ = pp;
					LEFT_CHILD == pp_child_tag ? pp->l_child_ = p : pp->r_child_ = p;
				}

				this->adjust_delete_balance(pp, p);
				if (fake_null)
				{
					if (fake_null->parent_->l_child_ == fake_null)
						fake_null->parent_->l_child_ = nullptr;
					else 
						fake_null->parent_->r_child_ = nullptr;
					delete fake_null;
				}
			}
			return 0;
		}
		else if (key < p->key)
			return this->erase_node(p, LEFT_CHILD, p->l_child_, key);
		else
			return this->erase_node(p, RIGHT_CHILD, p->r_child_, key);
	}

private:
	void adjust_balance(node_pointer p)
	{
		while (p->parent_ && p->parent_->parent_ && RED == p->parent_->color_)
		{
			auto pp = p->parent_;
			auto gp = pp->parent_;
			auto up = (gp->l_child_ == pp ? gp->r_child_ : gp->l_child_);
			if (gp->l_child_ == pp)
			{
				if (up && RED == up->color_)
				{
					pp->color_ = up->color_ = BLACK;
					gp->color_ = RED;
					p = gp;
				}
				else
				{
					if (pp->r_child_ == p)
					{
						this->l_rotate(pp);
						swap(p, pp);
					}

					pp->color_ = BLACK;
					gp->color_ = RED;
					this->r_rotate(gp);
				}
			}
			else
			{
				if (up && RED == up->color_)
				{
					pp->color_ = up->color_ = BLACK;
					gp->color_ = RED;
					p = gp;
				}
				else
				{
					if (pp->l_child_ == p)
					{
						this->r_rotate(pp);
						swap(p, pp);
					}

					pp->color_ = BLACK;
					gp->color_ = RED;
					this->l_rotate(gp);
				}
			}
		}

		this->root_->color_ = BLACK;
	}

	void adjust_delete_balance(node_pointer pp, node_pointer p)
	{
		while ( (nullptr == p || BLACK == p->color_) && p != this->root_)
		{
			if (pp->l_child_ == p)
			{
				auto s = pp->r_child_;
				if (s && RED == s->color_)
				{
					s->color_ = BLACK;
					pp->color_ = RED;
					this->l_rotate(pp);
					s = pp->r_child_;				
				}

				if ((nullptr == s->l_child_ || BLACK == s->l_child_->color_)
					&& (nullptr == s->r_child_ || BLACK == s->r_child_->color_))
				{
					s->color_ = RED;
					p = pp;
					pp = p->parent_;
				}
				else
				{
					if (nullptr == s->r_child_ || BLACK == s->r_child_->color_)
					{
						s->l_child_ ? s->l_child_->color_ = BLACK : 0;
						s->color_ = RED;
						this->r_rotate(s);
						s = pp->r_child_;
					}

					s->color_ = pp->color_;
					pp->color_ = BLACK;
					s->r_child_->color_ = BLACK;
					this->l_rotate(pp);
					p = this->root_;
				}
			}
			else
			{
				auto s = pp->l_child_;
				if (s && RED == s->color_)
				{
					s->color_ = BLACK;
					pp->color_ = RED;
					this->r_rotate(pp);
					s = pp->l_child_;
				}

				if ((nullptr == s->l_child_ || BLACK == s->l_child_->color_)
					&& (nullptr == s->r_child_ || BLACK == s->r_child_->color_))
				{
					s->color_ = RED;
					p = pp;
					pp = p->parent_;
				}
				else
				{
					if (nullptr == s->l_child_ || BLACK == s->l_child_->color_)
					{
						s->r_child_ ? s->r_child_->color_ = BLACK : 0;
						s->color_ = RED;
						this->l_rotate(s);
						s = pp->l_child_;
					}

					s->color_ = pp->color_;
					pp->color_ = BLACK;
					s->l_child_->color_ = BLACK;
					this->r_rotate(pp);
					p = this->root_;
				}
			}
		}

		p->color_ = BLACK;
	}

private:
	node_pointer l_rotate(node_pointer p)
	{
		auto lr = p->r_child_;
		p->r_child_ = lr->l_child_;
		if (lr->l_child_)
			lr->l_child_->parent_ = p;
		
		lr->parent_ = p->parent_;
		if (nullptr == p->parent_)
			this->root_ = lr;
		else if (p->parent_->l_child_ == p)
			p->parent_->l_child_ = lr;
		else
			p->parent_->r_child_ = lr;

		lr->l_child_ = p;
		p->parent_ = lr;

		return lr;
	}

	node_pointer r_rotate(node_pointer p)
	{
		auto lc = p->l_child_;
		p->l_child_ = lc->r_child_;
		if (lc->r_child_)
			lc->r_child_->parent_ = p;

		lc->parent_ = p->parent_;
		if (nullptr == p->parent_)
			this->root_ = lc;
		else if (p->parent_->l_child_ == p)
			p->parent_->l_child_ = lc;
		else
			p->parent_->r_child_ = lc;

		lc->r_child_ = p;
		p->parent_ = lc;

		return lc;
	}

	int height_i(node_pointer p)
	{
		if (nullptr == p)
			return 0;

		int left_height = 0;
		if (p->l_child_)
			left_height = height_i(p->l_child_) + 1;

		int right_height = 0;
		if (p->r_child_)
			right_height = height_i(p->r_child_) + 1;

		return left_height >= right_height ? left_height : right_height;
	}

	void mid_visit_node(const_node_pointer p)
	{
		if (nullptr != p)
		{
			mid_visit_node(p->l_child_);
			std::cout << p->data_ << std::endl;
			mid_visit_node(p->r_child_);
		}
	}

	void check_rb_feature_5_impl(node_pointer pp, node_pointer p, char trace[], int idx, std::list<int>& list)
	{
		if (nullptr == p)
		{
			trace[idx] = BLACK;
			int result = 0;
			for (auto k = 0; k <= idx; ++k)
			{
				if (BLACK == trace[k])
					++result;
			}

			list.push_back(result);
		}
		else
		{
			trace[idx] = p->color_;
			check_rb_feature_5_impl(p, p->l_child_, trace, idx + 1, list);
			check_rb_feature_5_impl(p, p->r_child_, trace, idx + 1, list);
		}
	}

private:
	node_pointer	root_;
};

#endif

