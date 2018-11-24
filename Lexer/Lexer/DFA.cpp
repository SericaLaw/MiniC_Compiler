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
	size_t cnt_line = 1; 		// ������-�к�
	size_t cnt_offset = 0; 		// ������-�����ַ����
	size_t cnt_id = 0; 		// ������-����

	int p = 0; // ������ʼλ��
	int q = 0; // ��ǰɨ��λ��
	int N = code.size();
	while (true) {
		// �����հ׷�
		while (q < N && (code[q] == ' ' || code[q] == '\t' || code[q] == '\n')) {
			if (code[q] == '\n') {
				// �������з������кż�һ�������ַ��������
				cnt_line++;
				cnt_offset = 0;
			}
			else {
				// �����ո��tab���������ַ���ż�һ
				cnt_offset++;
			}
			q++;
		}
		// �������ı�ĩβ
		if (q >= N)
			return;
		cnt_offset++; 	// �ҵ���һ�����ʵĿ�ʼλ�ã������ַ���ż�1
		p = q; 		// �ҵ���һ�����ʵĿ�ʼλ�ã���p��ס��

		int cur_state = start_state;
		// �ƥ��
		do {
			cur_state = move(cur_state, (int)code[q]);
			if (cur_state == -1) {
				cout << "error1 occurs!\n";
			}
			q++;
			cnt_offset++;
		} while (q < N && actions[cur_state] == "");
		// �����ı�ĩβ�����ǵ���û����������
		if (cur_state != -1 && actions[cur_state] == "") {
			cout << "error2 orrcurs!\n";
		}
		// ������ս��㿪ʼ�������߹��������ս��㣬ֱ���ߵ�һ�����ս���
		if (cur_state != -1 && actions[cur_state] != "") {
			// ��mark������������ս���
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
