#include "stdafx.h"
#include "NFA.h"


NFA::NFA()
{
	start_node = new FANode();
	end_node = new FANode();
}

NFA::NFA(int step)
{
	start_node = new FANode();
	end_node = new FANode();
	start_node->add_neighbour(step, end_node);
}


NFA::~NFA()
{
}

FANode * NFA::get_start_node()
{
	return start_node;
}

void NFA::set_start_node(FANode * start_node)
{
	this->start_node = start_node;
}

FANode * NFA::get_end_node()
{
	return end_node;
}

void NFA::set_end_node(FANode * end_node)
{
	this->end_node = end_node;
}

set<int>& NFA::get_alphabet()
{
	return alphabet;
}

void NFA::join(NFA * right)
{
	end_node->add_neighbour(FANode::EPSILON, right->get_start_node());
	right->set_start_node(nullptr);
	
	end_node = right->get_end_node();
	right->set_end_node(nullptr);
}

void NFA::union_with(NFA * right)
{
	FANode * new_start = new FANode();
	FANode * new_end = new FANode();
	new_start->add_neighbour(FANode::EPSILON, start_node);
	new_start->add_neighbour(FANode::EPSILON, right->get_start_node());

	end_node->add_neighbour(FANode::EPSILON, new_end);
	right->get_end_node()->add_neighbour(FANode::EPSILON, new_end);

	start_node = new_start;
	end_node = new_end;

	right->set_start_node(nullptr);
	right->set_end_node(nullptr);
}

void NFA::make_star_closure()
{
	FANode * new_start = new FANode();
	FANode * new_end = new FANode();
	
	new_start->add_neighbour(FANode::EPSILON, start_node);
	new_start->add_neighbour(FANode::EPSILON, new_end);
	
	end_node->add_neighbour(FANode::EPSILON, start_node);
	end_node->add_neighbour(FANode::EPSILON, new_end);

	start_node = new_start;
	end_node = new_end;
}

void NFA::make_question_closure()
{
	FANode * new_start = new FANode();
	FANode * new_end = new FANode();
	
	new_start->add_neighbour(FANode::EPSILON, start_node);
	new_start->add_neighbour(FANode::EPSILON, new_end);

	end_node->add_neighbour(FANode::EPSILON, new_end);

	start_node = new_start;
	end_node = new_end;
}

void NFA::make_plus_closure()
{
	FANode * new_start = new FANode();
	FANode * new_end = new FANode();

	new_start->add_neighbour(FANode::EPSILON, start_node);

	end_node->add_neighbour(FANode::EPSILON, start_node);
	end_node->add_neighbour(FANode::EPSILON, new_end);

	start_node = new_start;
	end_node = new_end;
}

void NFA::create_from_regex(const string & postfix_pattern, const string & action)
{
	stack<NFA *> operands;
	NFA * nfa_1 = nullptr;
	NFA * nfa_2 = nullptr;
	for (char c : postfix_pattern) {
		if (c == '*') {
			nfa_1 = operands.top();
			operands.pop();
			nfa_1->make_star_closure();
			operands.push(nfa_1);
		}
		else if (c == '+') {
			nfa_1 = operands.top();
			operands.pop();
			nfa_1->make_plus_closure();
			operands.push(nfa_1);
		}
		else if (c == '?') {
			nfa_1 = operands.top();
			operands.pop();
			nfa_1->make_question_closure();
			operands.push(nfa_1);
		}
		else if (c == '|') {
			nfa_2 = operands.top();
			operands.pop();
			nfa_1 = operands.top();
			operands.pop();

			nfa_1->union_with(nfa_2);
			operands.push(nfa_1);

			// delete nfa_2
		}
		else if (c == '#') {
			nfa_2 = operands.top();
			operands.pop();
			nfa_1 = operands.top();
			operands.pop();

			nfa_1->join(nfa_2);
			operands.push(nfa_1);

			// delete nfa_2
		}
		else {
			nfa_1 = new NFA((int)c);
			operands.push(nfa_1);
			alphabet.insert((int)c);
		}

		
	}
	NFA * res = operands.top();
	operands.pop();
	assert(operands.empty());
	start_node = res->get_start_node();
	
	end_node = res->get_end_node();
	end_node->set_action(action);
}
// TODO: 这是暂时的函数，用于测试用，实际使用时只需要传入vector<Regex> regexes, action为regex的lexme成员，这些需要在构造类constructor里实现
void NFA::create_from_regexes(const vector<string>& regexes, const vector<string>& actions)
{
	assert(regexes.size() == actions.size());
	vector<NFA> nfas;
	for (int i = 0, n = regexes.size(); i < n; i++) {
		NFA nfa;
		nfa.create_from_regex(Regex(regexes[i]).to_postfix(), actions[i]);
		nfas.push_back(nfa);
		// 也要复制一份字母表
		auto _alphabet = nfa.get_alphabet();
		for (int a : _alphabet) {
			alphabet.insert(a);
		}
	}
	for (NFA nfa : nfas) {
		start_node->add_neighbour(FANode::EPSILON, nfa.get_start_node());
	}
}

void NFA::print()
{
	queue<FANode *> nodes;
	nodes.push(start_node);

	vector<FANode *> visited;

	FANode * cur = nullptr;
	vector<pair<int, FANode*>>* transition_table = nullptr;
	
	while (!nodes.empty()) {
		cur = nodes.front();
		nodes.pop();
		transition_table = cur->get_transition_table();

		if (visited.end() == find(visited.begin(), visited.end(), cur)) {
	/*		if (!visited.empty() && visited.back() == cur)
				continue;*/
			cout << cur->get_state_id() << " (action: " << cur->get_action() << "): ";

			visited.push_back(cur);
			transition_table = cur->get_transition_table();
			for (pair<int, FANode*> neighbour : *transition_table) {
				nodes.push(neighbour.second);
				cout << "(--" << (char) neighbour.first << "-->" << neighbour.second->get_state_id() << ")";
			}
			cout << endl;
		}
	}

	cout << "alphabet:\n";
	for (char c : alphabet) {
		cout << c << ' ';
	}
	cout << endl;
}
