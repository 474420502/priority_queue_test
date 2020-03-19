
#include "sbt.h"
#include "vbtree.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <random>
#include "skiplist/skiplist.h"

using namespace std;
using chrono::high_resolution_clock;
using std::string;

int IntCompare(ULONG v1, ULONG v2) {
  if (v1 > v2) {
    return 1;
  } else if (v1 < v2) {
    return -1;
  } else {
    return 0;
  }
}

const ULONG N = 5000000;

vector<ULONG> vec;
map<string, void (*)()> funcmap ;

void init() {
  cout << "base on " << N << " data" << endl;
  const char *fpath = "./vec.dat";
  std::ifstream inf(fpath);
  if (!inf.is_open()) {
    cout << "vec.dat is not exists, create random data" << endl;
    std::ofstream openfile(fpath, ios::binary | ios::trunc);
    default_random_engine e;
    std::uniform_int_distribution<> dist{0, 1000000000};
    for (ULONG i = 0; i < N; i++) {
      auto v = dist(e);
      openfile  << v << " ";
      
    }
    openfile.close();
    inf.open(fpath);
  }
  
  cout << "vec.dat loading..." << endl;
  for(;!inf.eof();) {
    int v;
    inf >> v;
    vec.push_back(v);
  }
}

void Case1() {
  std::map<ULONG, ULONG> m;

  high_resolution_clock::time_point t1 =
      high_resolution_clock::now(); //返回时间戳

  for (ULONG i = 0; i < N; i++) {
    auto v = vec[i];
    m[v] = v;
  }
  high_resolution_clock::time_point t2 =
      high_resolution_clock::now(); //返回时间戳

  std::cout << (t2 - t1).count() / N << " ns/op" << std::endl;
  std::cout << "end RBTree Case <Put> Benchmark" << std::endl;



}

void Case1_1() {
  std::map<ULONG, ULONG> m;

  for (ULONG i = 0; i < N; i++) {
    auto v = vec[i];
    m[v] = v;
  }

  high_resolution_clock::time_point t1 =
      high_resolution_clock::now(); //返回时间戳

  for (auto iter = vec.begin(); iter != vec.end(); iter++) {
    m[*iter];
  }

  high_resolution_clock::time_point t2 =
      high_resolution_clock::now(); //返回时间戳

  std::cout << (t2 - t1).count() / N << " ns/op" << std::endl;
  std::cout << "end RBTree Case <Get> Benchmark" << std::endl;
}

void Case2() {
  VBTree<ULONG, ULONG> m(IntCompare);

  high_resolution_clock::time_point t1 =
      high_resolution_clock::now(); //返回时间戳
  for (ULONG i = 0; i < N; i++) {
    auto v = vec[i];
    m.put(v, v);
  }
  high_resolution_clock::time_point t2 =
      high_resolution_clock::now(); //返回时间戳

  std::cout << (t2 - t1).count() / N << " ns/op" << std::endl;
  std::cout << "end VBTree Case <Put> Benchmark" << std::endl;
}

void Case2_1() {
  VBTree<ULONG, ULONG> m(IntCompare);

  for (ULONG i = 0; i < N; i++) {
    auto v = vec[i];
    m.put(v, v);
  }

  high_resolution_clock::time_point t1 =
      high_resolution_clock::now(); //返回时间戳

  for (auto iter = vec.begin(); iter != vec.end(); iter++) {
    m.get(*iter);
  }

  high_resolution_clock::time_point t2 =
      high_resolution_clock::now(); //返回时间戳

  std::cout << (t2 - t1).count() / N << " ns/op" << std::endl;
  std::cout << "end VBTree Case <Get> Benchmark" << std::endl;
}

void Case3() {
  BinaryTree tree;

  high_resolution_clock::time_point t1 =
      high_resolution_clock::now(); //返回时间戳

  for (ULONG i = 0; i < N; i++) {
    auto v = vec[i];
    tree.insert(v);
  }
  high_resolution_clock::time_point t2 =
      high_resolution_clock::now(); //返回时间戳

  std::cout << (t2 - t1).count() / N << " ns/op" << std::endl;
  std::cout << "end SBT Case <Put> Benchmark" << std::endl;
}

void Case3_1() {
  BinaryTree tree;

  for (ULONG i = 0; i < N; i++) {
    auto v = vec[i];
    tree.insert(v);
  }

  high_resolution_clock::time_point t1 =
      high_resolution_clock::now(); //返回时间戳

  for (auto iter = vec.begin(); iter != vec.end(); iter++) {
    tree.find(*iter);
  }

  high_resolution_clock::time_point t2 =
      high_resolution_clock::now(); //返回时间戳

  std::cout << (t2 - t1).count() / N << " ns/op" << std::endl;
  std::cout << "end SBT Case <find(Get)> Benchmark" << std::endl;
}

void Case4() {
  struct skiplist *list = skiplist_new();

  high_resolution_clock::time_point t1 =
      high_resolution_clock::now(); //返回时间戳

  for (ULONG i = 0; i < N; i++) {
    auto v = vec[i];
    skiplist_insert(list, v, v);
  }
  high_resolution_clock::time_point t2 =
      high_resolution_clock::now(); //返回时间戳

  std::cout << (t2 - t1).count() / N << " ns/op" << std::endl;
  std::cout << "end SkipList Case <insert(Put)> Benchmark" << std::endl;
}

void Case4_1() {
  struct skiplist *list = skiplist_new();

  for (ULONG i = 0; i < N; i++) {
    auto v = vec[i];
    skiplist_insert(list, v, v);
  }


  high_resolution_clock::time_point t1 =
      high_resolution_clock::now(); //返回时间戳

  for (auto iter = vec.begin(); iter != vec.end(); iter++) {
    skiplist_search(list, *iter);
  }

  high_resolution_clock::time_point t2 =
      high_resolution_clock::now(); //返回时间戳


  std::cout << (t2 - t1).count() / N << " ns/op" << std::endl;
  std::cout << "end SkipList Case <search(Get)> Benchmark" << std::endl;
}



int main(int argc, char *argv[]) {
  init();

  funcmap["1"] = Case1;
  funcmap["1_1"] = Case1_1;

  funcmap["2"] = Case2;
  funcmap["2_1"] = Case2_1;

  funcmap["3"] = Case3;
  funcmap["3_1"] = Case3_1;

  funcmap["4"] = Case4;
  funcmap["4_1"] = Case4_1;
  
  cout << endl;
  cout << "case: " << argv[1] << endl;
  string fname = argv[1];
  funcmap[fname]();
}