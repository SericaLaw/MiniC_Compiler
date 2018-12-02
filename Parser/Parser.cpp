#include "Parser.h"


Parser::Parser()
{
}


Parser::~Parser()
{
}

// p满足yacc规范，例如 expr : expr '+' expr | expr '-' expr
// 暂时不支持空产生式
void Parser::add_production(const string& p)
{
	vector<string> productions = Tool::split(p, "|");
	// 对第一个产生式单独处理
	vector<string> first = Tool::split(productions[0], " ");
	assert(first.size() >= 3);
	assert(first[1] == ":");

	string head = first[0];
	vector<string> body(first.begin() + 2, first.end());
	add_production(head, body);

	for (int i = 1, n = productions.size(); i < n; ++i) {
		// 对于单个产生式 形如expr : expr '+' expr
		vector<string> body = Tool::split(productions[i], " ");
		assert(body.size() >= 1); // 至少有三项：head, :, body
		add_production(head, body);
	}
}

void Parser::build_vn_production_map()
{
	for (auto prod = productions.begin(), prod_end = productions.end(); prod != prod_end; ++prod) {
		if (vn_production_map.count(prod->head) == 0) {
			set<int> p;
			p.insert(prod->index);
			vn_production_map.insert(make_pair(prod->head, p));
		}
		else {
			vn_production_map[prod->head].insert(prod->index);
		}
	}
}

void Parser::build_production_index()
{
	int i = 0;
	for (auto prod = productions.begin(), prod_end = productions.end(); prod != prod_end; ++prod) {
		prod->index = i++;
	}
}



// 该函数顺便验证了映射对应关系的正确性
void Parser::print_production(int index)
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

void Parser::print_productions()
{
	for (int i = 0, n = productions.size(); i < n; ++i) {
		print_production(i);
		cout << endl;
	}
}

void Parser::print_first_map()
{
	for (auto vn : vn_map) {
		cout << "First(" << vn.first << ") = { ";
		auto first_set = first_map[vn.second];
		for (auto ele : first_set) {
			if (ele == EPSILON)
				cout << "epsilon" << " ";
			else
				cout << symbols[ele] << " ";
		}
		cout << "}\n";
	}
}

void Parser::print_follow_map()
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

void Parser::print_LL_1_parsing_table()
{
	for (auto vn : vn_map) {
		cout << vn.first << ":\t";
		for (auto vt : vt_map) {
			if (M.count(pair<int, int>(vn.second, vt.second)) == 1) {
				int prod_index = M[pair<int, int>(vn.second, vt.second)];
				cout << "(" << vt.first << ": ";
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

void Parser::add_mini_c_production()
{
	add_production("program : stmts"); 
	add_production("stmts : stmt stmts | 'epsilon'");
	// if...else...
	add_production("stmt : 'if' '(' logical-expression ')' stmt 'else' stmt");

	add_production("stmt : assignment-expression ';'");
	// block
	add_production("stmt : block");
	

	/* block */
	add_production("block : '{' stmts '}'");


	add_production("assignment-expression : identifier '=' arithmetic-expression");
	add_production("arithmetic-expression : identifier a`");
	add_production("a` : '+' arithmetic-expression a` | '-' arithmetic-expression a` | 'epsilon'");
	add_production("logical-expression : 'true' | 'false'");
	/* identifier */
	add_production("identifier : 'id'");
}

void Parser::print_LR_1_item(const LR_1_Item & i)
{
	Production p = productions[i.production];
	cout << p.index << ": " << symbols[p.head] << " =>";
	assert(vn_map[symbols[p.head]] == p.head);
	int dot = 0;
	for (int s : p.body) {
		if (i.dot == dot)
			cout << " ·";
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

void Parser::print_LR_1_set(const set<LR_1_Item>& s)
{
	for (LR_1_Item i : s) {
		print_LR_1_item(i);
		cout << endl;
	}
}

// !important 这里不允许插入epsilon
int Parser::insert_vn(const string & vn)
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

int Parser::insert_vt(const string & vt)
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

void Parser::add_production(const string& head, vector<string>& body)
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

void Parser::calc_first()
{
	for (auto vn : vn_map)
		first(vn.second);
}

void Parser::calc_follow()
{
	for (auto vn : vn_map)
		follow(vn.second);
}

void Parser::build_LL_1_parsing_table()
{
	for (auto prod = productions.begin(), prod_end = productions.end(); prod != prod_end; ++prod) {
		// 对于文法G的每个产生式A->alpha
		set<int> first_alpha = first(prod->body);

		for (auto s = first_alpha.begin(), s_end = first_alpha.end(); s != s_end; ++s) {
			// 对于FIRST(alpha)中的每个终结符号a，把A->alpha加入M[A, a]中
			if (is_vt(*s)) {
				// 这里要求is_vt(epsilon)==false
				// TODO: emm.. production里的index其实可以删掉
				if (M.count(pair<int, int>(prod->head, *s)) == 0)
					M[pair<int, int>(prod->head, *s)] = prod->index;
				else {
					//cout << "WARNING: The grammar is not LL(1)!";
					break;
				}
			}
		}
		// 如果epsilon在FIRST(alpha)中，那么对于FOLLOW(A)中的每个终结符号b，将A->alpha加入到M[A,b]中
		// 如果epsilon在FIRST(alpha)中，且$在FOLLOW(A)中，将A->alpha加入M[A, $]中 这里注意到is_vt($) == false
		if (first_alpha.count(EPSILON) == 1) {
			set<int> follow_A = follow(prod->head);
			for (auto s = follow_A.begin(), s_end = follow_A.end(); s != s_end; ++s) {
				if (is_vt(*s)) {
					if (M.count(pair<int, int>(prod->head, *s)) == 0)
						M[pair<int, int>(prod->head, *s)] = prod->index;
					else {
						//cout << "WARNING: The grammar is not LL(1)!";
						break;
					}
				}
			}
			if (follow_A.count(DOLLAR) == 1) {
				if (M.count(pair<int, int>(prod->head, DOLLAR)) == 0)
					M[pair<int, int>(prod->head, DOLLAR)] = prod->index;
				else {
					//cout << "WARNING: The grammar is not LL(1)!";
					break;
				}
			}
		}

	}
}

void Parser::parse_by_LL_1(vector<Token>& w)
{
	Token end("$", "$", 0, 0, 0);
	w.push_back(end);
	stack<string> s;
	s.push("$");
	s.push(symbols[0]);
	auto ip = w.begin();
	string X = s.top();
	while (X != "$") {
		// 如果X是终结符且和ip所指的符号相同则出栈
		if (X == ip->name) {
			cout << "match " << s.top() << endl;
			s.pop();
			++ip;
		}
		// 如果X是终结符却和ip所指符号不同，报错
		else if (is_vt(X)) {
			cout << "error\n";
			break;
		}
		// 如果M[X, a]是报错条目
		// hmm... 下面这么复杂也是因为把$单独拿出符号表的造成的...也许$应该还是放进符号表里吧...
		// $ epsilon出现的情况考虑不好就会有一堆bug呢...
		else if ((ip->name != "$" && M.count(pair<int, int>(vn_map[X], vt_map[ip->name])) == 0) ||
			(ip->name == "$" && M.count(pair<int, int>(vn_map[X], DOLLAR)) == 0)) {
			cout << "error\n";
			break;
		}
		else {
			unsigned int prod_index;
			if (ip->name == "$")
				prod_index = M[pair<int, int>(vn_map[X], DOLLAR)];
			else
				prod_index = M[pair<int, int>(vn_map[X], vt_map[ip->name])];
			print_production(prod_index);
			cout << endl;
			s.pop();
			Production p = productions[prod_index];
			for (int i = p.body.size() - 1; i >= 0; --i) {
				if (p.body[i] != EPSILON) // 如果不是空产生式才入栈
					s.push(symbols[p.body[i]]);
			}
		}
		X = s.top();
	}
}

set<int> Parser::first(int X)
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
		//for (auto prod = productions.begin(), prod_end = productions.end();
		//	prod != prod_end; ++prod) {
		
		for(auto it = vn_production_map[X].begin(), end = vn_production_map[X].end();
			it != end; ++it) {
			Production* prod = &productions[*it];
			//if (prod->head == X) {
			assert(prod->head == X);
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
			//}
		}
	}
	first_map[X] = first_X;
	return first_X;
}

// 龙书p140
set<int> Parser::first(vector<int>& beta)
{
	set<int> first_beta;
	// 传入了空产生式
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
// TODO：该函数不能解决循环依赖的问题，以后需要改为不动点算法
// 因此，不能对单个非终结符X求，而要对所有非终结符一起求，应用规则直到某次迭代里不再有新的终结符加入某非终结符的FOLLOW
// FIRST也应同理
// 这些函数应当直接体现在calc_first和calc_follow中
set<int> Parser::follow(int X)
{
	auto found = follow_map.find(X);
	if (found != follow_map.end())
		return found->second;
	else
		// 这句初始化十分关键！形如E-> TE的式子，要求FOLLOW(E)时若没有这句初始化 则会陷入不断求FOLLOW(E)的死循环
		// 加入初始化，每个非终结符的follow只会在最顶部的递归中求一次
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


bool Parser::is_vt(int x)
{
	// TODO: 这里对于DOLLAR还须进一步讨论
	if (x == EPSILON || x == DOLLAR)
		return false;
	return vt_map.count(symbols[x]) == 1;
}

bool Parser::is_vt(string s)
{
	if (s == "epsilon" || s == "$")
		return false;
	return vt_map.count(s) == 1;
}


