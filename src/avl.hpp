#include <iostream>
#include <string>
#include <sstream>
#include <memory>

template <class TYPE_KEY, class TYPE_VALUE>
struct AVLSlice
{
    TYPE_KEY Key;
    TYPE_VALUE Value;
    /* data */
};

template <class TYPE_KEY, class TYPE_VALUE>

struct AVLNode : public AVLSlice<TYPE_KEY, TYPE_VALUE>
{
    AVLNode *Parent;
    AVLNode *Children[2];
    int Height;
};

template <class TYPE_KEY, class TYPE_VALUE>
class AVL
{

    typedef AVLNode<TYPE_KEY, TYPE_VALUE> NODE;
    typedef AVLSlice<TYPE_KEY, TYPE_VALUE> SLICE;

public:
    AVL()
    {
        this->Root = new NODE();
        this->Root->Height = 0;
        this->Root->Parent = NULL;
        this->size = 0;
    }

    ~AVL()
    {
        // 递归delete Root
        // this->traverse_delete(this->Root);
        // delete this->Root;
    }

private:
    const int L = 0;
    const int R = 1;
    int size;

public:
    NODE *Root;

    inline int Compare(TYPE_KEY  key1, TYPE_KEY  key2)
    {
        if (key1 > key2)
        {
            return 1;
        }
        else if (key1 < key2)
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }

    NODE *newNode(TYPE_KEY  key, TYPE_VALUE  value, NODE* parent)
    {
        NODE *node = new NODE();
        node->Key = key;
        node->Value = value;
        node->Height = 0;
        node->Parent = parent;
        return node;
    };

    int Size() {
        return this->size;
    }

 
  	NODE *Get(TYPE_KEY  key)
	{
		NODE *cur = this->get_node(key);
		if (cur != NULL)
		{
			return  cur;
		}
		return NULL;
	}
  

    bool Set(TYPE_KEY  key, TYPE_VALUE value)
    {

        if (this->size == 0)
        {
            this->size++;
            this->Root = newNode(key, value, NULL);
            return true;
        }

        int c = 0;
        NODE *cur = this->Root;
        while (true)
        {
            c = this->Compare(key, cur->Key);
            if (c != -1) {
                if (cur->Children[c] == NULL)
                {
                    this->size++;
                    cur->Children[c] = newNode(key, value, cur);
                    if (cur->Height == 0)
                    {
                        this->fixPutHeight(cur);
                    }
                    return false;
                }
                cur = cur->Children[c];
            }
            else
            {
                cur->Key = key;
                cur->Value = value;
                return true;
            }
        }
    };

    bool Put(TYPE_KEY  key, TYPE_VALUE value)
    {

        if (this->size == 0)
        {
            this->size++;
            this->Root = newNode(key, value, nullptr);
            return true;
        }

        int c = 0;
        NODE *cur = this->Root;
        while (true)
        {
            c = this->Compare(key, cur->Key);
            if (c != -1)
            {
                if (cur->Children[c] == nullptr)
                {
                    this->size++;
                    cur->Children[c] = newNode(key, value, cur);
                    if (cur->Height == 0)
                    {
                        this->fixPutHeight(cur);
                    }
                    return true;
                }
                cur = cur->Children[c];
            }
            else
            {
                return false;
            }
        }
    };

private:
    inline void swap_kv(NODE *n1, NODE *n2)
    {
        TYPE_KEY k1 = n1->Key;
        TYPE_VALUE v1 = n1->Value;
        n1->Key = n2->Key;
        n1->Value = n2->Value;
        n2->Key = k1;
        n2->Value = v1;
    }

    inline NODE *get_node(TYPE_KEY  key)
	{

		NODE *cur = this->Root;
		while (cur != nullptr)
		{
			int c = this->Compare(key, cur->Key);
			if (c != -1)
			{
				cur = cur->Children[c];
			}
			else
			{
				return cur;
			}
		}
		return nullptr;
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

    template <const int l,const int r>
    void double_rotate(NODE *cur)
    {
        NODE *movparent = cur->Children[l];
        NODE *mov = movparent->Children[r];

        swap_kv(mov, cur);

        if (mov->Children[l] != nullptr)
        {
            movparent->Children[r] = mov->Children[l];
            movparent->Children[r]->Parent = movparent;
        }
        else
        {
            movparent->Children[r] = nullptr;
        }

        if (mov->Children[r] != nullptr)
        {
            mov->Children[l] = mov->Children[r];
        }
        else
        {
            mov->Children[l] = nullptr;
        }

        if (cur->Children[r] != nullptr)
        {
            mov->Children[r] = cur->Children[r];
            mov->Children[r]->Parent = mov;
        }
        else
        {
            mov->Children[r] = nullptr;
        }

        cur->Children[r] = mov;
        mov->Parent = cur;

        mov->Height = getMaxChildrenHeight(mov) + 1;
        movparent->Height = getMaxChildrenHeight(movparent) + 1;
        cur->Height = getMaxChildrenHeight(cur) + 1;
    }

    template <const int l,const int r>
    void rotate(NODE *cur)
    {
        // const l = 0
        // const r = 1
        // 1 right 0 left
        NODE *mov = cur->Children[l];

        swap_kv(mov, cur);

        //  mov->children[l]不可能为nil
        mov->Children[l]->Parent = cur;
        cur->Children[l] = mov->Children[l];

        // 解决mov节点孩子转移的问题
        if (mov->Children[r] != nullptr)
        {
            mov->Children[l] = mov->Children[r];
        }
        else
        {
            mov->Children[l] = nullptr;
        }

        if (cur->Children[r] != nullptr)
        {
            mov->Children[r] = cur->Children[r];
            mov->Children[r]->Parent = mov;
        }
        else
        {
            mov->Children[r] = nullptr;
        }

        // 连接转移后的节点 由于mov只是与cur交换值,parent不变
        cur->Children[r] = mov;

        mov->Height = getMaxChildrenHeight(mov) + 1;
        cur->Height = getMaxChildrenHeight(cur) + 1;
        return ;
    }

    inline int getMaxChildrenHeight(NODE *cur)
    {
        int h1 = getHeight(cur->Children[0]);
        int h2 = getHeight(cur->Children[1]);
        if (h1 > h2)
        {
            return h1;
        }
        return h2;
    }

    inline int getHeight(NODE *cur)
    {
        if (cur == nullptr)
        {
            return -1;
        }
        return cur->Height;
    };

    inline void fixPutHeight(NODE* cur )
    {

        while (true)
        {

            int lefth = getHeight(cur->Children[0]);
            int rigthh = getHeight(cur->Children[1]);

            // 计算高度的差值 绝对值大于2的时候需要旋转
            int diff = lefth - rigthh;

            if (lefth < rigthh)
            {
                int hdiff = 1;
                if (diff < -hdiff)
                {
                    NODE *r = cur->Children[1]; // 根据左旋转的右边节点的子节点 左右高度选择旋转的方式
                    if (getHeight(r->Children[0]) > getHeight(r->Children[1]))
                    {
                        // tree->lrrotate(cur)
                        this->double_rotate<1,0>(cur);
                    }
                    else
                    {
                        this->rotate<1,0>(cur);
                    }
                }
                else
                { // 选择一个child的最大高度 + 1为 高度
                    cur->Height = rigthh + 1;
                }
            }
            else if (lefth > rigthh)
            {
                int hdiff = 1;
                if (diff > hdiff)
                {
                    NODE *l = cur->Children[0];
                    if (getHeight(l->Children[1]) > getHeight(l->Children[0]))
                    {
                        // tree->rlrotate(cur) 
                        this->double_rotate<0,1>(cur);
                    }
                    else
                    {
                        this->rotate<0,1>(cur);
                    }
                }
                else
                { // 选择一个child的最大高度 + 1为 高度
                    cur->Height = lefth + 1;
                }
            }

            if (cur->Parent == nullptr || cur->Height < cur->Parent->Height)
            {
                return;
            };
            cur = cur->Parent;
        }
    };
};