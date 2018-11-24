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
	int cnt_line = 1; // ��ǰ�кţ������ڱ���
	int cnt_offset = 0; // ��ǰ����ƫ�ƣ������ڱ���
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
				cnt_offset = 0;
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
		cnt_offset++; 	// �ҵ���һ�����ʵĿ�ʼλ�ã������ַ���ż�1
		lexeme_begin = peek_forward; 		// �ҵ���һ�����ʵĿ�ʼλ�ã���p��ס��

		int cur_state = start_state;
		// �ƥ��ԭ��
		do {
			cur_state = move(cur_state, (int)code[peek_forward]);
			if (cur_state == -1) {
				cout << "error1 occurs!\n";
			}
			peek_forward++;
			cnt_offset++;
		} while (peek_forward < N && actions[cur_state] == "");
		// �����ı�ĩβ������״̬û�����ս�״̬
		if (cur_state != -1 && actions[cur_state] == "") {
			cout << "error2 orrcurs!\n";
		}
		// ������ս��㿪ʼ�������߹��������ս��㣬ֱ���ߵ�һ�����ս����ﵽ��״̬phai
		if (cur_state != -1 && actions[cur_state] != "") {
			// ��mark������������ս���
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
