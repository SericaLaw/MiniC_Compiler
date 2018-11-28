// Test.cpp: 定义控制台应用程序的入口点。
//

#include "../Parser/Parser.cpp"
#include "../Lexer/Lexer.cpp"
#include <gtest\gtest.h>

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

TEST(parser, mini_c) {
	Parser parser;
	parser.add_mini_c_production();
	parser.calc_first();
	parser.calc_follow();
	parser.build_LL_1_parsing_table();

	parser.print_productions();
	parser.print_first_map();
	parser.print_follow_map();
	parser.print_LL_1_parsing_table();

	vector<string> w;
	w.push_back("if");
	w.push_back("(");
	w.push_back("true");
	w.push_back(")");
	w.push_back("{");
	w.push_back("id");
	w.push_back("=");
	w.push_back("id");
	w.push_back("+");
	w.push_back("id");
	w.push_back(";");
	w.push_back("}");
	w.push_back("else");
	w.push_back("{");
	w.push_back("id");
	w.push_back("=");
	w.push_back("id");
	w.push_back("-");
	w.push_back("id");
	w.push_back(";");
	w.push_back("}");

	parser.parse_by_LL_1(w);
}

TEST(lexer, case) {
	Lexer lexer;
	lexer.run(false);
}