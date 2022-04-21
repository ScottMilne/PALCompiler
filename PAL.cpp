#include "PALScanner.h"
#include "PALParser.h"
#include <iostream>
#include <fstream>

int main(int argc, const char** argv) {

	// First, we check that we have the right command-line arguments.
	// If we have anything other than a single argument, bail.
	if (argc != 2) {
		std::cerr << "fatal error: wrong number of arguments\n";
		std::cerr << "usage: Eg1 <source-file>\n";
		return 1;
	}

	// Then, we load our file into a file stream, and check that that happened successfully.
	std::ifstream input(argv[1]);
	if (!input.is_open()) {
		std::cerr << "fatal error: cannot open source file '" << argv[1] << "'\n";
		return 1;
	}

	// Finally, we can instanciate our scanner and do whatever we need with it!

	PALScanner scanner(input);
	
	
	while (!scanner.end()) {
		Token tok = scanner.lex();
		std::cout << "Token: " << tok.type() << "\n";
	}
	 

	PALParser parser(scanner);
	
	parser.parse();

	for (const Error& error : parser.errors())
	{
		std::cout << error << "\n";
	}
	
}