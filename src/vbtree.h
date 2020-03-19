

#include <iostream>
#include <map>
#include <tuple>
#include <vector>

using namespace std;
using std::cout, std::endl;
using std::tuple, std::make_tuple;

typedef unsigned long ULONG;

template <class TKey, class TValue>
class VBTree
{
public:
	typedef int (*Compare)(TKey, TKey);
	struct TreeNode
	{
		TreeNode *children[2];
		TreeNode *parent;
		unsigned long size;
		TKey key;
		TValue value;

		TreeNode()
		{
			this->children[0] = NULL;
			this->children[1] = NULL;
			this->parent = NULL;
			this->size = 1;
		}
		/* data */
	};

private:
	TreeNode *root;
	Compare compare;
	ULONG getSize(TreeNode *cur)
	{
		if (cur == NULL)
		{
			return 0;
		}
		return cur->size;
	}

	ULONG getChildrenSumSize(TreeNode *cur)
	{
		return this->getSize(cur->children[0]) + this->getSize(cur->children[1]);
	}

	tuple<ULONG, ULONG> getChildrenSize(TreeNode *cur)
	{
		return make_tuple(this->getSize(cur->children[0]), this->getSize(cur->children[1]));
	}

	void swapNodeKV(TreeNode *n1, TreeNode *n2)
	{
		TKey tempkey = n1->key;
		TValue tempvalue = n1->value;

		n1->key = n2->key; //交换值达到, 相对位移
		n1->value = n2->value;

		n2->key = tempkey;
		n2->value = tempvalue;
	}

	template<int l, int r>
	void xxrotate3(TreeNode *cur) { //
		TreeNode *movparent = cur->children[l];
		TreeNode *mov = movparent->children[r];

		this->swapNodeKV(mov, cur);

		cur->children[r] = mov;
		mov->parent = cur;

		cur->children[l] = movparent;
		movparent->children[r] = NULL;

		cur->children[r] = mov;
		mov->parent = cur;

		cur->children[l]->size = 1;
	}

	void rlrotate3(TreeNode *cur)
	{
		const int l = 0;
		const int r = 1;

		TreeNode *movparent = cur->children[l];
		TreeNode *mov = movparent->children[r];

		this->swapNodeKV(mov, cur);

		cur->children[r] = mov;
		mov->parent = cur;

		cur->children[l] = movparent;
		movparent->children[r] = NULL;

		cur->children[r] = mov;
		mov->parent = cur;

		cur->children[l]->size = 1;
	}

	template<int l, int r>
	void xxrotate(TreeNode *cur) {
		TreeNode *movparent = cur->children[l];
		TreeNode *mov = movparent->children[r];

		this->swapNodeKV(mov, cur);

		if (mov->children[l] != NULL)
		{
			movparent->children[r] = mov->children[l];
			movparent->children[r]->parent = movparent;
		}
		else
		{
			movparent->children[r] = NULL;
		}

		if (mov->children[r] != NULL)
		{
			mov->children[l] = mov->children[r];
		}
		else
		{
			mov->children[l] = NULL;
		}

		if (cur->children[r] != NULL)
		{
			mov->children[r] = cur->children[r];
			mov->children[r]->parent = mov;
		}
		else
		{
			mov->children[r] = NULL;
		}

		cur->children[r] = mov;
		mov->parent = cur;

		movparent->size = this->getChildrenSumSize(movparent) + 1;
		mov->size = this->getChildrenSumSize(mov) + 1;
		cur->size = this->getChildrenSumSize(cur) + 1;
	}

	template<int l, int r>
	void xrotate3(TreeNode *cur) {
		TreeNode *mov = cur->children[l];

		this->swapNodeKV(mov, cur);

		cur->children[r] = mov;

		cur->children[l] = mov->children[l];
		cur->children[l]->parent = cur;

		mov->children[l] = NULL;

		mov->size = 1;		
	}

	void rrotate3(TreeNode *cur) // 0
	{ 
		const int l = 0; 
		const int r = 1; 
		// 1 right 0 left
		TreeNode *mov = cur->children[l];

		this->swapNodeKV(mov, cur);

		cur->children[r] = mov;

		cur->children[l] = mov->children[l];
		cur->children[l]->parent = cur;

		mov->children[l] = NULL;

		mov->size = 1;
	}

	template<int l, int r>
	void xrotate(TreeNode *cur) {
		// 1 right 0 left
		TreeNode *mov = cur->children[l];

		this->swapNodeKV(mov, cur);

		//  mov->children[l]不可能为nil
		mov->children[l]->parent = cur;

		cur->children[l] = mov->children[l];

		// 解决mov节点孩子转移的问题
		if (mov->children[r] != NULL)
		{
			mov->children[l] = mov->children[r];
		}
		else
		{
			mov->children[l] = NULL;
		}

		if (cur->children[r] != NULL)
		{
			mov->children[r] = cur->children[r];
			mov->children[r]->parent = mov;
		}
		else
		{
			mov->children[r] = NULL;
		}

		// 连接转移后的节点 由于mov只是与cur交换值,parent不变
		cur->children[r] = mov;

		mov->size = getChildrenSumSize(mov) + 1;
		cur->size = getChildrenSumSize(cur) + 1;		
	}

	void fixSize(TreeNode *cur, ULONG ls, ULONG rs)
	{
		if (ls > rs)
		{
			tuple<ULONG, ULONG> lllr = this->getChildrenSize(cur->children[0]);
			ULONG llsize = std::get<0>(lllr);
			ULONG lrsize = std::get<1>(lllr);
			if (lrsize > llsize)
			{
				this->xxrotate<0, 1>(cur);
				// this->rlrotate(cur);
			}
			else
			{
				this->xrotate<0, 1>(cur);
				// this->rrotate(cur);
			}
		}
		else
		{
			tuple<ULONG, ULONG> rlrr = this->getChildrenSize(cur->children[1]);
			ULONG rlsize = std::get<0>(rlrr);
			ULONG rrsize = std::get<1>(rlrr);
			if (rlsize > rrsize)
			{
				this->xxrotate<1, 0>(cur);
				// this->lrrotate(cur);
			}
			else
			{
				this->xrotate<1, 0>(cur);
				// this->lrotate(cur);
			}
		}
	}

public:
	VBTree(Compare compare)
	{
		this->compare = compare;
		this->root = NULL;
	}

	TValue* get(TKey key)
	{
		for (TreeNode *n = this->root; n != NULL;)
		{
			int c = this->compare(key, n->key);
			switch (c)
			{
			case -1:
				n = n->children[0];
				break;
			case 1:
				n = n->children[1];
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
				unsigned long factor = cur->size / 10;
				unsigned long ls = cur->children[0]->size;
				unsigned long rs = cur->children[1]->size;
				if (rs >= ls * 2 + factor || ls >= rs * 2 + factor)
				{
					this->fixSize(cur, ls, rs);
				}
			}

			cur->size++;

			int c = this->compare(key, cur->key);
			if (c < 0)
			{
				if (cur->children[0] == NULL)
				{
					TreeNode *node = new TreeNode();
					node->key = key;
					node->value = value;

					cur->children[0] = node;
					node->parent = cur;

					if (cur->parent != NULL && cur->parent->size == 3)
					{
						if (cur->parent->children[0] == NULL)
						{
							this->xxrotate3<1, 0>(cur->parent);
							// this->lrrotate3(cur->parent);
						}
						else
						{
							this->xrotate3<0, 1>(cur->parent);
							// this->rrotate3(cur->parent);
						}
					}
					return;
				}
				cur = cur->children[0];
			}
			else  
			{
				if (cur->children[1] == NULL)
				{
					TreeNode *node = new TreeNode();
					node->key = key;
					node->value = value;

					cur->children[1] = node;
					node->parent = cur;

					if (cur->parent != NULL && cur->parent->size == 3)
					{
						if (cur->parent->children[1] == NULL)
						{
							this->xxrotate3<0, 1>(cur->parent);
							// this->rlrotate3(cur->parent);
						}
						else
						{
							this->xrotate3<1, 0>(cur->parent);
							// this->lrotate3(cur->parent);
						}
					}
					return;
				}
				cur = cur->children[1];
			}
		}

		cout << "错误" << endl;
		return ;
	};

	/* data */
};
