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
	size_t cnt_line = 1; 		// 计数器-行号
	size_t cnt_offset = 0; 		// 计数器-行内字符序号
	size_t cnt_id = 0; 		// 计数器-内码

	int p = 0; // 单词起始位置
	int q = 0; // 当前扫描位置
	int N = code.size();
	while (true) {
		// 遇到空白符
		while (q < N && (code[q] == ' ' || code[q] == '\t' || code[q] == '\n')) {
			if (code[q] == '\n') {
				// 碰到换行符，则行号加一，行内字符序号清零
				cnt_line++;
				cnt_offset = 0;
			}
			else {
				// 碰到空格或tab，则行内字符序号加一
				cnt_offset++;
			}
			q++;
		}
		// 当到达文本末尾
		if (q >= N)
			return;
		cnt_offset++; 	// 找到了一个单词的开始位置，行内字符序号加1
		p = q; 		// 找到了一个单词的开始位置，用p记住它

		int cur_state = start_state;
		// 最长匹配
		do {
			cur_state = move(cur_state, (int)code[q]);
			if (cur_state == -1) {
				cout << "error1 occurs!\n";
			}
			q++;
			cnt_offset++;
		} while (q < N && actions[cur_state] == "");
		// 到达文本末尾，但是单词没有正常结束
		if (cur_state != -1 && actions[cur_state] == "") {
			cout << "error2 orrcurs!\n";
		}
		// 从这个终结结点开始，继续走过连续的终结结点，直至走到一个非终结结点
		if (cur_state != -1 && actions[cur_state] != "") {
			// 用mark来标记真正的终结结点
			int mark = cur_state;
			while (q < N) {
				cur_state = move(cur_state, (int)code[q]);
				if (cur_state == -1 || actions[cur_state] == "")
					break;
				mark = cur_state;
				cnt_offset++;
				q++;
			}

			cout << actions[mark] << ", " << code.substr(p, q - p) << endl;
		}
	}
}

int DFA::move(int state, int step)
{
	if (trans.count(pair<int, int>(state, step)) == 1)
		return trans[pair<int, int>(state, step)];
	return -1;
}
