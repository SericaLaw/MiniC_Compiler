#pragma once
#include <iostream>
#include <set>
// TODO: 结构体要放入set中还需要重载小于运算
struct LR_1_Item {
	LR_1_Item(unsigned p, unsigned int d): production(p), dot(d) {
		lookahead.clear();
	}
	unsigned int production; // 产生式在productions表中的位置
	unsigned int dot; // 点所在的位置
	set<int> lookahead;

};