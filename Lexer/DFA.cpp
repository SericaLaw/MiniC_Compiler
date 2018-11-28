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
	int cnt_line = 1; // ��ǰ�кţ������ڱ���
	int cnt_offset = 1; // ��ǰ����ƫ�ƣ������ڱ���
	int id = 0; // id������token���

	int lexeme_begin = 0; // ������ʼλ��
	int peek_forward = 0; // peek
	int N = code.size();

	while (true) {
		// �����հ׷�
		while (peek_forward < N && (code[peek_forward] == ' ' || code[peek_forward] == '\t' || code[peek_forward] == '\n')) {
			if (code[peek_forward] == '\n') {
				// ���з����кż�һ������ƫ��������
				cnt_line++;
				cnt_offset = 1;
			}
			else {
				// �����ո��tab
				cnt_offset++;
			}
			peek_forward++;
		}
		// ����Դ����ĩβ
		if (peek_forward >= N)
			return;
		cnt_offset++; // �ҵ���һ��������ʼ�㣬�����ַ���ż�1����¼
		lexeme_begin = peek_forward;

		int cur_state = start_state;
		// �ƥ��ԭ��
		// TODO: ������û�п���ȫ����� ��Ҫ�޸�
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
		// ���������β������״̬�Ƿǽ���״̬
		if (cur_state != -1 && (actions[cur_state] == "" || actions[cur_state] == "phai")) {
			output_error(cnt_line, cnt_offset - 1, code.substr(lexeme_begin, peek_forward - lexeme_begin));
		}
		// �Ӹý���״̬��ʼ�������߹������Ľ���״̬��ֱ���ߵ�һ�����ս�״̬��ﵽ��״̬phai
		if (cur_state != -1 && actions[cur_state] != "" && actions[cur_state] != "phai") {
			// ��mark�������󵽴�Ľ���״̬���ƥ�䣩
			int mark = cur_state;
			while (peek_forward < N) {
				cur_state = move(cur_state, (int)code[peek_forward]);
				if (cur_state == -1 || actions[cur_state] == "" || actions[cur_state] == "phai")
					break;
				mark = cur_state;
				cnt_offset++;
				peek_forward++;
			}

			// ������ű�
			string attribute = actions[mark];
			string value = code.substr(lexeme_begin, peek_forward - lexeme_begin);
			if (attribute == "id") {
				if (find(symbol_table.begin(), symbol_table.end(), value) != symbol_table.end()) {
					// ����Ǳ�����
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
