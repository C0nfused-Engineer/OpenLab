#include "lexer.h"

static const std::unordered_map<std::string, TokenType> keywords = {
	{"if", TokenType::IF},
	{"else", TokenType::ELSE},
	{"while", TokenType::WHILE},
	{"int", TokenType::INT},
	{"float", TokenType::FLOAT},
	{"true", TokenType::TRU},
	{"false", TokenType::FAL},
};

Lexer::Lexer(const std::string& source) : source(source) {}

std::vector<Token> Lexer::tokenize() {
	while(!atEnd()) {
		char c = peek();
		
		if(isSpace(c)) { 
			advance();
		} else if(isDigit(c) || (c == '.' && isDigit(peekNext()))) { // NUM literal
			lexNumber();
		} else if(isAlpha(c) || c == '_') { // IDENT or KEYWORD
			lexIdentifier();
		} else {
			switch(c) {
				case '=': // Case for 2 char tokens
					if(peekNext() == '=') {
						int startCol = col;
						advance(); advance();
						addToken(TokenType::EQUAL_EQUAL, "==", line, startCol);
					} else { 
						addSingle(TokenType::ASSIGN); 
					}
				break;
				case '(': addSingle(TokenType::LPAREN); break; // Cases for 1 char tokens
				case ')': addSingle(TokenType::RPAREN); break;
				case '[': addSingle(TokenType::LBRACK); break;
				case ']': addSingle(TokenType::RBRACK); break;
				case '{': addSingle(TokenType::LBRACE); break;
				case '}': addSingle(TokenType::RBRACE); break;
				case '+': addSingle(TokenType::PLUS); break;
				case '-': addSingle(TokenType::MINUS); break;
				case '*': addSingle(TokenType::ASTR); break;
				case '/': addSingle(TokenType::SLASH); break;
				case ';': addSingle(TokenType::SEMI); break;
				case '^': addSingle(TokenType::CARET); break;
				case ',': addSingle(TokenType::COMMA); break;
				default: 
					throw std::runtime_error(std::string("unexpected character '") + c + "' at column " + std::to_string(col));
			}
		}
	}
	addToken(TokenType::END, "", line, col);
	
	return tokens;
}

char Lexer::peek() const {
	if(!atEnd()) {
		return source[pos];
	} else {
		return '\0';
	}
}

char Lexer::peekNext() const {
	if(pos + 1 < source.length()) {
		return source[pos + 1];
	} else {
		return '\0';
	}
}

char Lexer::advance() {
	if(atEnd()) return '\0';
	
	char c = source[pos];
	pos++;
	
	if(c == '\n') {
		line++;
		col = 1;
	} else {
		col++;
	}
	
	return c;
}

bool Lexer::atEnd() const {
	return pos >= source.length();
}

bool Lexer::isSpace(char c) const {
	return std::isspace(static_cast<unsigned char>(c));
}

bool Lexer::isDigit(char c) const {
	return std::isdigit(static_cast<unsigned char>(c));
}

bool Lexer::isAlpha(char c) const {
	return std::isalpha(static_cast<unsigned char>(c));
}

void Lexer::lexNumber() {
	size_t start = pos;
	int startCol = col;
	
	while(isDigit(peek())) advance(); // Consume Continuous Integer
	
	if(peek() == '.' && isDigit(peekNext())) {
		advance(); // Consume Decimal
		while(isDigit(peek())) advance(); // Consume Continuous Fractional
	}
	
	std::string text = source.substr(start, pos - start);
	
	addToken(TokenType::NUM, text, line, startCol);
}

void Lexer::lexIdentifier() {
	size_t start = pos;
	int startCol = col;
	
	advance(); // Consume first required alpha char
	
	while(isAlpha(peek()) || peek() == '_' || isDigit(peek())) advance(); // Continuous ALNUM characters
	
	std::string text = source.substr(start, pos - start);
	
	TokenType type;
	auto it = keywords.find(text);
	if(it != keywords.end()) {
		type = it->second;
	} else {
		type = TokenType::IDENT;
	}
	
	addToken(type, text, line, startCol);
}

void Lexer::addSingle(TokenType type) {
	int startCol = col;
	char c = advance();
	tokens.push_back(Token{type, std::string(1, c), 0.0, line, startCol});
}

void Lexer::addToken(TokenType type, std::string text, int l, int c) {
	if(type == TokenType::NUM) {
		tokens.push_back(Token{type, text, std::stod(text), l, c});
	} else {
		tokens.push_back(Token{type, text, 0.0, l, c});
	}
}

void Lexer::debug_print_tokens() {
	for(auto T : tokens) {
		std::cout << tokenTypeName(T.type) << std::endl;
	}
}