all:
	g++ -std=c++11 -latomic -lgtest -lgtest_main main.cpp ctest.cpp
