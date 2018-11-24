// Lexer.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

DFA build_min_dfa();

// 传入参数 argv[1]=path_to_code
int main(int argc, char* argv[])
{
	string code, line;
	ifstream in;
	const char* path_to_code = argv[1];
	//const char* path_to_code = "C:\\Users\\Serica\\Archive\\大三上\\编译原理\\MiniC_Compiler\\Lexer\\test\\code.cpp";
	in.open(path_to_code);
	cout << "Source code:\n";
	if (in.is_open()) {
		while (getline(in, line)) {
			code.append(line);
			code.append("\n");
		}
		in.close();
	}
	cout << code;
	cout << "Building Min DFA now... It can take minutes, please be patient and wait...\n";
	DFA dfa = build_min_dfa();
	
	dfa.scan(code);

    return 0;
}

// 在下面配置可识别的正则表达式
DFA build_min_dfa() {
	vector<string> regexes;
	vector<string> actions;
	/* operators */
	// asignment operator
	regexes.push_back("=");
	actions.push_back("=");
	regexes.push_back("\\+=");
	actions.push_back("+=");
	regexes.push_back("-=");
	actions.push_back("-=");

	regexes.push_back("\\*=");
	actions.push_back("*=");
	regexes.push_back("/=");
	actions.push_back("/=");
	regexes.push_back("%=");
	actions.push_back("%=");

	regexes.push_back("<<=");
	actions.push_back("<<=");
	regexes.push_back(">>=");
	actions.push_back(">>=");
	// algo
	regexes.push_back("\\+");
	actions.push_back("+");
	regexes.push_back("-");
	actions.push_back("-");

	regexes.push_back("\\*");
	actions.push_back("*");
	regexes.push_back("/");
	actions.push_back("/");
	regexes.push_back("%");
	actions.push_back("%");

	regexes.push_back("<<");
	actions.push_back("<<");
	regexes.push_back(">>");
	actions.push_back(">>");

	// unary
	regexes.push_back("\\+\\+");
	actions.push_back("++");
	regexes.push_back("--");
	actions.push_back("--");
	regexes.push_back("~");
	actions.push_back("~");

	// logic
	regexes.push_back("&");
	actions.push_back("&");
	regexes.push_back("&&");
	actions.push_back("&&");

	regexes.push_back("\\|");
	actions.push_back("|");
	regexes.push_back("\\|\\|");
	actions.push_back("||");

	regexes.push_back("!");
	actions.push_back("!");

	// relation
	regexes.push_back("==");
	actions.push_back("eq");
	regexes.push_back("<");
	actions.push_back("lt");
	regexes.push_back(">");
	actions.push_back("gt");
	regexes.push_back("<=");
	actions.push_back("lte");
	regexes.push_back(">=");
	actions.push_back("gte");
	/* punctuator */
	regexes.push_back(":");
	actions.push_back(":");
	regexes.push_back("\\?");
	actions.push_back("?");

	regexes.push_back(".");
	actions.push_back(".");
	regexes.push_back(",");
	actions.push_back(",");
	regexes.push_back(";");
	actions.push_back(";");
	regexes.push_back("\"");
	actions.push_back("\"");
	regexes.push_back("'");
	actions.push_back("'");
	regexes.push_back("{");
	actions.push_back("{");
	regexes.push_back("}");
	actions.push_back("}");

	regexes.push_back("(");
	actions.push_back("(");
	regexes.push_back(")");
	actions.push_back(")");

	regexes.push_back("[");
	actions.push_back("[");
	regexes.push_back("]");
	actions.push_back("]");

	/* keywords */
	// type specifier
	regexes.push_back("int");
	actions.push_back("int");
	regexes.push_back("char");
	actions.push_back("char");
	regexes.push_back("bool");
	actions.push_back("bool");
	regexes.push_back("void");
	actions.push_back("void");
	regexes.push_back("struct");
	actions.push_back("struct");
	regexes.push_back("enum");
	actions.push_back("enum");
	regexes.push_back("class");
	actions.push_back("class");
	regexes.push_back("public");
	actions.push_back("public");
	regexes.push_back("private");
	actions.push_back("private");
	// storage class specifier
	regexes.push_back("typedef");
	actions.push_back("typedef");
	regexes.push_back("static");
	actions.push_back("static");
	// type qualifier
	regexes.push_back("const");
	actions.push_back("const");
	// statements
	regexes.push_back("if");
	actions.push_back("if");
	regexes.push_back("else");
	actions.push_back("else");
	regexes.push_back("switch");
	actions.push_back("switch");
	regexes.push_back("case");
	actions.push_back("case");
	regexes.push_back("default");
	actions.push_back("default");
	regexes.push_back("for");
	actions.push_back("for");
	regexes.push_back("do");
	actions.push_back("do");
	regexes.push_back("while");
	actions.push_back("while");
	regexes.push_back("break");
	actions.push_back("break");
	regexes.push_back("continue");
	actions.push_back("continue");
	regexes.push_back("return");
	actions.push_back("return");
	regexes.push_back("main");
	actions.push_back("main");
	regexes.push_back("true");
	actions.push_back("true");
	regexes.push_back("false");
	actions.push_back("false");
	

	
	/* identifier */
	regexes.push_back("[_a-zA-Z][_a-zA-Z0-9]*");
	actions.push_back("id");
	/* number */
	regexes.push_back("[0-9]+");
	actions.push_back("integer");
	cout << "...Regular expressions loaded, start to build NFA from them.\n";

	clock_t start, end;
	NFA nfa;

	start = clock();
	nfa.create_from_regexes(regexes, actions);
	end = clock();
	double time_consumed = (double)(end - start) / CLOCKS_PER_SEC;
	//double time_consumed = 1;
	cout << "...NFA already built, takes " << time_consumed << "s, now start to build DFA from the NFA.\n";

	Optimizer o;

	start = clock();
	o.build_dfa(nfa);
	end = clock();
	time_consumed = (double)(end - start) / CLOCKS_PER_SEC;
	cout << "...DFA already built, takes " << time_consumed << "s, now start to minimize the DFA.\n";

	//o.print_d_trans();
	//cout << endl;
	o.minimize_dfa();
	//o.print_min_dfa_trans();
	
	start = clock();
	DFA dfa(o.get_min_dfa_trans(), o.get_min_dfa_actions());
	end = clock();
	time_consumed = (double)(end - start) / CLOCKS_PER_SEC;
	cout << "...Min DFA already built, takes " << time_consumed << "s, ready to do lexical analysis.\n";
	return dfa;
}
