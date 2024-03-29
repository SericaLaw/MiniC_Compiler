// FrontEnd.cpp: 定义控制台应用程序的入口点。
//
#include <iostream>
#include <string>
using namespace std;
#include "../Lexer/Lexer.h"
#include "../Parser/Parser.h"
#include "../Common/Token.h"
int main(int argc, char* argv[])
{
	const char* path_to_code = (argc > 1) ? argv[1] : nullptr;
	bool debug = (argc == 3 && string(argv[2]) == "debug") ? true : false;
	Lexer lexer;
	if (lexer.run(debug, path_to_code) == true) {
		cout << "Lexical analysis finished successfully. Now start to parse.\n";
	}
	vector<Token> tokens = lexer.get_tokens();

	Parser parser;
	parser.add_mini_c_production();
	parser.build_vn_production_map();
	parser.calc_first();
	parser.calc_follow();
	parser.build_LL_1_parsing_table();

	parser.print_productions();
	parser.print_first_map();
	parser.print_follow_map();
	parser.print_LL_1_parsing_table();

	parser.parse_by_LL_1(tokens);
    return 0;


}

