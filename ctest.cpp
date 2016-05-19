#include "ctest.h"

void push(CStack<int>& stack, int begin) {
	for(int i = begin; i < ITERATIONS + begin; ++i)
		stack.push(i);
}

void pop(CStack<int>& stack) {
	for(int i = 0; i < ITERATIONS; ++i)
		stack.pop();
}

void push_pop(CStack<int>& stack) {
	for(int i = 0; i < ITERATIONS; ++i) {
		auto val = stack.pop();
		if(val) stack.push(*val);
	}
}

TEST_F(CTest, parallel_push) {
	CStack<int> stack;
	std::vector<short> test_vector(ITERATIONS * std::thread::hardware_concurrency());
	memset(&test_vector[0], 0, ITERATIONS * std::thread::hardware_concurrency());
	std::vector<std::thread> threads;
	for(size_t i = 0; i < std::thread::hardware_concurrency(); ++i)
		threads.emplace_back(push, std::ref(stack), ITERATIONS * i);
	for(size_t i = 0; i < threads.size(); ++i) threads[i].join();
	std::shared_ptr<int> val;
	while((val = stack.pop()) != NULL) {
		EXPECT_TRUE(*val >= 0);
		EXPECT_TRUE((size_t)(*val) < test_vector.size());
		EXPECT_EQ(test_vector[*val] , 0);
		test_vector[*val] = 1;
	} 
	for(size_t i = 0; i < test_vector.size(); ++i) EXPECT_EQ(test_vector[i], 1);
}

TEST_F(CTest, parallel_push_pop_not_empty) {
	CStack<int> stack;
	for(int i = 0; i < 10; ++i) stack.push(i);
	std::vector<std::thread> threads;
	for(size_t i = 0; i < std::thread::hardware_concurrency() / 2 + 1; ++i) 
		threads.emplace_back(push, std::ref(stack), ITERATIONS * i);
	for(size_t i = 0; i < std::thread::hardware_concurrency() / 2 - 1; ++i) 
		threads.emplace_back(pop, std::ref(stack));
	for(size_t i = 0; i < threads.size(); ++i) threads[i].join();
}

TEST_F(CTest, parallel_push_pop_empty) {
	CStack<int> stack;
	for(int i = 0; i < 10; ++i) stack.push(i);
	std::vector<std::thread> threads;
	for(size_t i = 0; i < std::thread::hardware_concurrency() / 2 - 1; ++i) 
		threads.emplace_back(push, std::ref(stack), ITERATIONS * i);
	for(size_t i = 0; i < std::thread::hardware_concurrency() / 2 + 1; ++i) 
		threads.emplace_back(pop, std::ref(stack));
	for(size_t i = 0; i < threads.size(); ++i) threads[i].join();
}

TEST_F(CTest, push_pop) {
	CStack<int> stack;
	for(int i = 0; i < ITERATIONS; ++i) stack.push(i);
	std::vector<short> test_vector(ITERATIONS);
	memset(&test_vector[0], 0, ITERATIONS);
	std::vector<std::thread> threads;
	for(size_t i = 0; i < std::thread::hardware_concurrency(); ++i)
		threads.emplace_back(push_pop, std::ref(stack));
	for(size_t i = 0; i < threads.size(); ++i) threads[i].join();
	std::shared_ptr<int> val;
	while((val = stack.pop()) != NULL) {
		EXPECT_TRUE(*val >= 0);
		EXPECT_TRUE((size_t)(*val) < test_vector.size());
		EXPECT_EQ(test_vector[*val] , 0);
		test_vector[*val] = 1;
	} 
	for(size_t i = 0; i < test_vector.size(); ++i) EXPECT_EQ(test_vector[i], 1);
}

