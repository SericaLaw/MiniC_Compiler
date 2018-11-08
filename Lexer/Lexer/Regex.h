#pragma once
#include "RegexOperator.h"
class Regex
{
private:
	/* 匹配模式，这里假设正则表达式没有语法错误，如括号、运算符不匹配，非法中文字符，非法语法（如[-z]）等 */
	/* 暂时没有考虑转义字符，所以假设输入的正则表达式里，方括号替换后的字符串没有'-'，否则会无限循环...*/
	string pattern = "";
	/* 后缀表达式形式 */
	string postfix = "";
	/* 词素，从.l文件获取词法描述时，根据每个模式的action获取相应词素，意味着正则表达式的状态机的终结节点采取的行动 */
	string lexeme = "";
public:
	/* 仅在构造时指定正则表达式，不可在稍后被修改 */
	Regex(string pattern);
	Regex(string pattern, string lexeme);
	string get_lexeme();
	string to_postfix();
	string get_pattern();
private:
	/*  对包含正则定义的正则表达式进行预处理，该函数在转为后缀的时候调用 */
	void preprocess();
};

