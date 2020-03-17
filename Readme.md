# 优先队列的一些理解与测试

## 优先队列(PriorityQueue)

最经典的就是Heap的应用, 效率也相当高, 当时操作仅限Put Pop. 无法实现Rank, Index等操作, 很多时候结构使用局限性较大. 甚至会出现一些每次需要Index, Rank的时候, 先Copy一个数组, Sort(排序一次), 通过数组Index. 数据量非常小的情景下, 实则怎么操作都没什么大问题. 量巨大的时候, 效率异常低下. 所以以下是我曾经为了优先队列的一些研究, 我写文章很随意, 如果有错的我后面更正, 我写完自己都不会看.
  
---

* SkipList
  
> 跳表是一个随机化的数据结构，可以被看做二叉树的一个变种，它在性能上和红黑树，AVL树不相上下，但是跳表的原理非常简单，目前在Redis和LeveIDB, Rocksdb中都有用到。
它采用随机技术决定链表中哪些节点应增加向前指针以及在该节点中应增加多少个指针。跳表结构的头节点需有足够的指针域，以满足可能构造最大级数的需要，而尾节点不需要指针域。
采用这种随机技术，跳表中的搜索、插入、删除操作的时间均为O(logn)，然而，最坏情况下时间复杂性却变成O(n)。相比之下，在一个有序数组或链表中进行插入/删除操作的时间为O(n)，最坏情况下为O(n)。

* 这里更正一下, 我个人认为跳表的Put效率远不如红黑树,AVL, 而且有大量测试都验证不如. 如果有异议可以纠正我错误.
* 跳表有index属性, 优先排序等. 所以我认为vbt 可以处理成更好的一个处理结构.
  
---

* Size Balance Tree
  
> 简称[SBT](https://wenku.baidu.com/view/364afa42a8956bec0975e3b1.html) 可以从链接山查看具体介绍, 拥有与跳表一样的Rank, Select(Index), 等属性, 在并行上处理不如跳表, 平衡树put,remove的操作都涉及范围内的旋转与调整, 需要锁树, 但是链表, 可以只锁相邻节点. 达到高效的并行处理.

* 通过各种语言和测试, 并不像论文所述, 并不优于AVL 和 RBTree. 感觉有夸大的嫌疑. 如果有异议可以纠正我错误.

---

* Vague Balance Tree
  
> 简称[VBT] 这个树是我根据宽度平衡的一些构想并实现, 我在实现这个树结构的时候,完全不知道SBT的存在(知道我不会写了). 通过大量的结果测试, 写出由一个平衡因子作旋转的平衡树, 具体以后再写. 实测性能不输RBTree(红黑), AVLTree, SBT SkipList并且有 SBT等同的属性. 在一些特殊倾斜的数据集会略输. 后面我想用vbt代替skiplist实现一个可以并行处理的顺序写简单例子(工作不忙的前提下).

---

## 测试结果

``` bash
git submodule init #初始化模块
git submodule update #更新子模块
sh run.sh #运行测试
```

``` bash
rm -f bin/main
g++ -std=c++17 -Wall -Wextra -g -O2 -Wno-unused-parameter -Wno-unused-function -Wno-sign-compare -Wno-maybe-uninitialized -Iinclude -Llib src/sbt.h src/vbtree.h src/main.cpp -o bin/main 
base on 5000000 data
vec.dat loading...

case: 1
775 ns/op
end RBTree Case <Put> Benchmark
base on 5000000 data
vec.dat loading...

case: 1_1
728 ns/op
end RBTree Case <Get> Benchmark
base on 5000000 data
vec.dat loading...

case: 2
701 ns/op
end VBTree Case <Put> Benchmark
base on 5000000 data
vec.dat loading...

case: 2_1
670 ns/op
end VBTree Case <Get> Benchmark
base on 5000000 data
vec.dat loading...

case: 3
1001 ns/op
end SBT Case <Put> Benchmark
base on 5000000 data
vec.dat loading...

case: 3_1
652 ns/op
end SBT Case <find(Get)> Benchmark
base on 5000000 data
vec.dat loading...

case: 4
1344 ns/op
end SkipList Case <insert(Put)> Benchmark
base on 5000000 data
vec.dat loading...

case: 4_1
1440 ns/op
end SkipList Case <search(Get)> Benchmark
```

* skiplist 效果并不如所述的理想. 只是在并行处理上有巨大的优势.

* 实际优先队列的选择上我更推荐平衡树带Size属性的实现. 这里有一个go的实现 <https://github.com/474420502/focus.>
  
* SBT浮动有点大, 不明白为什么.可能有更好实现
