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
		for (auto ele : first_set)
			cout << symbols[ele] << " ";
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
	p.head = insert_vn(head);
	for (string s : body) {
		auto s_split = Tool::split(s, "'");
		assert(s_split.size() == 1);
		if (s_split[0].size() == s.size()) {
			// 是非终结符
			p.body.push_back(insert_vn(s_split[0]));
		}
		else {
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
		for (auto prod = productions.begin(), last_prod = productions.end();
				prod != last_prod; ++prod) {
			if (prod->head == X) {
				// 对产生式体中所有符号进行操作
				if (prod->body[0] == X) // 如果是空产生式或左递归则不考虑该产生式
					continue;
				
				int last_s = prod->body.back();

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
							if (s == last_s) {
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

set<int> SyntaxParser::follow(int symbol)
{
	return set<int>();
}

bool SyntaxParser::is_vt(int x)
{
	return vt_map.count(symbols[x]) == 1;
}
