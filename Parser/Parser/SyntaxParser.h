#pragma once
class SyntaxParser
{
private:
	const int EPSILON = -1;
	const int DOLLAR = -2;
private:
	// ע��epsilon�������ս������ս����
	map<string, int> vn_map;
	map<string, int> vt_map;

	vector<string> symbols;
	/* �涨��˳�����productions ��һ����head�ǿ�ʼ����*/
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
	/* �����з��ս����FIRST */
	void calc_first();
	/* �����з��ս����FOLLOW ����Ҫ�����FIRST */
	void calc_follow();
	/* ����p143 */
	void build_LL_1_parsing_table();
	/* ����p144 */
	// TODO��ע�������wʵ������token���У���ֻ����ʱ��string���� ��Ҫ������ݹ�
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
	/* ������ս������������symbols�е����� */
	int insert_vn(const string& vn);
	/* �����ս������������symbols�е����� */
	int insert_vt(const string& vt);
	void add_production(const string& head, vector<string>& body);

	set<int> first(int X);
	/* ��beta��FIRST ����p140 */
	set<int> first(vector<int>& beta);

	set<int> follow(int X);
	
	/* �ж��Ƿ����ս�� */
	bool is_vt(int x);
	bool is_vt(string s);
	
};

