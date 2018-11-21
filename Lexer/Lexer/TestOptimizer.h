#pragma once
class TestOptimizer
{
private:
	TestOptimizer();
public:
	~TestOptimizer();

public:
	static void test_move();
	static void test_build_dfa();
	static void test_minimize_dfa();
	static void test_mini_mini_c();
};

