#pragma once
class SyntaxParser
{
private:
	const int EPSILON = -1;
private:
	// 注意epsilon不属于终结符或非终结符！
	map<string, int> vn_map;
	map<string, int> vt_map;

	vector<string> symbols;
	vector<Production> productions;
	map<int, set<int>> first_map;
public:
	SyntaxParser();
	~SyntaxParser();
public:
	void add_production(const string& p);
	/* 对所有非终结符求FIRST */
	void calc_first();
public:
	/* debug */
	void print_productions();
	void print_first_map();
private:
	/* 插入非终结符并返回其在symbols中的索引 */
	int insert_vn(const string& vn);
	/* 插入终结符并返回其在symbols中的索引 */
	int insert_vt(const string& vt);
	void add_production(const string& head, vector<string>& body);

	set<int> first(int X);
	set<int> follow(int X);

	/* 判断是否是终结符 */
	bool is_vt(int x);
	
};

