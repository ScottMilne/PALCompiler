#include "PALScanner.h"
#include "PALParser.h"
#include <iostream>
#include <fstream>

int main(int argc, const char** argv) {

	if (argc != 2) {
		std::cerr << "fatal error: wrong number of arguments\n";
		std::cerr << "usage: Eg1 <source-file>\n";
		return 1;
	}

	std::ifstream input(argv[1]);
	if (!input.is_open()) {
		std::cerr << "fatal error: cannot open source file '" << argv[1] << "'\n";
		return 1;
	}

	PALScanner scanner(input);

	PALParser parser(scanner);
	


	parser.parse();

	for (const Error& error : parser.errors())
	{
		std::cout << error << "\n";
	}
	
}