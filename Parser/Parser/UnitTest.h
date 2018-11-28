#pragma once
class UnitTest
{
public:
	UnitTest();
	~UnitTest();
	
public:
	template <typename T>
	static void time_it(T (*pf)());
public:
	static SyntaxParser test_add_production();
	static SyntaxParser test_first();
	static SyntaxParser test_follow();
	static SyntaxParser test_build_LL_1_parsing_table();
	static SyntaxParser test_parse_by_LL_1();
	static SyntaxParser test_parse_mini_c();
};

template<typename T>
inline void UnitTest::time_it(T (*pf)())
{
	time_t start = clock();
	pf();
	time_t end = clock();
	cout << "Time consumed: " << (double)(end - start) / CLOCKS_PER_SEC <<"s\n";
}