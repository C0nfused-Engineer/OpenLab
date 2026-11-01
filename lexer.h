// lexer.h
#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <cctype>
#include <unordered_map>
#include <stdexcept>
#include <iostream>
#include "token.h"

class Lexer {
	public:
		explicit Lexer(const std::string& source);
		std::vector<Token> tokenize();
		void debug_print_tokens();
	
	private:
		std::string source;
		size_t pos = 0;
		int line = 1;
		int col = 1;
		std::vector<Token> tokens;
		
		char peek() const;
		char peekNext() const;
		char advance();
		bool atEnd() const;
		bool isSpace(char c) const;
		bool isDigit(char c) const;
		bool isAlpha(char c) const;		
		
		void lexNumber();
		void lexIdentifier();
		void addSingle(TokenType type);
		
		void addToken(TokenType type, std::string text, int l, int c);
};

#endif