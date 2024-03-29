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

TEST(parser, basic) {
	Parser parser;
	parser.add_production("E : T E`");
	parser.add_production("E` : '+' T E` | 'epsilon'");
	parser.add_production("T : F T`");
	parser.add_production("T` : '*' F T` | 'epsilon'");
	parser.add_production("F : '(' E ')' | 'id'");
	parser.print_productions();
	parser.build_vn_production_map();
	parser.calc_first();
	parser.calc_follow();
	parser.build_LL_1_parsing_table();

	parser.print_productions();
	parser.print_first_map();
	parser.print_follow_map();
	parser.print_LL_1_parsing_table();

	vector<Token> w;
	w.push_back(Token("id", "id", 0, 0, 0));
	w.push_back(Token("+", "+", 0, 0, 0));
	w.push_back(Token("id", "id", 0, 0, 0));
	w.push_back(Token("*", "*", 0, 0, 0));
	w.push_back(Token("id", "id", 0, 0, 0));
	parser.parse_by_LL_1(w);
}


TEST(lexer, case) {
	Lexer lexer;
	lexer.run(false);
}