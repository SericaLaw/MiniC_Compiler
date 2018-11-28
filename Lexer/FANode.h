#pragma once
#include <string>
#include <vector>
using namespace std;

class FANode
{
private:
	static int state_count;
private:
	/* 状态号 */
	int state_id;
	//bool is_terminal = false;
	string action = "";
	/* transition_map[step_forward] = FANode 不可以，因为NFA里会出现一个step对应多个节点*/

	vector<pair<int, FANode*>> transition_table;
public:
	FANode();
	~FANode();

	const static int EPSILON = -1;
	int get_state_id() const;

	//bool get_terminal_state() const;
	//void set_terminal_state(bool state);

	string get_action() const;
	void set_action(const string & action);

	void add_neighbour(int step, FANode* neighbour);
	vector<pair<int, FANode*>>* get_transition_table();

};

