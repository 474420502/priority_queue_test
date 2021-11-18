
#include "sbt.h"
#include "indextree.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <random>
#include "skiplist/skiplist.h"

using namespace std;
using chrono::high_resolution_clock;
using std::string;


vector<ULONG> vec500 , vec5000, vec50000, vec500000, vec5000000;
map<string, void (*)()> funcmap;

void createData(vector<ULONG> &vec, ULONG num)
{
  cout << "base on " << num << " data" << endl;
  string fpath;
  stringstream sfpath;
  sfpath << "./vec" << num << ".dat";
  sfpath >> fpath;
  // const char *fpath = "./vec" + num + ".dat";
  std::ifstream inf(fpath.c_str());
  if (!inf.is_open())
  {
    cout << "vec.dat is not exists, create random data" << endl;
    std::ofstream openfile(fpath, ios::binary | ios::trunc);
    default_random_engine e;
    std::uniform_int_distribution<> dist{0, 1000000000};
    for (ULONG i = 0; i < num; i++)
    {
      auto v = dist(e);
      if(i != num - 1) {
        openfile << v << " ";
      } else {
        openfile << v;
      }
      
    }
    openfile.close();
    inf.open(fpath);
  }

  cout << "vec.dat loading..." << endl;
  for (; !inf.eof();)
  {
    int v;
    inf >> v;
    vec.push_back(v);
  }
}

void init()
{
  createData(vec500, 500);
  createData(vec5000, 5000);
  createData(vec50000, 50000);
  createData(vec500000, 500000);
  createData(vec5000000, 5000000);
}

void Case1()
{

  vector<ULONG> vecs[5] = {vec500, vec5000, vec50000, vec500000, vec5000000};

  for (int i = 0; i < 5; i++)
  {
    vector<ULONG> vec = vecs[i];

    std::map<ULONG, ULONG> m;

    high_resolution_clock::time_point t1 =
        high_resolution_clock::now(); //返回时间戳

    for (ULONG i = 0; i < vec.size(); i++)
    {
      auto v = vec[i];
      m[v] = v;
    }
    high_resolution_clock::time_point t2 =
        high_resolution_clock::now(); //返回时间戳

    std::cout << "size: " << vec.size() << ", " << (t2 - t1).count() / vec.size() << " ns/op" << std::endl;
    std::cout << "end RBTree Case <Put> Benchmark" << std::endl;
  }
}

void Case1_1()
{
  vector<ULONG> vecs[5] = {vec500, vec5000, vec50000, vec500000, vec5000000};

  for (int i = 0; i < 5; i++)
  {
    vector<ULONG> vec = vecs[i];

    std::map<ULONG, ULONG> m;

    for (ULONG i = 0; i < vec.size(); i++)
    {
      auto v = vec[i];
      m[v] = v;
    }

    high_resolution_clock::time_point t1 =
        high_resolution_clock::now(); //返回时间戳

    for (auto iter = vec.begin(); iter != vec.end(); iter++)
    {
      m[*iter];
    }

    high_resolution_clock::time_point t2 =
        high_resolution_clock::now(); //返回时间戳

    std::cout << "size: " << vec.size() << ", " << (t2 - t1).count() / vec.size() << " ns/op" << std::endl;
    std::cout << "end RBTree Case <Get> Benchmark" << std::endl;
  }
}

void Case2()
{
  vector<ULONG> vecs[5] = {vec500, vec5000, vec50000, vec500000, vec5000000};

  for (int i = 0; i < 5; i++)
  {
    vector<ULONG> vec = vecs[i];

    IndexTree<ULONG, ULONG> m;

    high_resolution_clock::time_point t1 =
        high_resolution_clock::now(); //返回时间戳
    for (ULONG i = 0; i < vec.size(); i++)
    {
      auto v = vec[i];
      m.Set(v, v);
    }
    high_resolution_clock::time_point t2 =
        high_resolution_clock::now(); //返回时间戳

    std::cout << "size: " << vec.size() << ", " << (t2 - t1).count() / vec.size() << " ns/op" << std::endl;
    std::cout << "end IndexTree Case <Put> Benchmark" << std::endl;
  }
}

void Case2_1()
{

  vector<ULONG> vecs[5] = {vec500, vec5000, vec50000, vec500000, vec5000000};

  for (int i = 0; i < 5; i++)
  {
    vector<ULONG> vec = vecs[i];

    IndexTree<ULONG, ULONG> m;

    for (ULONG i = 0; i < vec.size(); i++)
    {
      auto v = vec[i];
      m.Set(v, v);
    }

    high_resolution_clock::time_point t1 =
        high_resolution_clock::now(); //返回时间戳

    for (auto iter = vec.begin(); iter != vec.end(); iter++)
    {
      m.Get(*iter);
    }

    high_resolution_clock::time_point t2 =
        high_resolution_clock::now(); //返回时间戳

    std::cout << "size: " << vec.size() << ", " << (t2 - t1).count() / vec.size() << " ns/op" << std::endl;
    std::cout << "end IndexTree Case <Get> Benchmark" << std::endl;
  }
}

void Case3()
{

  vector<ULONG> vecs[5] = {vec500, vec5000, vec50000, vec500000, vec5000000};

  for (int i = 0; i < 5; i++)
  {
    vector<ULONG> vec = vecs[i];

    BinaryTree tree;

    high_resolution_clock::time_point t1 =
        high_resolution_clock::now(); //返回时间戳

    for (ULONG i = 0; i < vec.size(); i++)
    {
      auto v = vec[i];
      tree.insert(v);
    }
    high_resolution_clock::time_point t2 =
        high_resolution_clock::now(); //返回时间戳

    std::cout << "size: " << vec.size() << ", " << (t2 - t1).count() / vec.size() << " ns/op" << std::endl;
    std::cout << "end SBT Case <Put> Benchmark" << std::endl;
  }
}

void Case3_1()
{

  vector<ULONG> vecs[5] = {vec500, vec5000, vec50000, vec500000, vec5000000};

  for (int i = 0; i < 5; i++)
  {
    vector<ULONG> vec = vecs[i];

    BinaryTree tree;

    for (ULONG i = 0; i < vec.size(); i++)
    {
      auto v = vec[i];
      tree.insert(v);
    }

    high_resolution_clock::time_point t1 =
        high_resolution_clock::now(); //返回时间戳

    for (auto iter = vec.begin(); iter != vec.end(); iter++)
    {
      tree.find(*iter);
    }

    high_resolution_clock::time_point t2 =
        high_resolution_clock::now(); //返回时间戳

    std::cout << "size: " << vec.size() << ", " << (t2 - t1).count() / vec.size() << " ns/op" << std::endl;
    std::cout << "end SBT Case <find(Get)> Benchmark" << std::endl;
  }
}

void Case4()
{

  vector<ULONG> vecs[5] = {vec500, vec5000, vec50000, vec500000, vec5000000};

  for (int i = 0; i < 5; i++)
  {
    vector<ULONG> vec = vecs[i];

    struct skiplist *list = skiplist_new();

    high_resolution_clock::time_point t1 =
        high_resolution_clock::now(); //返回时间戳

    for (ULONG i = 0; i < vec.size(); i++)
    {
      auto v = vec[i];
      skiplist_insert(list, v, v);
    }
    high_resolution_clock::time_point t2 =
        high_resolution_clock::now(); //返回时间戳

    std::cout << "size: " << vec.size() << ", " << (t2 - t1).count() / vec.size() << " ns/op" << std::endl;
    std::cout << "end SkipList Case <insert(Put)> Benchmark" << std::endl;
  }
}

void Case4_1()
{
  vector<ULONG> vecs[5] = {vec500, vec5000, vec50000, vec500000, vec5000000};

  for (int i = 0; i < 5; i++)
  {
    vector<ULONG> vec = vecs[i];
    struct skiplist *list = skiplist_new();

    for (ULONG i = 0; i < vec.size(); i++)
    {
      auto v = vec[i];
      skiplist_insert(list, v, v);
    }

    high_resolution_clock::time_point t1 =
        high_resolution_clock::now(); //返回时间戳

    for (auto iter = vec.begin(); iter != vec.end(); iter++)
    {
      skiplist_search(list, *iter);
    }

    high_resolution_clock::time_point t2 =
        high_resolution_clock::now(); //返回时间戳

    std::cout << "size: " << vec.size() << ", " << (t2 - t1).count() / vec.size() << " ns/op" << std::endl;
    std::cout << "end SkipList Case <search(Get)> Benchmark" << std::endl;
  }
}

int main(int argc, char *argv[])
{
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

  cout << endl;
}