#include "stdafx.h"
#include "SyntaxParser.h"


SyntaxParser::SyntaxParser()
{
}


SyntaxParser::~SyntaxParser()
{
}

// p满足yacc规范，例如 expr : expr '+' expr
// 暂时不支持空产生式
void SyntaxParser::add_production(const string& p)
{
	vector<string> p_split = Tool::split(p, " ");
	assert(p_split.size() >= 3); // 至少有三项：head, :, body
	assert(p_split[1] == ":");
	string head = p_split[0];
	vector<string> body(p_split.begin() + 2, p_split.end());
	add_production(head, body);
}

void SyntaxParser::print_productions()
{
	for (Production p : productions) {
		cout << p.index << ": " << symbols[p.head] << " =>";
		assert(vn_map[symbols[p.head]] == p.head);
		for (int s : p.body) {
			if (s == EPSILON) {
				cout << " 'epsilon'";
				continue;
			}
			string symbol = symbols[s];
			if (vn_map.count(symbol) == 1) {
				cout << " " << symbol;
				assert(vn_map[symbol] == s);
			}
			else if (vt_map.count(symbol) == 1) {
				cout << " '" << symbol << "'";
				assert(vt_map[symbol] == s);
			}
		}
		cout << endl;
	}
}

void SyntaxParser::print_first_map()
{
	for (auto vn : vn_map) {
		cout << "First("<<vn.first<<") = { ";
		auto first_set = first_map[vn.second];
		for (auto ele : first_set) {
			if(ele == EPSILON)
				cout << "epsilon" << " ";
			else
				cout << symbols[ele] << " ";
		}
		cout << "}\n";
	}
}

void SyntaxParser::print_follow_map()
{
	for (auto vn : vn_map) {
		cout << "Follow(" << vn.first << ") = { ";
		auto follow_set = follow_map[vn.second];
		for (auto ele : follow_set) {
			if (ele == EPSILON)
				cout << "epsilon" << " ";
			else if (ele == DOLLAR)
				cout << "$ " << " ";
			else
				cout << symbols[ele] << " ";
		}
		cout << "}\n";
	}
}

// !important 这里不允许插入epsilon
int SyntaxParser::insert_vn(const string & vn)
{
	int index;
	if (vn_map.count(vn) == 0) {
		// 没有在符号表中出现
		index = symbols.size();
		vn_map[vn] = symbols.size();
		symbols.push_back(vn);
	}
	else {
		index = vn_map[vn];
	}
	return index;
}

int SyntaxParser::insert_vt(const string & vt)
{
	// 空产生式
	if (vt == "epsilon")
		return EPSILON;
	int index;
	if (vt_map.count(vt) == 0) {
		// 没有在符号表中出现
		index = symbols.size();
		vt_map[vt] = symbols.size();
		symbols.push_back(vt);
	}
	else {
		index = vt_map[vt];
	}
	return index;
}

void SyntaxParser::add_production(const string& head, vector<string>& body)
{
	Production p;
	// 头部赋值为非终结符head的索引
	p.head = insert_vn(head);
	for (string s : body) {
		auto s_split = Tool::split(s, "'");
		assert(s_split.size() == 1);
		if (s_split[0].size() == s.size()) {
			// 是非终结符
			p.body.push_back(insert_vn(s_split[0]));
		}
		else {
			// 非终结符
			p.body.push_back(insert_vt(s_split[0]));
		}
	}
	p.index = productions.size();
	productions.push_back(p);
}

void SyntaxParser::calc_first()
{
	for (auto vn : vn_map)
		first(vn.second);
}

void SyntaxParser::calc_follow()
{
	for (auto vn : vn_map)
		follow(vn.second);
}

set<int> SyntaxParser::first(int X)
{
	// 递归求FIRST(X)，这种情况下调用一次就可以求完整
	// 注意求之前要消除左递归，否则stack overflow, u know :)

	// 如果已经求过了，直接返回，求过一次就完全求解，不需要更新
	auto found = first_map.find(X);
	if (found != first_map.end())
		return found->second;

	// 龙书p140算法
	set<int> first_X;
	// X是终结符，则FIRST(X)={X}
	if (is_vt(X)) {
		first_X.insert(X);
	}
	else {
		// 对所有X为产生式头部的产生式进行操作
		for (auto prod = productions.begin(), prod_end = productions.end();
				prod != prod_end; ++prod) {
			if (prod->head == X) {
				// 对产生式体中所有符号进行操作
				if (prod->body[0] == X) // 如果是空产生式或左递归则不考虑该产生式
					continue;
				
				int s_end = prod->body.back();

				for (int s : prod->body) {
					if (is_vt(s)) {
						// X -> s... 无须再考虑后续
						first_X.insert(s);
						break;
					}
					else if (s == EPSILON) {
						first_X.insert(EPSILON);
						assert(s == prod->body[0]); // 如果是空产生式只可以是X->epsilon的形式
					}
					else {
						// X -> A...
						set<int> first_A = first(s);
						if (first_A.count(EPSILON) == 0) {
							// A是非终结符且不会产生空串，则加入FIRST(A)就break
							first_X.insert(first_A.begin(), first_A.end());
							break;
						}
						else {
							// A是非终结符且会产生空串，则把FIRST(A)-{epsilon}加入FIRST(X)
							first_A.erase(find(first_A.begin(), first_A.end(), EPSILON));
							first_X.insert(first_A.begin(), first_A.end());
							if (s == s_end) {
								// X->A1...An中 A1..An都会产生空串则加入epsilon（如果前面有不会的已经break跳出了）
								assert(first_X.count(EPSILON) == 0);
								first_X.insert(EPSILON);
							}
						}
					}
				}
			}
		}		
	}
	first_map[X] = first_X;
	return first_X;
}

// 龙书p140
set<int> SyntaxParser::first(vector<int>& beta)
{
	set<int> first_beta;
	for (auto s = beta.begin(), s_end = beta.end(); s != s_end; ++s) {
		set<int> first_s = first(*s);
		if (first_s.count(EPSILON) == 0) {
			first_beta.insert(first_s.begin(), first_s.end());
			break;
		}
		else {
			// s产生空串 FIRST_BETA = FIRST_BETA ∪ FIRST_S - { epsilon }
			first_s.erase(find(first_s.begin(), first_s.end(), EPSILON));
			first_beta.insert(first_s.begin(), first_s.end());
			if (s + 1 == s_end) {
				assert(first_beta.count(EPSILON) == 0);
				first_beta.insert(EPSILON);
			}
		}
	}
	return first_beta;
}

// 龙书p141
set<int> SyntaxParser::follow(int X)
{
	auto found = follow_map.find(X);
	if (found != follow_map.end())
		return found->second;
	else
		// 这句初始化十分关键！形如E-> TE的式子，要求FOLLOW(E)时若没有这句初始化 则会陷入不断求FOLLOW(E)的死循环
		follow_map[X] = set<int>();
	// $加入FOLLOW(S)，其中$是开始符号，$是右端的结束标记

	
	
	
	set<int> follow_X;
	if (X == productions[0].head) {
		follow_X.insert(DOLLAR);
	}
	// 性能起见，应该少用冒号迭代，尤其是对于复杂对象 多使用迭代器或下标，且用++it代替it++
	// 遍历所有形如 A->aX，A->aXb的式子
	for (auto prod = productions.begin(), prod_end = productions.end(); prod != prod_end; ++prod) {
		for (auto s = prod->body.begin(), s_end = prod->body.end(); s != s_end; ++s) {
			if (X == *s) {
				auto next = s;
				++next;
				if (next == s_end) {
					// A->aX
					set<int> follow_A = follow(prod->head);
					follow_X.insert(follow_A.begin(), follow_A.end());
				}
				else {
					// A->aXb
					vector<int> beta(next, s_end);
					set<int> first_beta = first(beta);
					if (first_beta.count(EPSILON) == 0) {
						follow_X.insert(first_beta.begin(), first_beta.end());
					}
					else {
						// b =*=> epsilon
						first_beta.erase(find(first_beta.begin(), first_beta.end(), EPSILON));
						follow_X.insert(first_beta.begin(), first_beta.end());
						set<int> follow_A = follow(prod->head);
						follow_X.insert(follow_A.begin(), follow_A.end());
					}
					break;
				}
			}
			
		}
	}
	follow_map[X] = follow_X;
	return follow_X;
}

bool SyntaxParser::is_vt(int x)
{
	// TODO: 这里对于DOLLAR还须进一步讨论
	if (x == EPSILON || x == DOLLAR)
		return false;
	return vt_map.count(symbols[x]) == 1;
}
