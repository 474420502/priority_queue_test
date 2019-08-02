#include <iostream>
#include <map>
#include <chrono>
#include <random>
#include "vbtree.h"
#include "sbt.h"


using namespace std;
using chrono::high_resolution_clock;

int IntCompare(int v1, int v2)
{
  if (v1 > v2)
  {
    return 1;
  }
  else if (v1 < v2)
  {
    return -1;
  }
  else
  {
    return 0;
  }
}

const ULONG N = 5000000;

void Case1()
{
  std::map<int, int> m;

  default_random_engine e;
  std::uniform_int_distribution<> dist{0, 1000000000};

  high_resolution_clock::time_point t1 = high_resolution_clock::now(); //返回时间戳

  for (ULONG i = 0; i < N; i++)
  {
    auto v = dist(e);
    m[v] = v;
  }
  high_resolution_clock::time_point t2 = high_resolution_clock::now(); //返回时间戳

  std::cout << (t2 - t1).count() / N << std::endl;
  std::cout << "end RBTree Case Benchmark" << std::endl;
}

void Case1_1()
{
  std::map<int, int> m;

  default_random_engine e;
  std::uniform_int_distribution<> dist{0, 1000000000};

  vector<int> vec;

  for (ULONG i = 0; i < N; i++)
  {
    auto v = dist(e);
    m[v] = v;
    vec.push_back(v);
  }

  high_resolution_clock::time_point t1 = high_resolution_clock::now(); //返回时间戳

  for(auto iter = vec.begin(); iter != vec.end() ; iter++) {
      m[*iter];
  }

   high_resolution_clock::time_point t2 = high_resolution_clock::now(); //返回时间戳

  std::cout << (t2 - t1).count() / N << std::endl;
  std::cout << "end RBTree Case Benchmark" << std::endl;
}

void Case2()
{
  VBTree<int, int> m(IntCompare);

  default_random_engine e;
  std::uniform_int_distribution<> dist{0, 1000000000};

  high_resolution_clock::time_point t1 = high_resolution_clock::now(); //返回时间戳

  for (ULONG i = 0; i < N; i++)
  {
    auto v = dist(e);
    m.put(v, v);
  }
  high_resolution_clock::time_point t2 = high_resolution_clock::now(); //返回时间戳

  std::cout << (t2 - t1).count() / N << std::endl;
  std::cout << "end VBTree Case Benchmark" << std::endl;
}

void Case2_1()
{
  VBTree<int, int> m(IntCompare);

  default_random_engine e;
  std::uniform_int_distribution<> dist{0, 1000000000};

  vector<int> vec;

  for (ULONG i = 0; i < N; i++)
  {
    auto v = dist(e);
    m.put(v, v);
    vec.push_back(v);
  }

  high_resolution_clock::time_point t1 = high_resolution_clock::now(); //返回时间戳

  for(auto iter = vec.begin(); iter != vec.end() ; iter++) {
      m.get(*iter);
  }

  high_resolution_clock::time_point t2 = high_resolution_clock::now(); //返回时间戳

  std::cout << (t2 - t1).count() / N << std::endl;
  std::cout << "end VBTree Case Benchmark" << std::endl;
}

void Case3() {
  BinaryTree tree;

  default_random_engine e;
  std::uniform_int_distribution<> dist{0, 1000000000};

  high_resolution_clock::time_point t1 = high_resolution_clock::now(); //返回时间戳

  for (ULONG i = 0; i < N; i++)
  {
    auto v = dist(e);
    tree.insert(v);
  }
  high_resolution_clock::time_point t2 = high_resolution_clock::now(); //返回时间戳

  std::cout << (t2 - t1).count() / N << std::endl;
  std::cout << "end SBT Case Benchmark" << std::endl;    
}

int main(int argc, char *argv[])
{
  Case2_1();
  // Case1();
}