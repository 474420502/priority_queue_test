

#include <iostream>
#include <map>

using namespace std;
using std::cout, std::endl;

template <class TKey, class TValue>
class VBTree
{
public:
    typedef struct Node
    {
        Node *children[2];
        Node *parent;
        unsigned long size;
        TKey key;
        TValue value;
        /* data */
    } ;

public:
    void put(TKey key, TValue value) {
       Node *node = new Node();
	}
   
	// if tree.root == nil {
	// 	tree.root = node
	// 	return
	// }

	// for cur := tree.root; ; {

	// 	if cur.size > 8 {
	// 		factor := cur.size / 10 // or factor = 1
	// 		ls, rs := cur.children[0].size, cur.children[1].size
	// 		if rs >= ls*2+factor || ls >= rs*2+factor {
	// 			tree.fixSize(cur, ls, rs)
	// 		}
	// 	}

	// 	cur.size++

	// 	c := tree.Compare(key, cur.key)
	// 	if c < 0 {
	// 		if cur.children[0] == nil {
	// 			cur.children[0] = node
	// 			node.parent = cur

	// 			if cur.parent != nil && cur.parent.size == 3 {
	// 				if cur.parent.children[0] == nil {
	// 					tree.lrrotate3(cur.parent)
	// 				} else {
	// 					tree.rrotate3(cur.parent)
	// 				}
	// 			}
	// 			return
	// 		}
	// 		cur = cur.children[0]
	// 	} else {
	// 		if cur.children[1] == nil {
	// 			cur.children[1] = node
	// 			node.parent = cur

	// 			if cur.parent != nil && cur.parent.size == 3 {
	// 				if cur.parent.children[1] == nil {
	// 					tree.rlrotate3(cur.parent)
	// 				} else {
	// 					tree.lrotate3(cur.parent)
	// 				}
	// 			}
	// 			return
	// 		}
	// 		cur = cur.children[1]
	// 	}
	// }
    
    /* data */
};
