
#ifndef __VBTREE__
#define __VBTREE__

#include <iostream>
#include <map>
#include <tuple>
#include <vector>
#include "compare.hpp"

using namespace std;
using std::cout, std::endl;
using std::tuple, std::make_tuple;

typedef unsigned long ULONG;

template <class TKey, class TValue, int Compare(TKey, TKey) = compare>
class VBTree
{
public:
    enum CHILDREN {LEFT = 0, RIGHT = 1}; 
public:
	struct TreeNode
	{
		TreeNode *children[2];
		TreeNode *parent;
		unsigned long size;
		TKey key;
		TValue value;

		TreeNode()
		{
			this->children[LEFT] = NULL;
			this->children[RIGHT] = NULL;
			this->parent = NULL;
			this->size = 1;
		}
		/* data */
	};

private:
	TreeNode *root;
	inline ULONG get_size(TreeNode *cur)
	{
		if (cur == NULL)
		{
			return 0;
		}
		return cur->size;
	}

	inline ULONG get_children_sum_size(TreeNode *cur)
	{
		return this->get_size(cur->children[LEFT]) + this->get_size(cur->children[RIGHT]);
	}

	inline tuple<ULONG, ULONG> get_children_size(TreeNode *cur)
	{
		return make_tuple(this->get_size(cur->children[LEFT]), this->get_size(cur->children[RIGHT]));
	}

	inline void swap_keyvalue(TreeNode *n1, TreeNode *n2)
	{
		TKey tempkey = n1->key;
		TValue tempvalue = n1->value;

		n1->key = n2->key; //交换值达到, 相对位移
		n1->value = n2->value;

		n2->key = tempkey;
		n2->value = tempvalue;
	}

	template<int left, int right>
	inline void xxrotate3(TreeNode *cur) { //
		TreeNode *movparent = cur->children[left];
		TreeNode *mov = movparent->children[right];

		this->swap_keyvalue(mov, cur);

		cur->children[right] = mov;
		mov->parent = cur;

		cur->children[left] = movparent;
		movparent->children[right] = NULL;

		cur->children[right] = mov;
		mov->parent = cur;

		cur->children[left]->size = 1;
	}

	template<int left, int right>
	inline void xxrotate(TreeNode *cur) {
		TreeNode *movparent = cur->children[left];
		TreeNode *mov = movparent->children[right];

		this->swap_keyvalue(mov, cur);

		if (mov->children[left] != NULL)
		{
			movparent->children[right] = mov->children[left];
			movparent->children[right]->parent = movparent;
		}
		else
		{
			movparent->children[right] = NULL;
		}

		if (mov->children[right] != NULL)
		{
			mov->children[left] = mov->children[right];
		}
		else
		{
			mov->children[left] = NULL;
		}

		if (cur->children[right] != NULL)
		{
			mov->children[right] = cur->children[right];
			mov->children[right]->parent = mov;
		}
		else
		{
			mov->children[right] = NULL;
		}

		cur->children[right] = mov;
		mov->parent = cur;

		movparent->size = this->get_children_sum_size(movparent) + 1;
		mov->size = this->get_children_sum_size(mov) + 1;
		cur->size = this->get_children_sum_size(cur) + 1;
	}

	template<int left, int right>
	inline void xrotate3(TreeNode *cur) {
		TreeNode *mov = cur->children[left];

		this->swap_keyvalue(mov, cur);

		cur->children[right] = mov;

		cur->children[left] = mov->children[left];
		cur->children[left]->parent = cur;

		mov->children[left] = NULL;

		mov->size = 1;		
	}

	template<int left, int right>
	inline void xrotate(TreeNode *cur) {
		// 1 right 0 left
		TreeNode *mov = cur->children[left];

		this->swap_keyvalue(mov, cur);

		//  mov->children[l]不可能为nil
		mov->children[left]->parent = cur;

		cur->children[left] = mov->children[left];

		// 解决mov节点孩子转移的问题
		if (mov->children[right] != NULL)
		{
			mov->children[left] = mov->children[right];
		}
		else
		{
			mov->children[left] = NULL;
		}

		if (cur->children[right] != NULL)
		{
			mov->children[right] = cur->children[right];
			mov->children[right]->parent = mov;
		}
		else
		{
			mov->children[right] = NULL;
		}

		// 连接转移后的节点 由于mov只是与cur交换值,parent不变
		cur->children[right] = mov;

		mov->size = get_children_sum_size(mov) + 1;
		cur->size = get_children_sum_size(cur) + 1;		
	}

	inline void fix_size(TreeNode *cur, ULONG ls, ULONG rs)
	{
		if (ls > rs)
		{
			tuple<ULONG, ULONG> lllr = this->get_children_size(cur->children[LEFT]);
			ULONG llsize = std::get<LEFT>(lllr);
			ULONG lrsize = std::get<RIGHT>(lllr);
			if (lrsize > llsize)
			{
				this->xxrotate<LEFT, RIGHT>(cur);
			}
			else
			{
				this->xrotate<LEFT, RIGHT>(cur);
			}
		}
		else
		{
			tuple<ULONG, ULONG> rlrr = this->get_children_size(cur->children[RIGHT]);
			ULONG rlsize = std::get<LEFT>(rlrr);
			ULONG rrsize = std::get<RIGHT>(rlrr);
			if (rlsize > rrsize)
			{
				this->xxrotate<RIGHT, LEFT>(cur);
			}
			else
			{
				this->xrotate<RIGHT, LEFT>(cur);
			}
		}
	}

public:
	VBTree()
	{
		this->root = NULL;
	}

	TValue* get(TKey key)
	{
		for (TreeNode *n = this->root; n != NULL;)
		{
			int c = Compare(key, n->key);
			switch (c)
			{
			case -1:
				n = n->children[LEFT];
				break;
			case 1:
				n = n->children[RIGHT];
				break;
			case 0:
				return  &n->value;
			}
		}
		return NULL;
	}

	void put(TKey key, TValue value)
	{

		if (this->root == NULL)
		{
			TreeNode *node = new TreeNode();
			node->key = key;
			node->value = value;
			this->root = node;
			return;
		}


		for (TreeNode *cur = this->root;;)
		{
			if (cur->size > 8)
			{
				unsigned long factor = cur->size >> 3; // 原来是/10 为了简化为位运算/8　
				unsigned long ls = cur->children[LEFT]->size;
				unsigned long rs = cur->children[RIGHT]->size;
				if (rs >= (ls << 1) + factor || ls >= (rs << 1) + factor) // ls rs * 2 
				{
					this->fix_size(cur, ls, rs);
				}
			}

			cur->size++;

			int c = Compare(key, cur->key);
			if (c < 0)
			{
				if (cur->children[LEFT] == NULL)
				{
					TreeNode *node = new TreeNode();
					node->key = key;
					node->value = value;

					cur->children[LEFT] = node;
					node->parent = cur;

					if (cur->parent != NULL && cur->parent->size == 3)
					{
						if (cur->parent->children[LEFT] == NULL)
						{
							this->xxrotate3<RIGHT, LEFT>(cur->parent);
						}
						else
						{
							this->xrotate3<LEFT, RIGHT>(cur->parent);
						}
					}
					return;
				}
				cur = cur->children[LEFT];
			}
			else  
			{
				if (cur->children[RIGHT] == NULL)
				{
					TreeNode *node = new TreeNode();
					node->key = key;
					node->value = value;

					cur->children[RIGHT] = node;
					node->parent = cur;

					if (cur->parent != NULL && cur->parent->size == 3)
					{
						if (cur->parent->children[RIGHT] == NULL)
						{
							this->xxrotate3<LEFT, RIGHT>(cur->parent);
						}
						else
						{
							this->xrotate3<RIGHT, LEFT>(cur->parent);
						}
					}
					return;
				}
				cur = cur->children[RIGHT];
			}
		}

		cout << "error" << endl;
		return ;
	};

	/* data */
};


#endif // __VBTREE__