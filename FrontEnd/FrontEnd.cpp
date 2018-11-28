// FrontEnd.cpp: 定义控制台应用程序的入口点。
//
#include <iostream>
#include <string>
using namespace std;
#include "../Lexer/Lexer.h"

int main(int argc, char* argv[])
{
	const char* path_to_code = (argc > 1) ? argv[1] : nullptr;
	bool debug = (argc == 3 && string(argv[2]) == "debug") ? true : false;
	Lexer lexer;
	lexer.run(debug, path_to_code);
    return 0;
}

