#include "stdafx.h"
#include "DFA.h"


DFA::DFA()
{
}


DFA::~DFA()
{
}

DFA::DFA(map<pair<int, int>, int> trans, vector<string> actions)
{
	this->trans = trans;
	this->actions = actions;
	for (int i = 0, n = actions.size(); i < n; i++) {
		if (actions[i] == "start") {
			start_state = i;
			actions[i] = "";
			break;
		}
	}
}

void DFA::scan(const string & code)
{
	int cnt_line = 1; // 当前行号，可用于报错
	int cnt_offset = 0; // 当前行内偏移，可用于报错
	int id = 0; // id，用于token输出

	int lexeme_begin = 0; // 词素起始位置
	int peek_forward = 0; // peek
	int N = code.size();

	while (true) {
		// 遇到空白符
		while (peek_forward < N && (code[peek_forward] == ' ' || code[peek_forward] == '\t' || code[peek_forward] == '\n')) {
			if (code[peek_forward] == '\n') {
				// 换行符，行号加一，行内偏移量归零
				cnt_line++;
				cnt_offset = 0;
			}
			else {
				// 碰到空格或tab
				cnt_offset++;
			}
			peek_forward++;
		}
		// 到达源代码末尾
		if (peek_forward >= N)
			return;
		cnt_offset++; 	// 找到了一个单词的开始位置，行内字符序号加1
		lexeme_begin = peek_forward; 		// 找到了一个单词的开始位置，用p记住它

		int cur_state = start_state;
		// 最长匹配原则
		do {
			cur_state = move(cur_state, (int)code[peek_forward]);
			if (cur_state == -1) {
				cout << "error1 occurs!\n";
			}
			peek_forward++;
			cnt_offset++;
		} while (peek_forward < N && actions[cur_state] == "");
		// 到达文本末尾，但是状态没到达终结状态
		if (cur_state != -1 && actions[cur_state] == "") {
			cout << "error2 orrcurs!\n";
		}
		// 从这个终结结点开始，继续走过连续的终结结点，直至走到一个非终结结点或达到死状态phai
		if (cur_state != -1 && actions[cur_state] != "") {
			// 用mark来标记真正的终结结点
			int mark = cur_state;
			while (peek_forward < N) {
				cur_state = move(cur_state, (int)code[peek_forward]);
				if (cur_state == -1 || actions[cur_state] == "" || actions[cur_state] == "phai")
					break;
				mark = cur_state;
				cnt_offset++;
				peek_forward++;
			}

			cout << "< " << id++ << "\t" << actions[mark] << "\t" << code.substr(lexeme_begin, peek_forward - lexeme_begin) << " >" << endl;
		}
	}
}

int DFA::move(int state, int step)
{
	if (trans.count(pair<int, int>(state, step)) == 1)
		return trans[pair<int, int>(state, step)];
	return -1;
}
