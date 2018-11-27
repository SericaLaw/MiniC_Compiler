#pragma once
class SyntaxParser
{
private:
	const int EPSILON = -1;
private:
	// ע��epsilon�������ս������ս����
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
	/* �����з��ս����FIRST */
	void calc_first();
public:
	/* debug */
	void print_productions();
	void print_first_map();
private:
	/* ������ս������������symbols�е����� */
	int insert_vn(const string& vn);
	/* �����ս������������symbols�е����� */
	int insert_vt(const string& vt);
	void add_production(const string& head, vector<string>& body);

	set<int> first(int X);
	set<int> follow(int X);

	/* �ж��Ƿ����ս�� */
	bool is_vt(int x);
	
};

