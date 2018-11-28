#pragma once
class SyntaxParser
{
private:
	const int EPSILON = -1;
	const int DOLLAR = -2;
private:
	// 注意epsilon不属于终结符或非终结符！
	map<string, int> vn_map;
	map<string, int> vt_map;

	vector<string> symbols;
	/* 规定按顺序加入productions 第一条的head是开始符号*/
	vector<Production> productions;
	map<int, set<int>> first_map;
	map<int, set<int>> follow_map;

	// LL_1_table[A, a] = a production or error
	map<pair<int, int>, unsigned int> M;
public:
	SyntaxParser();
	~SyntaxParser();
public:
	void add_production(const string& p);
	/* 对所有非终结符求FIRST */
	void calc_first();
	/* 对所有非终结符求FOLLOW 必须要先求好FIRST */
	void calc_follow();
	/* 龙书p143 */
	void build_LL_1_parsing_table();
	/* 龙书p144 */
	// TODO：注意下面的w实际上是token序列，这只是暂时用string代替 需要消除左递归
	void parse_by_LL_1(vector<string> w);
public:
	/* debug */
	void print_production(int index);
	void print_productions();
	void print_first_map();
	void print_follow_map();
	void print_LL_1_parsing_table();

	void add_mini_c_production();
private:
	/* 插入非终结符并返回其在symbols中的索引 */
	int insert_vn(const string& vn);
	/* 插入终结符并返回其在symbols中的索引 */
	int insert_vt(const string& vt);
	void add_production(const string& head, vector<string>& body);

	set<int> first(int X);
	/* 求串beta的FIRST 龙书p140 */
	set<int> first(vector<int>& beta);

	set<int> follow(int X);
	
	/* 判断是否是终结符 */
	bool is_vt(int x);
	bool is_vt(string s);
	
};

