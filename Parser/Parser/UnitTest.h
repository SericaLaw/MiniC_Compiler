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
};

template<typename T>
inline void UnitTest::time_it(T (*pf)())
{
	time_t start = clock();
	pf();
	time_t end = clock();
	cout << "Time consumed: " << (double)(end - start) / CLOCKS_PER_SEC <<"s\n";
}
