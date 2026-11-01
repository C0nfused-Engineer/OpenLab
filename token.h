// token.h
#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType {
	IDENT,
	NUM,
	PLUS,
	MINUS,
	ASTR,
	SLASH,
	LPAREN,
	RPAREN,
	LBRACK,
	RBRACK,
	LBRACE,
	RBRACE,
	SEMI,
	COMMA,
	CARET,
	ASSIGN,
	IF,
	ELSE,
	WHILE,
	INT,
	FLOAT,
	STR,
	AND,
	OR,
	EQUAL_EQUAL,
	GREATER,
	GREATER_EQUAL,
	LESSER,
	LESSER_EQUAL,
	NOT,
	NOT_EQUAL,
	COMMENT,
	TRU,
	FAL,
	NEWLINE,
	END
};

// Tokens to add
// [], {}, !, %, &&, ||, x||, >, < >=, <=, !=, +=, -=, /=, *=, .*, ./, .^, e, pi, ++, --, for

struct Token {
	TokenType type;
	std::string text;
	double number = 0.0;
	int line = 1;
	int col = 1;
};

std::string opSymbol(TokenType op);
std::string tokenTypeName(TokenType t);

#endif