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
	/* keywords */
	// type specifier
	symbol_table.push_back("int");
	symbol_table.push_back("char");
	symbol_table.push_back("bool");
	symbol_table.push_back("void");
	symbol_table.push_back("struct");
	symbol_table.push_back("enum");
	symbol_table.push_back("class");
	symbol_table.push_back("public");
	symbol_table.push_back("private");
	// storage class specifier
	symbol_table.push_back("typedef");
	symbol_table.push_back("static");
	// type qualifier
	symbol_table.push_back("const");
	// statements
	symbol_table.push_back("if");
	symbol_table.push_back("else");
	symbol_table.push_back("switch");
	symbol_table.push_back("case");
	symbol_table.push_back("default");
	symbol_table.push_back("for");
	symbol_table.push_back("do");
	symbol_table.push_back("while");
	symbol_table.push_back("break");
	symbol_table.push_back("continue");
	symbol_table.push_back("return");
	symbol_table.push_back("this");
	symbol_table.push_back("new");
	symbol_table.push_back("delete");
	symbol_table.push_back("friend");
	symbol_table.push_back("true");
	symbol_table.push_back("false");
}

void DFA::scan(const string & code)
{
	int cnt_line = 1; // 当前行号，可用于报错
	int cnt_offset = 1; // 当前行内偏移，可用于报错
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
				cnt_offset = 1;
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
		cnt_offset++; // 找到了一个词素起始点，行内字符序号加1，记录
		lexeme_begin = peek_forward;

		int cur_state = start_state;
		// 最长匹配原则
		// TODO: 这里有没有考虑全的情况 需要修复
		do {
			cur_state = move(cur_state, (int)code[peek_forward]);
			if (cur_state == -1) {
				output_error(cnt_line, cnt_offset, code.substr(lexeme_begin, peek_forward - lexeme_begin + 1));
				cnt_offset -= 2;
				peek_forward++;
				break;
			}
			peek_forward++;
			cnt_offset++;
		} while (peek_forward < N && actions[cur_state] == "");
		// 读到程序结尾，但是状态是非接受状态
		if (cur_state != -1 && (actions[cur_state] == "" || actions[cur_state] == "phai")) {
			output_error(cnt_line, cnt_offset - 1, code.substr(lexeme_begin, peek_forward - lexeme_begin));
		}
		// 从该接受状态开始，继续走过连续的接收状态，直至走到一个非终结状态或达到死状态phai
		if (cur_state != -1 && actions[cur_state] != "" && actions[cur_state] != "phai") {
			// 用mark来标记最后到达的接收状态（最长匹配）
			int mark = cur_state;
			while (peek_forward < N) {
				cur_state = move(cur_state, (int)code[peek_forward]);
				if (cur_state == -1 || actions[cur_state] == "" || actions[cur_state] == "phai")
					break;
				mark = cur_state;
				cnt_offset++;
				peek_forward++;
			}

			// 处理符号表
			string attribute = actions[mark];
			string value = code.substr(lexeme_begin, peek_forward - lexeme_begin);
			if (attribute == "id") {
				if (find(symbol_table.begin(), symbol_table.end(), value) != symbol_table.end()) {
					// 如果是保留字
					output_res(id++, value, value);
					continue;
				}
			}
			output_res(id++, actions[mark], code.substr(lexeme_begin, peek_forward - lexeme_begin));
		}
	}
}

void DFA::output_res(int id, const string & lexeme, const string & word)
{
	cout << "< " << id << "\t" << lexeme << "\t" << word << " >" << endl;
}

void DFA::output_error(int line, int offset, const string & word)
{
	cout << "error occurs in line (" << line << ", " << offset << "): " << word << endl;
}

int DFA::move(int state, int step)
{
	if (trans.count(pair<int, int>(state, step)) == 1)
		return trans[pair<int, int>(state, step)];
	return -1;
}
