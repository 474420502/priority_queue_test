#include "sbt.h"
#include "vbtree.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <random>

using namespace std;
using chrono::high_resolution_clock;
using std::string;

int IntCompare(int v1, int v2) {
  if (v1 > v2) {
    return 1;
  } else if (v1 < v2) {
    return -1;
  } else {
    return 0;
  }
}

const ULONG N = 5000000;

vector<unsigned int> vec;
map<string, void (*)()> funcmap ;

void init() {
  const char *fpath = "./vec.dat";
  std::ifstream inf(fpath);
  if (!inf.is_open()) {
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
  for(;!inf.eof();) {
    int v;
    inf >> v;
    vec.push_back(v);
  }
}

void Case1() {
  std::map<int, int> m;

  default_random_engine e;
  std::uniform_int_distribution<> dist{0, 1000000000};

  high_resolution_clock::time_point t1 =
      high_resolution_clock::now(); //返回时间戳

  for (ULONG i = 0; i < N; i++) {
    auto v = vec[i];
    m[v] = v;
  }
  high_resolution_clock::time_point t2 =
      high_resolution_clock::now(); //返回时间戳

  std::cout << (t2 - t1).count() / N << std::endl;
  std::cout << "end RBTree Case <Put> Benchmark" << std::endl;



}

void Case1_1() {
  std::map<int, int> m;

  default_random_engine e;
  std::uniform_int_distribution<> dist{0, 1000000000};

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

  std::cout << (t2 - t1).count() / N << std::endl;
  std::cout << "end RBTree Case <Get> Benchmark" << std::endl;
}

void Case2() {
  VBTree<int, int> m(IntCompare);

  high_resolution_clock::time_point t1 =
      high_resolution_clock::now(); //返回时间戳
  for (ULONG i = 0; i < N; i++) {
    auto v = vec[i];
    m.put(v, v);
  }
  high_resolution_clock::time_point t2 =
      high_resolution_clock::now(); //返回时间戳

  std::cout << (t2 - t1).count() / N << std::endl;
  std::cout << "end VBTree Case <Put> Benchmark" << std::endl;
}

void Case2_1() {
  VBTree<int, int> m(IntCompare);

  default_random_engine e;
  std::uniform_int_distribution<> dist{0, 1000000000};

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

  std::cout << (t2 - t1).count() / N << std::endl;
  std::cout << "end VBTree Case <Get> Benchmark" << std::endl;
}

void Case3() {
  BinaryTree tree;

  default_random_engine e;
  std::uniform_int_distribution<> dist{0, 1000000000};

  high_resolution_clock::time_point t1 =
      high_resolution_clock::now(); //返回时间戳

  for (ULONG i = 0; i < N; i++) {
    auto v = vec[i];
    tree.insert(v);
  }
  high_resolution_clock::time_point t2 =
      high_resolution_clock::now(); //返回时间戳

  std::cout << (t2 - t1).count() / N << std::endl;
  std::cout << "end SBT Case <Put> Benchmark" << std::endl;
}

int main(int argc, char *argv[]) {
  init();

  funcmap["1"] = Case1;
  funcmap["1_1"] = Case1_1;

  funcmap["2"] = Case2;
  funcmap["2_1"] = Case2_1;

  funcmap["3"] = Case3;

  cout << argv[1] << endl;
  string fname = argv[1];
  funcmap[fname]();
}