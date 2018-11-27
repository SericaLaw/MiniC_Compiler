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

// �ú���˳����֤��ӳ���Ӧ��ϵ����ȷ��
void SyntaxParser::print_production(int index)
{
	Production p = productions[index];
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
}

void SyntaxParser::print_productions()
{
	for (int i = 0, n = productions.size(); i < n; ++i) {
		print_production(i);
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
				cout << "$" << " ";
			else
				cout << symbols[ele] << " ";
		}
		cout << "}\n";
	}
}

void SyntaxParser::print_LL_1_parsing_table()
{
	for (auto vn : vn_map) {
		cout << vn.first<<":\t";
		for (auto vt : vt_map) {
			if (M.count(pair<int, int>(vn.second, vt.second)) == 1) {
				int prod_index = M[pair<int, int>(vn.second, vt.second)];
				cout << "(" << vt.first <<": ";
				print_production(prod_index);
				cout << ")\t";
			}
		}
		// do not forget $
		if (M.count(pair<int, int>(vn.second, DOLLAR)) == 1) {
			int prod_index = M[pair<int, int>(vn.second, DOLLAR)];
			cout << "($: ";
			print_production(prod_index);
			cout << ")\t";
		}
		cout << endl;
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
	// �ղ���ʽ
	if (vt == "epsilon")
		return EPSILON;
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
	// ͷ����ֵΪ���ս��head������
	p.head = insert_vn(head);
	for (string s : body) {
		auto s_split = Tool::split(s, "'");
		assert(s_split.size() == 1);
		if (s_split[0].size() == s.size()) {
			// �Ƿ��ս��
			p.body.push_back(insert_vn(s_split[0]));
		}
		else {
			// ���ս��
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

void SyntaxParser::build_LL_1_parsing_table()
{
	for (auto prod = productions.begin(), prod_end = productions.end(); prod != prod_end; ++prod) {
		// �����ķ�G��ÿ������ʽA->alpha
		set<int> first_alpha = first(prod->body);

		for (auto s = first_alpha.begin(), s_end = first_alpha.end(); s != s_end; ++s) {
			// ����FIRST(alpha)�е�ÿ���ս����a����A->alpha����M[A, a]��
			if (is_vt(*s)) {
				// ����Ҫ��is_vt(epsilon)==false
				// TODO: emm.. production���index��ʵ����ɾ��
				M[pair<int, int>(prod->head, *s)] = prod->index;
			}
		}
		// ���epsilon��FIRST(alpha)�У���ô����FOLLOW(A)�е�ÿ���ս����b����A->alpha���뵽M[A,b]��
		// ���epsilon��FIRST(alpha)�У���$��FOLLOW(A)�У���A->alpha����M[A, $]�� ����ע�⵽is_vt($) == false
		if (first_alpha.count(EPSILON) == 1) {
			set<int> follow_A = follow(prod->head);
			for (auto s = follow_A.begin(), s_end = follow_A.end(); s != s_end; ++s) {
				if (is_vt(*s))
					M[pair<int, int>(prod->head, *s)] = prod->index;
			}
			if (follow_A.count(DOLLAR) == 1) {
				M[pair<int, int>(prod->head, DOLLAR)] = prod->index;
			}
		}

	}
}

void SyntaxParser::parse_by_LL_1(vector<string> w)
{
	w.push_back("$");
	stack<string> s;
	s.push("$");
	s.push(symbols[0]);
	auto ip = w.begin();
	string X = s.top();
	while (X != "$") {
		// ���X���ս���Һ�ip��ָ�ķ�����ͬ���ջ
		if (X == *ip) {
			s.pop();
			++ip;
		}
		// ���X���ս��ȴ��ip��ָ���Ų�ͬ������
		else if (is_vt(X)) {
			cout << "error\n";
		}
		// ���M[X, a]�Ǳ�����Ŀ
		// hmm... ������ô����Ҳ����Ϊ��$�����ó����ű����ɵ�...Ҳ��$Ӧ�û��ǷŽ����ű����...
		// $ epsilon���ֵ�������ǲ��þͻ���һ��bug��...
		else if ((*ip != "$" && M.count(pair<int, int>(vn_map[X], vt_map[*ip])) == 0) || 
				(*ip == "$" && M.count(pair<int, int>(vn_map[X], DOLLAR)) == 0)) {
			cout << "error\n";
		}
		else {
			unsigned int prod_index;
			if(*ip == "$")
				prod_index = M[pair<int, int>(vn_map[X], DOLLAR)];
			else
				prod_index = M[pair<int, int>(vn_map[X], vt_map[*ip])];
			print_production(prod_index);
			cout << endl;
			s.pop();
			Production p = productions[prod_index];
			for (int i = p.body.size() - 1; i >= 0; --i) {
				if(p.body[i] != EPSILON) // ������ǿղ���ʽ����ջ
					s.push(symbols[p.body[i]]);
			}
		}
		X = s.top();
	}
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
		for (auto prod = productions.begin(), prod_end = productions.end();
				prod != prod_end; ++prod) {
			if (prod->head == X) {
				// �Բ���ʽ�������з��Ž��в���
				if (prod->body[0] == X) // ����ǿղ���ʽ����ݹ��򲻿��Ǹò���ʽ
					continue;
				
				int s_end = prod->body.back();

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
							if (s == s_end) {
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

// ����p140
set<int> SyntaxParser::first(vector<int>& beta)
{
	set<int> first_beta;
	// �����˿ղ���ʽ
	if (beta[0] == EPSILON) {
		first_beta.insert(EPSILON);
		return first_beta;
	}
		
	for (auto s = beta.begin(), s_end = beta.end(); s != s_end; ++s) {
		set<int> first_s = first(*s);
		if (first_s.count(EPSILON) == 0) {
			first_beta.insert(first_s.begin(), first_s.end());
			break;
		}
		else {
			// s�����մ� FIRST_BETA = FIRST_BETA �� FIRST_S - { epsilon }
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

// ����p141
set<int> SyntaxParser::follow(int X)
{
	auto found = follow_map.find(X);
	if (found != follow_map.end())
		return found->second;
	else
		// ����ʼ��ʮ�ֹؼ�������E-> TE��ʽ�ӣ�Ҫ��FOLLOW(E)ʱ��û������ʼ�� ������벻����FOLLOW(E)����ѭ��
		// �����ʼ����ÿ�����ս����followֻ��������ĵݹ�����һ��
		follow_map[X] = set<int>();

	// $����FOLLOW(S)������$�ǿ�ʼ���ţ�$���Ҷ˵Ľ������	
	set<int> follow_X;
	if (X == productions[0].head) {
		follow_X.insert(DOLLAR);
	}
	// ���������Ӧ������ð�ŵ����������Ƕ��ڸ��Ӷ��� ��ʹ�õ��������±꣬����++it����it++
	// ������������ A->aX��A->aXb��ʽ��
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
	// TODO: �������DOLLAR�����һ������
	if (x == EPSILON || x == DOLLAR)
		return false;
	return vt_map.count(symbols[x]) == 1;
}

bool SyntaxParser::is_vt(string s)
{
	if (s == "epsilon" || s == "$")
		return false;
	return vt_map.count(s) == 1;
}


