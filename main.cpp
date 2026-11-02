// Console for OpenLab Scripting Language
// Compile: g++ main.cpp token.cpp lexer.cpp expr.cpp parser.cpp value.cpp builtins.cpp -o OpenLab.exe
#include <iostream>
#include <string>
#include <stdexcept>
#include <cmath>
#include "lexer.h"
#include "parser.h"
#include "builtins.h"

int main(int const argc, char* argv[]) {
	Environment env;
	env["pi"] = std::acos(-1.0);
	env["e"] = std::exp(1.0);
	
	
	std::string input;
	while(std::cout << ">> ", std::getline(std::cin, input)) {
		if (input == ":funcs") {
			for (const auto& [name, b] : getBuiltins())
				std::cout << "  " << name << " - " << b.help << "\n";
			continue;
		}
		
		try {
			Lexer lexer(input);
			Parser parser(lexer.tokenize());
			
			auto program = parser.parse();
			
			for(const auto& stmt : program) {
				stmt->execute(env);
			}
			
		} catch(const std::exception& e) {
			std::cout << "Error: " << e.what() << "\n";
		}
	}
	
	
	return 0;
}