
#include <iostream>
using namespace std;
 
typedef unsigned long ULONG;

class SBTNode {
public:
	//size:子树大小(就是以当前结点为根构成的树有多少结点)
	//data:权值，就是树上的结点储存的值
	//value:应该是临时储存权值的
    ULONG data, size, value;
    SBTNode * lchild, * rchild, * father;
	//构造函数，参数分别为 权值，以当前结点为根的树的大小，父亲结点
    SBTNode(ULONG init_data, ULONG init_size = 0, SBTNode * init_father = NULL);
    ~SBTNode();
	//下面依次是
	//二叉排序树的插入，搜索，找前驱，找后继，移除某个度为0或1的结点，移除某个权值的点，找出第k大的元素
    void insert(ULONG value);
    SBTNode * search(ULONG value);
    SBTNode * predecessor();
    SBTNode * successor();
    void remove_node(SBTNode * delete_node);
    bool remove(ULONG value);
    ULONG select(ULONG k);
};
 
class BinaryTree {
private:
    SBTNode * root;
public:
    BinaryTree();
    ~BinaryTree();
	//下面依次是
	//二叉树的插入 查找 删除结点 找出第k大的树，都是以上面的结点类的函数为基础的
    void insert(ULONG value);
    bool find(ULONG value);
    bool remove(ULONG value);
    ULONG select(ULONG k);
    ULONG size();
};
 
//这里搞了个权值为0的结点，避免在边界情况时对空指针（NULL）进行特判，所以将所有原本指向空指针的情况都改为指向一个 ZPTR，并将其 size 设置为 0，从而降低代码复杂度。
SBTNode ZERO(0);
SBTNode * ZPTR = &ZERO;
 
SBTNode::SBTNode(ULONG init_data, ULONG init_size, SBTNode * init_father) {
    data = init_data;
    size = init_size;
    lchild = ZPTR;
    rchild = ZPTR;
    father = init_father;
}
 
SBTNode::~SBTNode() {
    if (lchild != ZPTR) {
        delete lchild;
    }
    if (rchild != ZPTR) {
        delete rchild;
    }
}
 
//左旋：将右孩子变为“根结点”（当前子树的根结点），右孩子的左孩子就变成原来的根结点的右孩子
//下面注释中：node（原来的根结点）的右孩子用“根结点”来说
SBTNode * left_rotate(SBTNode * node) {
	//用temp保存“根结点”
    SBTNode * temp = node->rchild;
	//“根结点”的左孩子  变成node（原来的根结点）的右孩子
    node->rchild = temp->lchild;
	//更新“根结点”原来的左孩子的父亲为node（原来的根结点） 
    temp->lchild->father = node;
	//node（原来的根结点） 就变成“根结点”的左孩子
    temp->lchild = node;
	//“根结点”的父亲更新为node（原来的根结点）的父亲
    temp->father = node->father;
	//node（原来的根结点）的父亲更新为“根结点”
    node->father = temp;
	//“根结点”的大小更新为node（原来的根结点）的大小（这里的大小是以该结点为根构成的树的结点的个数）
    temp->size = node->size;
	//node（原来的根结点）的大小更新为 它左孩子和右孩子的大小再在上本身1
    node->size = node->lchild->size + node->rchild->size + 1;
	//返回左旋后的根结点
    return temp;
}
 
//右旋：将左孩子变为“根结点”（当前子树的根结点），左孩子的右孩子就变成原来的根结点的左孩子
//下面注释中：node（原来的根结点）的左孩子用“根结点”来说
//反正这里跟上几乎相反
SBTNode * right_rotate(SBTNode * node) {
	//用temp保存“根结点”
    SBTNode * temp = node->lchild;
	//“根结点”的右孩子  变成node（原来的根结点）的左孩子
    node->lchild = temp->rchild;
	//更新“根结点”原来的右孩子的父亲为node（原来的根结点） 
    temp->rchild->father = node;
	//node（原来的根结点） 就变成“根结点”的右孩子
    temp->rchild = node;
	//“根结点”的父亲更新为node（原来的根结点）的父亲
    temp->father = node->father;
	//node（原来的根结点）的父亲更新为“根结点”
    node->father = temp;
	//“根结点”的大小更新为node（原来的根结点）的大小（这里的大小是以该结点为根构成的树的结点的个数）
    temp->size = node->size;
	//node（原来的根结点）的大小更新为 它左孩子和右孩子的大小再在上本身1
    node->size = node->lchild->size + node->rchild->size + 1;
	//返回右旋后的根结点
    return temp;
}
 
//利用上面的左右旋进行调整的函数
//flag为false：处理左子树更高的情况，否则处理右子树更高的情况
//node:要调整的子树的根结点
SBTNode * maULONGain(SBTNode * node, bool flag) {
	//左子树比右子树高（或者叫深度要深）
    if (flag == false) {
		//LL型：左子树的左子树的元素个数大于右子树的元素个数，应进行右旋
        if (node->lchild->lchild->size > node->rchild->size) {
			//右旋并更新子树的根结点
            node = right_rotate(node);
        }
		//LR型：左子树的右子树的元素个数大于右子树的元素个数
		//那么先对左子树进行左旋，就变成LL型，再右旋即可
		else if (node->lchild->rchild->size > node->rchild->size) {
			//左旋并更新左子树的根结点
            node->lchild = left_rotate(node->lchild);
			//右旋并更新根节点
            node = right_rotate(node);
        } else {
			//说明平衡了，返回根节点
            return node;
        }
	//右子树比左子树高（或者叫深度要深）
    } else {
		//RR型：右子树的右子树的元素个数大于左子树的元素个数,应进行左旋
        if (node->rchild->rchild->size > node->lchild->size) {
			//左旋并更新根节点
            node = left_rotate(node);
        }
		//RL型: 右子树的左子树的元素个数大于左子树的元素个数
		//那么先对右子树进行右旋，变成RR型，在左旋即可
		else if (node->rchild->lchild->size > node->lchild->size) {
			//右旋并更新左子树的根结点
            node->rchild = right_rotate(node->rchild);
			//左旋并更新根节点
            node = left_rotate(node);
        } else {
			//说明平衡了，返回根节点
            return node;
        }
    }
	//下面为递归调用，因为有时上面的调整过后，左子树和右子树的某个结点还是不平衡
 
	//递归调用，处理可能左子树的左子树高度更高的情况
	//false表示左子树较高
    node->lchild = maULONGain(node->lchild, false);
	//其右子树的右子树高度更高的情况
    node->rchild = maULONGain(node->rchild, true);
	//最后再对子树根结点的左右子树递归进行调整
    node = maULONGain(node, false);
    node = maULONGain(node, true);
	//返回调整后的子树的根结点
    return node;
}
 
SBTNode * insert(SBTNode * node, ULONG value) {
    if (value == node->data) {
        return node;
    } else {
        node->size++;
        if (value > node->data) {
            if (node->rchild == ZPTR) {
                node->rchild = new SBTNode(value, 1, node);
            } else {
                node->rchild = insert(node->rchild, value);
            }
        } else {
            if (node->lchild == ZPTR) {
                node->lchild = new SBTNode(value, 1, node);
            } else {
                node->lchild = insert(node->lchild, value);
            }
        }
    }
    return maULONGain(node, value > node->data);
}
 
SBTNode * SBTNode::search(ULONG value) {
    if (data == value) {
        return this;
    } else if (value > data) {
        if (rchild == ZPTR) {
            return ZPTR;
        } else {
            return rchild->search(value);
        }
    } else {
        if (lchild == ZPTR) {
            return ZPTR;
        } else {
            return lchild->search(value);
        }
    }
}
 
SBTNode * SBTNode::predecessor() {
    SBTNode * temp = lchild;
    while (temp != ZPTR && temp->rchild != ZPTR) {
        temp = temp->rchild;
    }
    return temp;
}
 
SBTNode * SBTNode::successor() {
    SBTNode * temp = rchild;
    while (temp != ZPTR && temp->lchild != ZPTR) {
        temp = temp->lchild;
    }
    return temp;
}
 
void SBTNode::remove_node(SBTNode * delete_node) {
    SBTNode * temp = ZPTR;
    if (delete_node->lchild != ZPTR) {
        temp = delete_node->lchild;
        temp->father = delete_node->father;
        delete_node->lchild = ZPTR;
    }
 
    if (delete_node->rchild != ZPTR) {
        temp = delete_node->rchild;
        temp->father = delete_node->father;
        delete_node->rchild = ZPTR;
    }
    if (delete_node->father->lchild == delete_node) {
        delete_node->father->lchild = temp;
    } else {
        delete_node->father->rchild = temp;
    }
    temp = delete_node;
    while (temp != NULL) {
        temp->size--;
        temp = temp->father;
    }
    delete delete_node;
}
 
bool SBTNode::remove(ULONG value) {
    SBTNode * delete_node, * current_node;
    current_node = search(value);
    if (current_node == ZPTR) {
        return false;
    }
    size--;
    if (current_node->lchild != ZPTR) {
        delete_node = current_node->predecessor();
    } else if (current_node->rchild != ZPTR) {
        delete_node = current_node->successor();
    } else {
        delete_node = current_node;
    }
    current_node->data = delete_node->data;
    remove_node(delete_node);
    return true;
}
 
ULONG SBTNode::select(ULONG k) {
    //rank表示当前结点在子树的排位
	ULONG rank = lchild->size + 1;
	//若rank等于第k小的k，说明就是要找的值，直接返回权值即可
	if (rank == k) {
		return data;
	}else if (k < rank) {
	//小于rank，就表明要找比当前结点更小的，就在左边查找
		return lchild->select(k);
	}else{
	//大于就在右边咯
		//这里为什么看k - rank呢，因为我们已经把前rank排除了，
		//相当于我们要在右子树（把他当做一颗新的树去查找），所以排位当然要减去rank了
		return rchild->select(k - rank);
	}
}
 
BinaryTree::BinaryTree() {
    root = NULL;
}
 
BinaryTree::~BinaryTree() {
    if (root != NULL) {
        delete root;
    }
}
 
void BinaryTree::insert(ULONG value) {
    if (root == NULL) {
		//初始化时只有根结点，所以子树大小为1
        root = new SBTNode(value, 1);
    } else {
        root = ::insert(root, value);
    }
}

ULONG BinaryTree::size()  {
    return this->root->size;
}
 
 
bool BinaryTree::find(ULONG value) {
    if (root->search(value) == NULL) {
        return false;
    } else {
       return true;
    }
}
 
bool BinaryTree::remove(ULONG value) {
    return root->remove(value);
}
 
ULONG BinaryTree::select(ULONG k) {
    return root->select(k);
}
