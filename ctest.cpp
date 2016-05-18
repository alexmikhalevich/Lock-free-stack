#include "ctest.h"

void push(CStack<int>& stack, int begin) {
	for(int i = begin; i < ITERATIONS + begin; ++i)
		stack.push(i);
}

TEST_F(CTest, parallel_push) {
	CStack<int> stack;
	std::vector<std::thread> threads;
	for(size_t i = 0; i < std::thread::hardware_concurrency(); ++i)
		threads.emplace_back(push, std::ref(stack), ITERATIONS * i);
	for(size_t i = 0; i < threads.size(); ++i)
		threads[i].join();
}
