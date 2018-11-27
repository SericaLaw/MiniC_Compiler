#include "stdafx.h"
#include "SyntaxParser.h"


SyntaxParser::SyntaxParser()
{
}


SyntaxParser::~SyntaxParser()
{
}

// p����yacc�淶������ expr : expr '+' expr
// ��ʱ��֧�ֿղ���ʽ
void SyntaxParser::add_production(const string& p)
{
	vector<string> p_split = Tool::split(p, " ");
	assert(p_split.size() >= 3); // ���������head, :, body
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

// !important ���ﲻ�������epsilon
int SyntaxParser::insert_vn(const string & vn)
{
	int index;
	if (vn_map.count(vn) == 0) {
		// û���ڷ��ű��г���
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
		// û���ڷ��ű��г���
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
			// �Ƿ��ս��
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
	// �ݹ���FIRST(X)����������µ���һ�ξͿ���������
	// ע����֮ǰҪ������ݹ飬����stack overflow, u know :)

	// ����Ѿ�����ˣ�ֱ�ӷ��أ����һ�ξ���ȫ��⣬����Ҫ����
	auto found = first_map.find(X);
	if (found != first_map.end())
		return found->second;

	// ����p140�㷨
	set<int> first_X;
	// X���ս������FIRST(X)={X}
	if (is_vt(X)) {
		first_X.insert(X);
	}
	else {
		// ������XΪ����ʽͷ���Ĳ���ʽ���в���
		for (auto prod = productions.begin(), last_prod = productions.end();
				prod != last_prod; ++prod) {
			if (prod->head == X) {
				// �Բ���ʽ�������з��Ž��в���
				if (prod->body[0] == X) // ����ǿղ���ʽ����ݹ��򲻿��Ǹò���ʽ
					continue;
				
				int last_s = prod->body.back();

				for (int s : prod->body) {
					if (is_vt(s)) {
						// X -> s... �����ٿ��Ǻ���
						first_X.insert(s);
						break;
					}
					else if (s == EPSILON) {
						first_X.insert(EPSILON);
						assert(s == prod->body[0]); // ����ǿղ���ʽֻ������X->epsilon����ʽ
					}
					else {
						// X -> A...
						set<int> first_A = first(s);
						if (first_A.count(EPSILON) == 0) {
							// A�Ƿ��ս���Ҳ�������մ��������FIRST(A)��break
							first_X.insert(first_A.begin(), first_A.end());
							break;
						}
						else {
							// A�Ƿ��ս���һ�����մ������FIRST(A)-{epsilon}����FIRST(X)
							first_A.erase(find(first_A.begin(), first_A.end(), EPSILON));
							first_X.insert(first_A.begin(), first_A.end());
							if (s == last_s) {
								// X->A1...An�� A1..An��������մ������epsilon�����ǰ���в�����Ѿ�break�����ˣ�
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
