#pragma once
#include <iostream>
#include <set>
// TODO: �ṹ��Ҫ����set�л���Ҫ����С������
struct LR_1_Item {
	LR_1_Item(unsigned p, unsigned int d): production(p), dot(d) {
		lookahead.clear();
	}
	unsigned int production; // ����ʽ��productions���е�λ��
	unsigned int dot; // �����ڵ�λ��
	set<int> lookahead;

};