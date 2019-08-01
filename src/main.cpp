#include <iostream>
#include <map>
#include <chrono>
#include <random>

using namespace std;
using chrono::high_resolution_clock;



void Case1()
{
    std::map<int, int> m;
	

    default_random_engine e;
    std::uniform_int_distribution<> dist{0, 1000000000};
    double N = 1000000;


    high_resolution_clock::time_point t1 = high_resolution_clock::now(); //返回时间戳

    for (int i = 0; i < N; i++)
    {
		auto v = dist(e);
		m[v] = v;
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now(); //返回时间戳

	std::cout << (t2 - t1).count() / N << std::endl;
    std::cout << "end Push Case Benchmark" << std::endl;
}

int main(int argc, char *argv[]) {
	Case1();
}