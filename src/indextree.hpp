#include <iostream>
#include <string>
#include <sstream>
#include <memory>

class limit_table {
public: 

	struct height_limit_size {
		int	rootsize;
	 	int bottomsize; 
	};

	height_limit_size table[64];

	limit_table() {

			 
	for (int i = 2; i < 64; i++) {
		int root2nsize = 1 << i ;
		int bottomsize = root2nsize >> 1;
 
		for ( int x = 3; x < 64; x++) {
			int rsize = root2nsize >> x;
			if (rsize == 0) {
				break;
			};
			bottomsize -= rsize;
		}

		 table[i].rootsize = root2nsize;
		 table[i].bottomsize = bottomsize;
	};
 
	};

};

limit_table table;

template <class TYPE_KEY, class TYPE_VALUE>
struct Slice
{
	TYPE_KEY Key;
	TYPE_VALUE Value;
	/* data */
};

template <class TYPE_KEY, class TYPE_VALUE>
struct Node : public Slice<TYPE_KEY, TYPE_VALUE>
{
	Node *Parent;
	Node *Children[2];
	int Size;
};

template <class TYPE_KEY, class TYPE_VALUE>
class IndexTree
{

	typedef Node<TYPE_KEY, TYPE_VALUE> NODE;
	typedef Slice<TYPE_KEY, TYPE_VALUE> SLICE;

public:
	IndexTree()
	{
		this->Root = new NODE();
		this->Root->Size = 1;
		this->Root->Parent = nullptr;
	}

	~IndexTree()
	{
		// 递归delete Root
		// this->traverse_delete(this->Root);
		// delete this->Root;
	}

	NODE *Root;

public:
	inline int Compare(const TYPE_KEY & key1,const TYPE_KEY & key2)
	{
		if (key1 > key2)
		{
			return 1;
		}
		else if (key1 < key2)
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}

	int Size()
	{
		NODE *root = this->get_root();
		if (root != nullptr)
		{
			return root->Size;
		}
		return 0;
	}

	SLICE *Get(const TYPE_KEY & key)
	{
		NODE *cur = this->get_node(key);
		if (cur != nullptr)
		{
			return (SLICE *)cur;
		}
		return nullptr;
	}

	void DebugLog()
	{
		std::string str;
		NODE *root = this->get_root();
		if (root == nullptr)
		{
			std::cout << str + "nil" << std::endl;
			return;
		}
		this->outputfordebug(root, "", true, &str);
		std::cout << str << std::endl;
	}

	bool Set(TYPE_KEY & key, TYPE_VALUE & value)
	{

		NODE *cur = this->get_root();
		if (cur == nullptr)
		{
			this->Root->Children[L] = this->create_default_node(this->Root, key, value);
			return true;
		}

		while (true)
		{
			int c = this->Compare(key, cur->Key);
			if (c < 0)
			{
				if (cur->Children[L] != nullptr)
				{
					cur = cur->Children[L];
				}
				else
				{
					cur->Children[L] = this->create_default_node(cur, key, value);
					this->fix_put(cur);
					return true;
				}
			}
			else if (c > 0)
			{
				if (cur->Children[R] != nullptr)
				{
					cur = cur->Children[R];
				}
				else
				{
					cur->Children[R] = this->create_default_node(cur, key, value);
					this->fix_put(cur);
					return true;
				}
			}
			else
			{
				cur->Key = key;
				cur->Value = value;
				return false;
			}
		}
	}

private:
	const int L = 0;
	const int R = 1;

	inline NODE *get_root()
	{
		return this->Root->Children[L];
	}

	inline NODE *create_default_node(NODE *parent, TYPE_KEY & key, TYPE_VALUE & value)
	{
		NODE *node = new NODE();
		node->Key = key;
		node->Value = value;
		node->Size = 1; 
		node->Parent = parent;
		return node;
	}

	inline NODE *get_node(const TYPE_KEY & key)
	{

		NODE *cur = this->get_root();
		while (cur != nullptr)
		{
			int c = this->Compare(key, cur->Key);
			if (c < 0)
			{
				cur = cur->Children[L];
			}
			else if (c > 0)
			{
				cur = cur->Children[R];
			}
			else
			{
				return cur;
			}
		}
		return nullptr;
	}

	void outputfordebug(NODE *node, std::string prefix, bool isTail, std::string *str)
	{
		if (node->Children[1] != nullptr)
		{
			std::string newPrefix(prefix);
			if (isTail)
			{
				newPrefix += "\033[34m│   \033[0m";
			}
			else
			{
				newPrefix += "    ";
			}
			outputfordebug(node->Children[1], newPrefix, false, str);
		}

		*str += prefix;
		if (isTail)
		{
			*str += "\033[34m└── \033[0m";
		}
		else
		{
			*str += "\033[31m┌── \033[0m";
		}

		std::string suffix("(");
		std::string parentv;
		if (node->Parent == nullptr)
		{
			parentv = "nil";
		}
		else
		{
			std::ostringstream k;
			k << node->Parent->Key;
			parentv = k.str();
		}

		std::ostringstream s;
		s << node->Size;
		suffix += parentv + "|" + s.str() + ")";

		std::ostringstream k;
		k << node->Key;
		*str += k.str() + suffix + "\n";

		if (node->Children[0] != nullptr)
		{
			std::string newPrefix(prefix);
			if (isTail)
			{
				newPrefix += "    ";
			}
			else
			{
				newPrefix += "\033[31m│   \033[0m";
			}
			outputfordebug(node->Children[0], newPrefix, true, str);
		}
	}

	inline void traverse_delete(NODE *Current)
	{
		if (Current->Children[L] != nullptr)
		{
			traverse_delete(Current->Children[L]);
			delete Current->Children[L];
		}

		if (Current->Children[R] != nullptr)
		{
			traverse_delete(Current->Children[R]);
			delete Current->Children[R];
		}
	}

 
	inline void fix_put(NODE *cur)
	{

		cur->Size++;
		if (cur->Size == 3)
		{
			this->fix_put_size(cur->Parent);
			return;
		}

		int height = 2;
		int lsize, rsize ;
 
		NODE *parent;
		cur = cur->Parent;

		while (cur != this->Root)
		{
			cur->Size++;
			parent = cur->Parent;

			limit_table::height_limit_size &  limitsize = table.table[height];
 
			if (cur->Size <= limitsize.rootsize)
			{
				lsize = get_child_size(cur->Children[L]);
				rsize = get_child_size(cur->Children[R]);
				if (rsize > lsize)
				{
					if (rsize - lsize >= limitsize.bottomsize)
					{
						this->size_right_rotate(cur);
						fix_put_size(parent);
						return ;
					}
				}
				else
				{
					if (lsize - rsize >= limitsize.bottomsize)
					{
						this->size_left_rotate(cur);
						fix_put_size(parent);
						return ;
					}
				}
			}

			height++;
			cur = parent;
		}
	}

	inline void fix_put_size(NODE *cur)
	{
		while (cur != this->Root)
		{
			cur->Size++;
			cur = cur->Parent;
		};
	}

	inline int get_child_size(NODE *cur)
	{
		if (cur == nullptr)
		{
			return 0;
		}
		return cur->Size;
	}

	inline NODE *size_right_rotate(NODE *cur)
	{
		NODE* child = cur->Children[R];
		int lsize = get_child_size(child->Children[L]);
		int rsize = get_child_size(child->Children[R]);

		if (lsize > rsize)
		{
			this->rotate<0, 1>(child);
		}
		return this->rotate<1, 0>(cur);
	}

	inline NODE *size_left_rotate(NODE *cur)
	{
		NODE* child = cur->Children[L];
		int lsize = get_child_size(child->Children[L]);
		int rsize = get_child_size(child->Children[R]);
		if (lsize < rsize)
		{
			this->rotate<1, 0>(child);
		}
		return this->rotate<0, 1>(cur);
	}

	// L = 0, R = 1 右旋. 反之 左旋
	template <const int L, const int R>
	inline NODE *rotate(NODE *cur)
	{

		NODE *mov = cur->Children[L];
		NODE *movright = mov->Children[R];

		if (cur->Parent->Children[L] == cur)
		{
			cur->Parent->Children[L] = mov;
		}
		else
		{
			cur->Parent->Children[R] = mov;
		}
		mov->Parent = cur->Parent;

		if (movright != nullptr)
		{
			cur->Children[L] = movright;
			movright->Parent = cur;
		}
		else
		{
			cur->Children[L] = nullptr;
		}

		mov->Children[R] = cur;
		cur->Parent = mov;

		cur->Size = get_children_sum_size(cur) + 1;
		mov->Size = get_children_sum_size(mov) + 1;

		return mov;
	}

	inline int get_children_sum_size(NODE *cur)
	{
		return this->get_child_size(cur->Children[L]) + this->get_child_size(cur->Children[R]);
	}
};



