#pragma once
class NFA
{
private:
	FANode* start_node;
	FANode* end_node;
	
public:
	NFA();
	NFA(int step);
	~NFA();

	FANode * get_start_node();
	void set_start_node(FANode * start_node);
	FANode * get_end_node();
	void set_end_node(FANode * end_node);


	/* 连接运算 */
	void join(NFA * right);
	void union_with(NFA * right);
	void make_star_closure();
	void make_question_closure();
	void make_plus_closure();

	/* 扫描一个后缀形式的正则表达式，由此创建匹配该模式，终结节点接收action的NFA */
	void create_from_regex(const string& postfix_pattern, const string & action);
	void print();
};

