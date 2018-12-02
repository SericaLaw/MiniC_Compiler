#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <set>
#include <cassert>
#include <stack>
#include "Production.h"
#include "../Common/Token.h"
#include "../Common/Tool.h"
#include "LR_1_Item.h"
using namespace std;

class Parser
{
private:
	const int EPSILON = -1;
	const int DOLLAR = -2;
private:
	// ע��epsilon�������ս������ս����
	map<string, int> vn_map;
	map<string, int> vt_map;

	vector<string> symbols;
	/* �涨��˳�����productions ��һ����head�ǿ�ʼ���� */
	vector<Production> productions;
	map<int, set<int>> vn_production_map; // ���ս��vn�Ĳ���ʽ���ϣ����ں����㷨ִ��
	map<int, set<int>> first_map;
	map<int, set<int>> follow_map;

	// LL_1_table[A, a] = a production or error
	map<pair<int, int>, unsigned int> M;
public:
	Parser();
	~Parser();
public:
	void add_production(const string& p);
	void build_vn_production_map();
	// ��ʹ�õ���������ʱ���±���Ϣ�������ǵõ�����
	void build_production_index();
	/* �����з��ս����FIRST */
	void calc_first();
	/* �����з��ս����FOLLOW ����Ҫ�����FIRST */
	void calc_follow();
	/* LL1���� */
	/* ����p143 */
	void build_LL_1_parsing_table();
	/* ����p144 */
	// TODO��ע�������wʵ������token���У���ֻ����ʱ��string���� ��Ҫ������ݹ�
	void parse_by_LL_1(vector<Token>& w);

	/* LR���� */
	// �ò���ֱ����add_production֮��ʱ�����
	//void build_augmented_grammar();
public:
	/* debug */
	void print_production(int index);
	void print_productions();
	void print_first_map();
	void print_follow_map();
	void print_LL_1_parsing_table();

	void add_mini_c_production();
	// LR
	void print_LR_1_item(const LR_1_Item & i);
	void print_LR_1_set(const set<LR_1_Item>& s);
private:
	/* ������ս������������symbols�е����� */
	int insert_vn(const string& vn);
	/* �����ս������������symbols�е����� */
	int insert_vt(const string& vt);
	void add_production(const string& head, vector<string>& body);

	set<int> first(int X);
	/* ��beta��FIRST ����p140 */
	set<int> first(vector<int>& beta);

	set<int> follow(int X);
public:
	/* closure p156 */
	/* goto p156*/
	/* p157 */
private:
	/* �ж��Ƿ����ս�� */
	bool is_vt(int x);
	bool is_vt(string s);

};