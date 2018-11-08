#include "stdafx.h"
#include "FANode.h"
/* 初始化静态成员变量 */
int FANode::state_count = 0;

FANode::FANode()
{
	state_id = state_count++;
}


FANode::~FANode()
{
}

int FANode::get_state_id() const
{
	return state_id;
}

//bool FANode::get_terminal_state() const
//{
//	return is_terminal;
//}
//
//void FANode::set_terminal_state(bool state)
//{
//	is_terminal = state;
//}

string FANode::get_action() const
{
	return action;
}

void FANode::set_action(const string & action)
{
	this->action = action;
}

void FANode::add_neighbour(int step, FANode * neighbour)
{
	transition_table.push_back(pair<int, FANode*>(step, neighbour));
}

vector<pair<int, FANode*>>* FANode::get_transition_table() 
{
	return &transition_table;
}
