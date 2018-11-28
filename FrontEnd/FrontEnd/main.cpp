#include "../../Parser/Parser/SyntaxParser.h"
int main() 
{
	SyntaxParser parser;
	parser.add_production("E : T E`");
	parser.add_production("E` : '+' T E` | 'epsilon'");
	parser.add_production("T : F T`");
	parser.add_production("T` : '*' F T` | 'epsilon'");
	parser.add_production("F : '(' E ')' | 'id'");
	parser.print_productions();
	getchar();
}