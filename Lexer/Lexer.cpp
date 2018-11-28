#include "Lexer.h"

Lexer::Lexer()
{
}


Lexer::~Lexer()
{
}

void Lexer::build_mini_c_min_dfa(bool debug)
{
	if (debug)
		cout << "***IN DEBUG MOD***\n";
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
	regexes.push_back("!=");
	actions.push_back("ne");
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
	actions.push_back("LP");
	regexes.push_back("}");
	actions.push_back("RP");

	regexes.push_back("(");
	actions.push_back("SLP");
	regexes.push_back(")");
	actions.push_back("SRP");

	regexes.push_back("[");
	actions.push_back("SLB");
	regexes.push_back("]");
	actions.push_back("SRB");
	/* identifier */
	regexes.push_back("[_a-zA-Z][_a-zA-Z0-9]*");
	actions.push_back("id");
	/* number */
	regexes.push_back("[0-9]+");
	actions.push_back("integer");
	regexes.push_back("[0-9]+(.[0-9]*)?");
	actions.push_back("real");
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
	if (debug)
		o.print_d_trans();
	//cout << endl;
	o.minimize_dfa();
	if (debug)
		o.print_min_dfa_trans();

	start = clock();
	DFA d(o.get_min_dfa_trans(), o.get_min_dfa_actions());
	end = clock();
	time_consumed = (double)(end - start) / CLOCKS_PER_SEC;
	cout << "...Min DFA already built, takes " << time_consumed << "s, ready to do lexical analysis.\n";
	dfa = d;
}

void Lexer::run(bool debug, const char * path_to_code)
{
	string code, line;
	ifstream in;
	//const char* path_to_code = argv[1];
	if (path_to_code == nullptr)
		path_to_code = "C:\\Users\\Serica\\Archive\\大三上\\编译原理\\MiniC_Compiler\\assets\\code.cpp";
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
	build_mini_c_min_dfa(debug);

	dfa.scan(code);
}
