// Console for OpenLab Scripting Language
// Compile: g++ main.cpp token.cpp lexer.cpp expr.cpp parser.cpp value.cpp builtins.cpp -o OpenLab.exe
#include <iostream>
#include <string>
#include <stdexcept>
#include <cmath>
#include "lexer.h"
#include "parser.h"
#include "builtins.h"

static bool needsMoreInput(const std::string& buffer, const std::string& lastLine) {
	size_t end = lastLine.find_last_not_of(" \t\r");
	if(end != std::string::npos && end >= 2 && lastLine.compare(end - 2, 3, "...") == 0) {
		return true;
	}
	
	std::vector<Token> tokens;
	try {
		tokens = Lexer(buffer).tokenize();
	} catch(const std::exception&) {
		return false;
	}
	
	int depth = 0;
	for(const auto& tok : tokens) {
		if(tok.type == TokenType::LBRACE) depth++;
		if(tok.type == TokenType::RBRACE) depth--;
	}
	
	return depth > 0;
}

int main(int const argc, char* argv[]) {
	Environment env;
	env["pi"] = std::acos(-1.0);
	env["e"] = std::exp(1.0);
	
	std::string buffer;
	std::string line;
	
	while(true) {
		std::cout << (buffer.empty() ? ">> " : ".. ");
		if(!std::getline(std::cin, line)) break;
		
		if(line == ":cancel") {
			buffer.clear();
			continue;
		}
		
		if(buffer.empty()) {
			if(line == ":funcs") { /* list built-ins */ continue; }
		}
		
		if(!buffer.empty()) buffer += '\n';
		buffer += line;
		
		if(needsMoreInput(buffer, line)) continue;
		std::string source = buffer;
		buffer.clear();
		
		try {
			Lexer lexer(source);
			Parser parser(lexer.tokenize());
			auto program = parser.parse();
			for(const auto& stmt : program) {
				(void)stmt->execute(env);
			}
		} catch(const std::exception& e) {
			std::cout << "Error: " << e.what() << "\n";
		}
	}
}